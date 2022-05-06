/* Proposed SG14 status_code
(C) 2018 Niall Douglas <http://www.nedproductions.biz/> (5 commits)
File Created: Feb 2018


Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License in the accompanying file
Licence.txt or at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.


Distributed under the Boost Software License, Version 1.0.
(See accompanying file Licence.txt or copy at
http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef SYSTEM_ERROR2_STATUS_CODE_DOMAIN_HPP
#define SYSTEM_ERROR2_STATUS_CODE_DOMAIN_HPP

#include "config.hpp"

#include <cstring>  // for strchr

SYSTEM_ERROR2_NAMESPACE_BEGIN

/*! The main workhorse of the system_error2 library, can be typed (`status_code<DomainType>`), erased-immutable (`status_code<void>`) or erased-mutable (`status_code<erased<T>>`).

Be careful of placing these into containers! Equality and inequality operators are
*semantic* not exact. Therefore two distinct items will test true! To help prevent
surprise on this, `operator<` and `std::hash<>` are NOT implemented in order to
trap potential incorrectness. Define your own custom comparison functions for your
container which perform exact comparisons.
*/
template <class DomainType> class status_code;
class _generic_code_domain;
//! The generic code is a status code with the generic code domain, which is that of `errc` (POSIX).
using generic_code = status_code<_generic_code_domain>;

namespace detail
{
  template <class StatusCode> class indirecting_domain;
  template <class T> struct status_code_sizer
  {
    void *a;
    T b;
  };
  template <class To, class From> struct type_erasure_is_safe
  {
    static constexpr bool value = traits::is_move_bitcopying<From>::value  //
                                  && (sizeof(status_code_sizer<From>) <= sizeof(status_code_sizer<To>));
  };
  /* We are severely limited by needing to retain C++ 11 compatibility when doing
  constexpr string parsing. MSVC lets you throw exceptions within a constexpr
  evaluation context when exceptions are globally disabled, but won't let you
  divide by zero, even if never evaluated, ever in constexpr. GCC and clang won't
  let you throw exceptions, ever, if exceptions are globally disabled. So let's
  use the trick of divide by zero in constexpr on GCC and clang if and only if
  exceptions are globally disabled.
  */
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdiv-by-zero"
#endif
#if defined(__cpp_exceptions) || (defined(_MSC_VER) && !defined(__clang__))
#define SYSTEM_ERROR2_FAIL_CONSTEXPR(msg) throw msg
#else
#define SYSTEM_ERROR2_FAIL_CONSTEXPR(msg) ((void) msg, 1 / 0)
#endif
  constexpr inline unsigned long long parse_hex_byte(char c) { return ('0' <= c && c <= '9') ? (c - '0') : ('a' <= c && c <= 'f') ? (10 + c - 'a') : ('A' <= c && c <= 'F') ? (10 + c - 'A') : SYSTEM_ERROR2_FAIL_CONSTEXPR("Invalid character in UUID"); }
  constexpr inline unsigned long long parse_uuid2(const char *s)
  {
    return ((parse_hex_byte(s[0]) << 0) | (parse_hex_byte(s[1]) << 4) | (parse_hex_byte(s[2]) << 8) | (parse_hex_byte(s[3]) << 12) | (parse_hex_byte(s[4]) << 16) | (parse_hex_byte(s[5]) << 20) | (parse_hex_byte(s[6]) << 24) | (parse_hex_byte(s[7]) << 28) | (parse_hex_byte(s[9]) << 32) | (parse_hex_byte(s[10]) << 36) |
            (parse_hex_byte(s[11]) << 40) | (parse_hex_byte(s[12]) << 44) | (parse_hex_byte(s[14]) << 48) | (parse_hex_byte(s[15]) << 52) | (parse_hex_byte(s[16]) << 56) | (parse_hex_byte(s[17]) << 60))  //
           ^                                                                                                                                                                                                //
           ((parse_hex_byte(s[19]) << 0) | (parse_hex_byte(s[20]) << 4) | (parse_hex_byte(s[21]) << 8) | (parse_hex_byte(s[22]) << 12) | (parse_hex_byte(s[24]) << 16) | (parse_hex_byte(s[25]) << 20) | (parse_hex_byte(s[26]) << 24) | (parse_hex_byte(s[27]) << 28) | (parse_hex_byte(s[28]) << 32) |
            (parse_hex_byte(s[29]) << 36) | (parse_hex_byte(s[30]) << 40) | (parse_hex_byte(s[31]) << 44) | (parse_hex_byte(s[32]) << 48) | (parse_hex_byte(s[33]) << 52) | (parse_hex_byte(s[34]) << 56) | (parse_hex_byte(s[35]) << 60));
  }
  template <size_t N> constexpr inline unsigned long long parse_uuid_from_array(const char (&uuid)[N]) { return (N == 37) ? parse_uuid2(uuid) : ((N == 39) ? parse_uuid2(uuid + 1) : SYSTEM_ERROR2_FAIL_CONSTEXPR("UUID does not have correct length")); }
  template <size_t N> constexpr inline unsigned long long parse_uuid_from_pointer(const char *uuid) { return (N == 36) ? parse_uuid2(uuid) : ((N == 38) ? parse_uuid2(uuid + 1) : SYSTEM_ERROR2_FAIL_CONSTEXPR("UUID does not have correct length")); }
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
  static constexpr unsigned long long test_uuid_parse = parse_uuid_from_array("430f1201-94fc-06c7-430f-120194fc06c7");
  //static constexpr unsigned long long test_uuid_parse2 = parse_uuid_from_array("x30f1201-94fc-06c7-430f-120194fc06c7");
}  // namespace detail

/*! Abstract base class for a coding domain of a status code.
 */
class status_code_domain
{
  template <class DomainType> friend class status_code;
  template <class StatusCode> friend class indirecting_domain;

public:
  //! Type of the unique id for this domain.
  using unique_id_type = unsigned long long;
  /*! (Potentially thread safe) Reference to a message string.

  Be aware that you cannot add payload to implementations of this class.
  You get exactly the `void *[3]` array to keep state, this is usually
  sufficient for a `std::shared_ptr<>` or a `std::string`.

  You can install a handler to be called when this object is copied,
  moved and destructed. This takes the form of a C function pointer.
  */
  class string_ref
  {
  public:
    //! The value type
    using value_type = const char;
    //! The size type
    using size_type = size_t;
    //! The pointer type
    using pointer = const char *;
    //! The const pointer type
    using const_pointer = const char *;
    //! The iterator type
    using iterator = const char *;
    //! The const iterator type
    using const_iterator = const char *;

  protected:
    //! The operation occurring
    enum class _thunk_op
    {
      copy,
      move,
      destruct
    };
    //! The prototype of the handler function. Copies can throw, moves and destructs cannot.
    using _thunk_spec = void (*)(string_ref *dest, const string_ref *src, _thunk_op op);
#ifndef NDEBUG
  private:
    static void _checking_string_thunk(string_ref *dest, const string_ref *src, _thunk_op /*unused*/) noexcept
    {
      (void) dest;
      (void) src;
      assert(dest->_thunk == _checking_string_thunk);                   // NOLINT
      assert(src == nullptr || src->_thunk == _checking_string_thunk);  // NOLINT
      // do nothing
    }

  protected:
#endif
    //! Pointers to beginning and end of character range
    pointer _begin{}, _end{};
    //! Three `void*` of state
    void *_state[3]{};  // at least the size of a shared_ptr
    //! Handler for when operations occur
    const _thunk_spec _thunk{nullptr};

    constexpr explicit string_ref(_thunk_spec thunk) noexcept
        : _thunk(thunk)
    {
    }

  public:
    //! Construct from a C string literal
    SYSTEM_ERROR2_CONSTEXPR14 explicit string_ref(const char *str, size_type len = static_cast<size_type>(-1), void *state0 = nullptr, void *state1 = nullptr, void *state2 = nullptr,
#ifndef NDEBUG
                                                  _thunk_spec thunk = _checking_string_thunk
#else
                                                  _thunk_spec thunk = nullptr
#endif
                                                  ) noexcept
        : _begin(str)
        , _end((len == static_cast<size_type>(-1)) ? (str + detail::cstrlen(str)) : (str + len))
        ,  // NOLINT
        _state{state0, state1, state2}
        , _thunk(thunk)
    {
    }
    //! Copy construct the derived implementation.
    SYSTEM_ERROR2_CONSTEXPR20 string_ref(const string_ref &o)
        : _begin(o._begin)
        , _end(o._end)
        , _state{o._state[0], o._state[1], o._state[2]}
        , _thunk(o._thunk)
    {
      if(_thunk != nullptr)
      {
        _thunk(this, &o, _thunk_op::copy);
      }
    }
    //! Move construct the derived implementation.
    SYSTEM_ERROR2_CONSTEXPR20 string_ref(string_ref &&o) noexcept
        : _begin(o._begin)
        , _end(o._end)
        , _state{o._state[0], o._state[1], o._state[2]}
        , _thunk(o._thunk)
    {
      if(_thunk != nullptr)
      {
        _thunk(this, &o, _thunk_op::move);
      }
    }
    //! Copy assignment
    SYSTEM_ERROR2_CONSTEXPR20 string_ref &operator=(const string_ref &o)
    {
      if(this != &o)
      {
#if defined(__cpp_exceptions) || defined(__EXCEPTIONS) || defined(_CPPUNWIND)
        string_ref temp(static_cast<string_ref &&>(*this));
        this->~string_ref();
        try
        {
          new(this) string_ref(o);  // may throw
        }
        catch(...)
        {
          new(this) string_ref(static_cast<string_ref &&>(temp));
          throw;
        }
#else
        this->~string_ref();
        new(this) string_ref(o);
#endif
      }
      return *this;
    }
    //! Move assignment
    SYSTEM_ERROR2_CONSTEXPR20 string_ref &operator=(string_ref &&o) noexcept
    {
      if(this != &o)
      {
        this->~string_ref();
        new(this) string_ref(static_cast<string_ref &&>(o));
      }
      return *this;
    }
    //! Destruction
    SYSTEM_ERROR2_CONSTEXPR20 ~string_ref()
    {
      if(_thunk != nullptr)
      {
        _thunk(this, nullptr, _thunk_op::destruct);
      }
      _begin = _end = nullptr;
    }

    //! Returns whether the reference is empty or not
    SYSTEM_ERROR2_NODISCARD constexpr bool empty() const noexcept { return _begin == _end; }
    //! Returns the size of the string
    constexpr size_type size() const noexcept { return _end - _begin; }
    //! Returns a null terminated C string
    constexpr const_pointer c_str() const noexcept { return _begin; }
    //! Returns a null terminated C string
    constexpr const_pointer data() const noexcept { return _begin; }
    //! Returns the beginning of the string
    SYSTEM_ERROR2_CONSTEXPR14 iterator begin() noexcept { return _begin; }
    //! Returns the beginning of the string
    SYSTEM_ERROR2_CONSTEXPR14 const_iterator begin() const noexcept { return _begin; }
    //! Returns the beginning of the string
    constexpr const_iterator cbegin() const noexcept { return _begin; }
    //! Returns the end of the string
    SYSTEM_ERROR2_CONSTEXPR14 iterator end() noexcept { return _end; }
    //! Returns the end of the string
    SYSTEM_ERROR2_CONSTEXPR14 const_iterator end() const noexcept { return _end; }
    //! Returns the end of the string
    constexpr const_iterator cend() const noexcept { return _end; }
  };

  /*! A reference counted, threadsafe reference to a message string.
   */
  class atomic_refcounted_string_ref : public string_ref
  {
    struct _allocated_msg
    {
      mutable std::atomic<unsigned> count{1};
    };
     _allocated_msg *&_msg() noexcept { return reinterpret_cast<_allocated_msg *&>(this->_state[0]); }  // NOLINT
     const _allocated_msg *_msg() const noexcept { return reinterpret_cast<const _allocated_msg *>(this->_state[0]); }  // NOLINT

    static SYSTEM_ERROR2_CONSTEXPR20 void _refcounted_string_thunk(string_ref *_dest, const string_ref *_src, _thunk_op op) noexcept
    {
      auto dest = static_cast<atomic_refcounted_string_ref *>(_dest);      // NOLINT
      auto src = static_cast<const atomic_refcounted_string_ref *>(_src);  // NOLINT
      (void) src;
      assert(dest->_thunk == _refcounted_string_thunk);                   // NOLINT
      assert(src == nullptr || src->_thunk == _refcounted_string_thunk);  // NOLINT
      switch(op)
      {
      case _thunk_op::copy:
      {
        if(dest->_msg() != nullptr)
        {
          auto count = dest->_msg()->count.fetch_add(1, std::memory_order_relaxed);
          (void) count;
          assert(count != 0);  // NOLINT
        }
        return;
      }
      case _thunk_op::move:
      {
        assert(src);                                                  // NOLINT
        auto msrc = const_cast<atomic_refcounted_string_ref *>(src);  // NOLINT
        msrc->_begin = msrc->_end = nullptr;
        msrc->_state[0] = msrc->_state[1] = msrc->_state[2] = nullptr;
        return;
      }
      case _thunk_op::destruct:
      {
        if(dest->_msg() != nullptr)
        {
          auto count = dest->_msg()->count.fetch_sub(1, std::memory_order_release);
          if(count == 1)
          {
            std::atomic_thread_fence(std::memory_order_acquire);
            free((void *) dest->_begin);  // NOLINT
            delete dest->_msg();          // NOLINT
          }
        }
      }
      }
    }

  public:
    //! Construct from a C string literal allocated using `malloc()`.
    explicit atomic_refcounted_string_ref(const char *str, size_type len = static_cast<size_type>(-1), void *state1 = nullptr, void *state2 = nullptr) noexcept
        : string_ref(str, len, new(std::nothrow) _allocated_msg, state1, state2, _refcounted_string_thunk)
    {
      if(_msg() == nullptr)
      {
        free((void *) this->_begin);  // NOLINT
        _msg() = nullptr;             // disabled
        this->_begin = "failed to get message from system";
        this->_end = strchr(this->_begin, 0);
        return;
      }
    }
  };

private:
  unique_id_type _id;

protected:
  /*! Use [https://www.random.org/cgi-bin/randbyte?nbytes=8&format=h](https://www.random.org/cgi-bin/randbyte?nbytes=8&format=h) to get a random 64 bit id.

  Do NOT make up your own value. Do NOT use zero.
  */
  constexpr explicit status_code_domain(unique_id_type id) noexcept
      : _id(id)
  {
  }
  /*! UUID constructor, where input is constexpr parsed into a `unique_id_type`.
   */
  template <size_t N>
  constexpr explicit status_code_domain(const char (&uuid)[N]) noexcept
      : _id(detail::parse_uuid_from_array<N>(uuid))
  {
  }
  template <size_t N> struct _uuid_size
  {
  };
  //! Alternative UUID constructor
  template <size_t N>
  constexpr explicit status_code_domain(const char *uuid, _uuid_size<N> /*unused*/) noexcept
      : _id(detail::parse_uuid_from_pointer<N>(uuid))
  {
  }
  //! No public copying at type erased level
  status_code_domain(const status_code_domain &) = default;
  //! No public moving at type erased level
  status_code_domain(status_code_domain &&) = default;
  //! No public assignment at type erased level
  status_code_domain &operator=(const status_code_domain &) = default;
  //! No public assignment at type erased level
  status_code_domain &operator=(status_code_domain &&) = default;
  //! No public destruction at type erased level
  ~status_code_domain() = default;

public:
  //! True if the unique ids match.
  constexpr bool operator==(const status_code_domain &o) const noexcept { return _id == o._id; }
  //! True if the unique ids do not match.
  constexpr bool operator!=(const status_code_domain &o) const noexcept { return _id != o._id; }
  //! True if this unique is lower than the other's unique id.
  constexpr bool operator<(const status_code_domain &o) const noexcept { return _id < o._id; }

  //! Returns the unique id used to identify identical category instances.
  constexpr unique_id_type id() const noexcept { return _id; }
  //! Name of this category.
  SYSTEM_ERROR2_CONSTEXPR20 virtual string_ref name() const noexcept = 0;

protected:
  //! True if code means failure.
  SYSTEM_ERROR2_CONSTEXPR20 virtual bool _do_failure(const status_code<void> &code) const noexcept = 0;
  //! True if code is (potentially non-transitively) equivalent to another code in another domain.
  SYSTEM_ERROR2_CONSTEXPR20 virtual bool _do_equivalent(const status_code<void> &code1, const status_code<void> &code2) const noexcept = 0;
  //! Returns the generic code closest to this code, if any.
  SYSTEM_ERROR2_CONSTEXPR20 virtual generic_code _generic_code(const status_code<void> &code) const noexcept = 0;
  //! Return a reference to a string textually representing a code.
  SYSTEM_ERROR2_CONSTEXPR20 virtual string_ref _do_message(const status_code<void> &code) const noexcept = 0;
#if defined(_CPPUNWIND) || defined(__EXCEPTIONS) || defined(STANDARDESE_IS_IN_THE_HOUSE)
  //! Throw a code as a C++ exception.
  SYSTEM_ERROR2_NORETURN SYSTEM_ERROR2_CONSTEXPR20 virtual void _do_throw_exception(const status_code<void> &code) const = 0;
#else
  // Keep a vtable slot for binary compatibility
  SYSTEM_ERROR2_NORETURN virtual void _do_throw_exception(const status_code<void> & /*code*/) const { abort(); }
#endif
  // For a `status_code<erased<T>>` only, copy from `src` to `dst`. Default implementation uses `memcpy()`.
  virtual void _do_erased_copy(status_code<void> &dst, const status_code<void> &src, size_t bytes) const { memcpy(&dst, &src, bytes); }  // NOLINT
  // For a `status_code<erased<T>>` only, destroy the erased value type. Default implementation does nothing.
  SYSTEM_ERROR2_CONSTEXPR20 virtual void _do_erased_destroy(status_code<void> &code, size_t bytes) const noexcept  // NOLINT
  {
    (void) code;
    (void) bytes;
  }
};

SYSTEM_ERROR2_NAMESPACE_END

#endif
