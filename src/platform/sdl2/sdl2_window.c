#include "platform/window.h"
#include "SDL2/SDL_system.h"
#include "SDL2/SDL_syswm.h"

SWindowHandle skr_create_window(const char8_t* name, const SWindowDescroptor* desc)
{
    uint32_t flags = SDL_WINDOW_VULKAN;
    flags = desc->resizable ? (flags | SDL_WINDOW_RESIZABLE) : flags;
    flags |= SDL_WINDOW_ALLOW_HIGHDPI;
    SDL_Window* sdl_window = SDL_CreateWindow(name,
        desc->centered ? SDL_WINDOWPOS_CENTERED : desc->posx,
        desc->centered ? SDL_WINDOWPOS_CENTERED : desc->posy,
        desc->width, desc->height, flags);
    return (SWindowHandle)sdl_window;
}

void skr_window_set_title(SWindowHandle window, const char8_t* name)
{
    SDL_SetWindowTitle((SDL_Window*)window, name);
}

void skr_window_get_extent(SWindowHandle window, int32_t* width, int32_t* height)
{
    SDL_Window* sdl_window = (SDL_Window*)window;
    SDL_GetWindowSize(sdl_window, width, height);
}

bool skr_window_is_focused(SWindowHandle window)
{
    uint32_t flags = SDL_GetWindowFlags((SDL_Window*)window);
    return (flags & SDL_WINDOW_MOUSE_FOCUS) || (flags & SDL_WINDOW_INPUT_FOCUS);
}

SWindowHandle skr_get_mouse_focused_window()
{
    return (SWindowHandle)SDL_GetMouseFocus();
}

void* skr_window_get_native_handle(SWindowHandle window)
{
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    SDL_GetWindowWMInfo((SDL_Window*)window, &wmInfo);
#ifdef SKR_OS_WINDOWS
    return wmInfo.info.win.window;
#elif defined(SKR_OS_MACOSX)
    return wmInfo.info.cocoa.window;
#endif
    return NULL;
}

#ifdef SKR_OS_MACOSX
    #include "platform/apple/macos/window.h"
#endif

void* skr_window_get_native_view(SWindowHandle window)
{
#ifdef SKR_OS_MACOSX
    void* ns_view =
        nswindow_get_content_view(skr_window_get_native_handle(window));
    return ns_view;
#else
    return skr_window_get_native_handle(window);
#endif
}

void skr_free_window(SWindowHandle window)
{
    SDL_DestroyWindow((SDL_Window*)window);
}