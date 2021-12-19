#pragma once
#include "cgpu_config.h"
#include "assert.h"

#ifdef __cplusplus
extern "C" {
#endif

// enums
typedef enum ECGpuNvAPI_Status
{
    CGPU_NVAPI_OK = 0, //!< Success. Request is completed.
    CGPU_NVAPI_NONE = 1,
    CGPU_NVAPI_ERROR = -1, //!< Generic error
} ECGpuNvAPI_Status;

typedef enum ECGpuAGSReturnCode
{
    CGPU_AGS_SUCCESS,                 ///< Successful function call
    CGPU_AGS_FAILURE,                 ///< Failed to complete call for some unspecified reason
    CGPU_AGS_INVALID_ARGS,            ///< Invalid arguments into the function
    CGPU_AGS_OUT_OF_MEMORY,           ///< Out of memory when allocating space internally
    CGPU_AGS_MISSING_D3D_DLL,         ///< Returned when a D3D dll fails to load
    CGPU_AGS_LEGACY_DRIVER,           ///< Returned if a feature is not present in the installed driver
    CGPU_AGS_NO_AMD_DRIVER_INSTALLED, ///< Returned if the AMD GPU driver does not appear to be installed
    CGPU_AGS_EXTENSION_NOT_SUPPORTED, ///< Returned if the driver does not support the requested driver extension
    CGPU_AGS_ADL_FAILURE,             ///< Failure in ADL (the AMD Display Library)
    CGPU_AGS_DX_FAILURE,              ///< Failure from DirectX runtime
    CGPU_AGS_NONE
} ECGpuAGSReturnCode;

// Format
typedef enum ECGpuFormat
{
    PF_UNDEFINED = 0,
    PF_R1_UNORM = 1,
    PF_R2_UNORM = 2,
    PF_R4_UNORM = 3,
    PF_R4G4_UNORM = 4,
    PF_G4R4_UNORM = 5,
    PF_A8_UNORM = 6,
    PF_R8_UNORM = 7,
    PF_R8_SNORM = 8,
    PF_R8_UINT = 9,
    PF_R8_SINT = 10,
    PF_R8_SRGB = 11,
    PF_B2G3R3_UNORM = 12,
    PF_R4G4B4A4_UNORM = 13,
    PF_R4G4B4X4_UNORM = 14,
    PF_B4G4R4A4_UNORM = 15,
    PF_B4G4R4X4_UNORM = 16,
    PF_A4R4G4B4_UNORM = 17,
    PF_X4R4G4B4_UNORM = 18,
    PF_A4B4G4R4_UNORM = 19,
    PF_X4B4G4R4_UNORM = 20,
    PF_R5G6B5_UNORM = 21,
    PF_B5G6R5_UNORM = 22,
    PF_R5G5B5A1_UNORM = 23,
    PF_B5G5R5A1_UNORM = 24,
    PF_A1B5G5R5_UNORM = 25,
    PF_A1R5G5B5_UNORM = 26,
    PF_R5G5B5X1_UNORM = 27,
    PF_B5G5R5X1_UNORM = 28,
    PF_X1R5G5B5_UNORM = 29,
    PF_X1B5G5R5_UNORM = 30,
    PF_B2G3R3A8_UNORM = 31,
    PF_R8G8_UNORM = 32,
    PF_R8G8_SNORM = 33,
    PF_G8R8_UNORM = 34,
    PF_G8R8_SNORM = 35,
    PF_R8G8_UINT = 36,
    PF_R8G8_SINT = 37,
    PF_R8G8_SRGB = 38,
    PF_R16_UNORM = 39,
    PF_R16_SNORM = 40,
    PF_R16_UINT = 41,
    PF_R16_SINT = 42,
    PF_R16_SFLOAT = 43,
    PF_R16_SBFLOAT = 44,
    PF_R8G8B8_UNORM = 45,
    PF_R8G8B8_SNORM = 46,
    PF_R8G8B8_UINT = 47,
    PF_R8G8B8_SINT = 48,
    PF_R8G8B8_SRGB = 49,
    PF_B8G8R8_UNORM = 50,
    PF_B8G8R8_SNORM = 51,
    PF_B8G8R8_UINT = 52,
    PF_B8G8R8_SINT = 53,
    PF_B8G8R8_SRGB = 54,
    PF_R8G8B8A8_UNORM = 55,
    PF_R8G8B8A8_SNORM = 56,
    PF_R8G8B8A8_UINT = 57,
    PF_R8G8B8A8_SINT = 58,
    PF_R8G8B8A8_SRGB = 59,
    PF_B8G8R8A8_UNORM = 60,
    PF_B8G8R8A8_SNORM = 61,
    PF_B8G8R8A8_UINT = 62,
    PF_B8G8R8A8_SINT = 63,
    PF_B8G8R8A8_SRGB = 64,
    PF_R8G8B8X8_UNORM = 65,
    PF_B8G8R8X8_UNORM = 66,
    PF_R16G16_UNORM = 67,
    PF_G16R16_UNORM = 68,
    PF_R16G16_SNORM = 69,
    PF_G16R16_SNORM = 70,
    PF_R16G16_UINT = 71,
    PF_R16G16_SINT = 72,
    PF_R16G16_SFLOAT = 73,
    PF_R16G16_SBFLOAT = 74,
    PF_R32_UINT = 75,
    PF_R32_SINT = 76,
    PF_R32_SFLOAT = 77,
    PF_A2R10G10B10_UNORM = 78,
    PF_A2R10G10B10_UINT = 79,
    PF_A2R10G10B10_SNORM = 80,
    PF_A2R10G10B10_SINT = 81,
    PF_A2B10G10R10_UNORM = 82,
    PF_A2B10G10R10_UINT = 83,
    PF_A2B10G10R10_SNORM = 84,
    PF_A2B10G10R10_SINT = 85,
    PF_R10G10B10A2_UNORM = 86,
    PF_R10G10B10A2_UINT = 87,
    PF_R10G10B10A2_SNORM = 88,
    PF_R10G10B10A2_SINT = 89,
    PF_B10G10R10A2_UNORM = 90,
    PF_B10G10R10A2_UINT = 91,
    PF_B10G10R10A2_SNORM = 92,
    PF_B10G10R10A2_SINT = 93,
    PF_B10G11R11_UFLOAT = 94,
    PF_E5B9G9R9_UFLOAT = 95,
    PF_R16G16B16_UNORM = 96,
    PF_R16G16B16_SNORM = 97,
    PF_R16G16B16_UINT = 98,
    PF_R16G16B16_SINT = 99,
    PF_R16G16B16_SFLOAT = 100,
    PF_R16G16B16_SBFLOAT = 101,
    PF_R16G16B16A16_UNORM = 102,
    PF_R16G16B16A16_SNORM = 103,
    PF_R16G16B16A16_UINT = 104,
    PF_R16G16B16A16_SINT = 105,
    PF_R16G16B16A16_SFLOAT = 106,
    PF_R16G16B16A16_SBFLOAT = 107,
    PF_R32G32_UINT = 108,
    PF_R32G32_SINT = 109,
    PF_R32G32_SFLOAT = 110,
    PF_R32G32B32_UINT = 111,
    PF_R32G32B32_SINT = 112,
    PF_R32G32B32_SFLOAT = 113,
    PF_R32G32B32A32_UINT = 114,
    PF_R32G32B32A32_SINT = 115,
    PF_R32G32B32A32_SFLOAT = 116,
    PF_R64_UINT = 117,
    PF_R64_SINT = 118,
    PF_R64_SFLOAT = 119,
    PF_R64G64_UINT = 120,
    PF_R64G64_SINT = 121,
    PF_R64G64_SFLOAT = 122,
    PF_R64G64B64_UINT = 123,
    PF_R64G64B64_SINT = 124,
    PF_R64G64B64_SFLOAT = 125,
    PF_R64G64B64A64_UINT = 126,
    PF_R64G64B64A64_SINT = 127,
    PF_R64G64B64A64_SFLOAT = 128,
    PF_D16_UNORM = 129,
    PF_X8_D24_UNORM = 130,
    PF_D32_SFLOAT = 131,
    PF_S8_UINT = 132,
    PF_D16_UNORM_S8_UINT = 133,
    PF_D24_UNORM_S8_UINT = 134,
    PF_D32_SFLOAT_S8_UINT = 135,
    PF_DXBC1_RGB_UNORM = 136,
    PF_DXBC1_RGB_SRGB = 137,
    PF_DXBC1_RGBA_UNORM = 138,
    PF_DXBC1_RGBA_SRGB = 139,
    PF_DXBC2_UNORM = 140,
    PF_DXBC2_SRGB = 141,
    PF_DXBC3_UNORM = 142,
    PF_DXBC3_SRGB = 143,
    PF_DXBC4_UNORM = 144,
    PF_DXBC4_SNORM = 145,
    PF_DXBC5_UNORM = 146,
    PF_DXBC5_SNORM = 147,
    PF_DXBC6H_UFLOAT = 148,
    PF_DXBC6H_SFLOAT = 149,
    PF_DXBC7_UNORM = 150,
    PF_DXBC7_SRGB = 151,
    PF_PVRTC1_2BPP_UNORM = 152,
    PF_PVRTC1_4BPP_UNORM = 153,
    PF_PVRTC2_2BPP_UNORM = 154,
    PF_PVRTC2_4BPP_UNORM = 155,
    PF_PVRTC1_2BPP_SRGB = 156,
    PF_PVRTC1_4BPP_SRGB = 157,
    PF_PVRTC2_2BPP_SRGB = 158,
    PF_PVRTC2_4BPP_SRGB = 159,
    PF_ETC2_R8G8B8_UNORM = 160,
    PF_ETC2_R8G8B8_SRGB = 161,
    PF_ETC2_R8G8B8A1_UNORM = 162,
    PF_ETC2_R8G8B8A1_SRGB = 163,
    PF_ETC2_R8G8B8A8_UNORM = 164,
    PF_ETC2_R8G8B8A8_SRGB = 165,
    PF_ETC2_EAC_R11_UNORM = 166,
    PF_ETC2_EAC_R11_SNORM = 167,
    PF_ETC2_EAC_R11G11_UNORM = 168,
    PF_ETC2_EAC_R11G11_SNORM = 169,
    PF_ASTC_4x4_UNORM = 170,
    PF_ASTC_4x4_SRGB = 171,
    PF_ASTC_5x4_UNORM = 172,
    PF_ASTC_5x4_SRGB = 173,
    PF_ASTC_5x5_UNORM = 174,
    PF_ASTC_5x5_SRGB = 175,
    PF_ASTC_6x5_UNORM = 176,
    PF_ASTC_6x5_SRGB = 177,
    PF_ASTC_6x6_UNORM = 178,
    PF_ASTC_6x6_SRGB = 179,
    PF_ASTC_8x5_UNORM = 180,
    PF_ASTC_8x5_SRGB = 181,
    PF_ASTC_8x6_UNORM = 182,
    PF_ASTC_8x6_SRGB = 183,
    PF_ASTC_8x8_UNORM = 184,
    PF_ASTC_8x8_SRGB = 185,
    PF_ASTC_10x5_UNORM = 186,
    PF_ASTC_10x5_SRGB = 187,
    PF_ASTC_10x6_UNORM = 188,
    PF_ASTC_10x6_SRGB = 189,
    PF_ASTC_10x8_UNORM = 190,
    PF_ASTC_10x8_SRGB = 191,
    PF_ASTC_10x10_UNORM = 192,
    PF_ASTC_10x10_SRGB = 193,
    PF_ASTC_12x10_UNORM = 194,
    PF_ASTC_12x10_SRGB = 195,
    PF_ASTC_12x12_UNORM = 196,
    PF_ASTC_12x12_SRGB = 197,
    PF_CLUT_P4 = 198,
    PF_CLUT_P4A4 = 199,
    PF_CLUT_P8 = 200,
    PF_CLUT_P8A8 = 201,
    PF_R4G4B4A4_UNORM_PACK16 = 202,
    PF_B4G4R4A4_UNORM_PACK16 = 203,
    PF_R5G6B5_UNORM_PACK16 = 204,
    PF_B5G6R5_UNORM_PACK16 = 205,
    PF_R5G5B5A1_UNORM_PACK16 = 206,
    PF_B5G5R5A1_UNORM_PACK16 = 207,
    PF_A1R5G5B5_UNORM_PACK16 = 208,
    PF_G16B16G16R16_422_UNORM = 209,
    PF_B16G16R16G16_422_UNORM = 210,
    PF_R12X4G12X4B12X4A12X4_UNORM_4PACK16 = 211,
    PF_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16 = 212,
    PF_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16 = 213,
    PF_R10X6G10X6B10X6A10X6_UNORM_4PACK16 = 214,
    PF_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16 = 215,
    PF_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16 = 216,
    PF_G8B8G8R8_422_UNORM = 217,
    PF_B8G8R8G8_422_UNORM = 218,
    PF_G8_B8_R8_3PLANE_420_UNORM = 219,
    PF_G8_B8R8_2PLANE_420_UNORM = 220,
    PF_G8_B8_R8_3PLANE_422_UNORM = 221,
    PF_G8_B8R8_2PLANE_422_UNORM = 222,
    PF_G8_B8_R8_3PLANE_444_UNORM = 223,
    PF_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16 = 224,
    PF_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16 = 225,
    PF_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16 = 226,
    PF_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16 = 227,
    PF_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16 = 228,
    PF_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16 = 229,
    PF_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16 = 230,
    PF_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16 = 231,
    PF_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16 = 232,
    PF_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16 = 233,
    PF_G16_B16_R16_3PLANE_420_UNORM = 234,
    PF_G16_B16_R16_3PLANE_422_UNORM = 235,
    PF_G16_B16_R16_3PLANE_444_UNORM = 236,
    PF_G16_B16R16_2PLANE_420_UNORM = 237,
    PF_G16_B16R16_2PLANE_422_UNORM = 238,
    PF_Count = PF_G16_B16R16_2PLANE_422_UNORM + 1
} ECGpuFormat;

typedef enum ECGpuLoadAction
{
    LA_DONTCARE,
    LA_LOAD,
    LA_CLEAR,
    LA_COUNT
} ECGpuLoadAction;

typedef enum ECGpuPrimitiveTopology
{
    TOPO_POINT_LIST = 0,
    TOPO_LINE_LIST,
    TOPO_LINE_STRIP,
    TOPO_TRI_LIST,
    TOPO_TRI_STRIP,
    TOPO_PATCH_LIST,
    TOPO_COUNT,
} ECGpuPrimitiveTopology;

typedef enum ECGpuBlendConstant
{
    BC_ZERO = 0,
    BC_ONE,
    BC_SRC_COLOR,
    BC_ONE_MINUS_SRC_COLOR,
    BC_DST_COLOR,
    BC_ONE_MINUS_DST_COLOR,
    BC_SRC_ALPHA,
    BC_ONE_MINUS_SRC_ALPHA,
    BC_DST_ALPHA,
    BC_ONE_MINUS_DST_ALPHA,
    BC_SRC_ALPHA_SATURATE,
    BC_BLEND_FACTOR,
    BC_ONE_MINUS_BLEND_FACTOR,
    MAX_BLEND_CONSTANTS
} ECGpuBlendConstant;

typedef enum ECGpuCullMode
{
    CM_NONE = 0,
    CM_BACK,
    CM_FRONT,
    CM_BOTH,
    CM_COUNT
} ECGpuCullMode;

typedef enum ECGpuFrontFace
{
    FF_CCW = 0,
    FF_CW
} ECGpuFrontFace;

typedef enum ECGpuFillMode
{
    FM_SOLID,
    FM_WIREFRAME,
    FM_COUNT
} ECGpuFillMode;

typedef enum ECGpuVertexAttribRate
{
    VAR_VERTEX = 0,
    VAR_INSTANCE = 1,
    VAR_COUNT,
} ECGpuVertexAttribRate;

typedef enum ECGpuCompareMode
{
    CMP_NEVER,
    CMP_LESS,
    CMP_EQUAL,
    CMP_LEQUAL,
    CMP_GREATER,
    CMP_NOTEQUAL,
    CMP_GEQUAL,
    CMP_ALWAYS,
    MAX_COMPARE_MODES,
} ECGpuCompareMode;

typedef enum ECGpuStencilOp
{
    STENCIL_OP_KEEP,
    STENCIL_OP_SET_ZERO,
    STENCIL_OP_REPLACE,
    STENCIL_OP_INVERT,
    STENCIL_OP_INCR,
    STENCIL_OP_DECR,
    STENCIL_OP_INCR_SAT,
    STENCIL_OP_DECR_SAT,
    MAX_STENCIL_OPS,
} ECGpuStencilOp;

typedef enum ECGpuBlendMode
{
    BM_ADD,
    BM_SUBTRACT,
    BM_REVERSE_SUBTRACT,
    BM_MIN,
    BM_MAX,
    MAX_BLEND_MODES,
} ECGpuBlendMode;

typedef enum ECGpuTextureDimension
{
    TD_1D,
    TD_2D,
    TD_2DMS,
    TD_3D,
    TD_CUBE,
    TD_1D_ARRAY,
    TD_2D_ARRAY,
    TD_2DMS_ARRAY,
    TD_CUBE_ARRAY,
    TD_COUNT,
    TD_UNDEFINED,
} ECGpuTextureDimension;

// Same Value As Vulkan Enumeration Bits.
typedef enum ECGpuShaderStage
{
    SS_NONE = 0,

    SS_VERT = 0X00000001,
    SS_TESC = 0X00000002,
    SS_TESE = 0X00000004,
    SS_GEOM = 0X00000008,
    SS_FRAG = 0X00000010,
    SS_COMPUTE = 0X00000020,

    SS_ALL_GRAPHICS = (uint32_t)SS_VERT | (uint32_t)SS_TESC | (uint32_t)SS_TESE | (uint32_t)SS_GEOM | (uint32_t)SS_FRAG,
    SS_COUNT = 6
} ECGpuShaderStage;
typedef uint32_t CGpuShaderStages;

typedef enum ECGpuResourceState
{
    RS_UNDEFINED = 0,
    RS_VERTEX_AND_CONSTANT_BUFFER = 0x1,
    RS_INDEX_BUFFER = 0x2,
    RS_RENDER_TARGET = 0x4,
    RS_UNORDERED_ACCESS = 0x8,
    RS_DEPTH_WRITE = 0x10,
    RS_DEPTH_READ = 0x20,
    RS_NON_PIXEL_SHADER_RESOURCE = 0x40,
    RS_PIXEL_SHADER_RESOURCE = 0x80,
    RS_SHADER_RESOURCE = 0x40 | 0x80,
    RS_STREAM_OUT = 0x100,
    RS_INDIRECT_ARGUMENT = 0x200,
    RS_COPY_DEST = 0x400,
    RS_COPY_SOURCE = 0x800,
    RS_GENERIC_READ = (((((0x1 | 0x2) | 0x40) | 0x80) | 0x200) | 0x800),
    RS_PRESENT = 0x1000,
    RS_COMMON = 0x2000,
    RS_RAYTRACING_ACCELERATION_STRUCTURE = 0x4000,
    RS_SHADING_RATE_SOURCE = 0x8000,
} ECGpuResourceState;
typedef uint32_t ECGpuResourceStates;

typedef enum ECGpuMemoryUsage
{
    /// No intended memory usage specified.
    MU_UNKNOWN = 0,
    /// Memory will be used on device only, no need to be mapped on host.
    MU_GPU_ONLY = 1,
    /// Memory will be mapped on host. Could be used for transfer to device.
    MU_CPU_ONLY = 2,
    /// Memory will be used for frequent (dynamic) updates from host and reads on device.
    MU_CPU_TO_GPU = 3,
    /// Memory will be used for writing on device and readback on host.
    MU_GPU_TO_CPU = 4,
    MU_COUNT,
    MU_MAX_ENUM = 0x7FFFFFFF
} ECGpuMemoryUsage;

typedef enum ECGpuBufferCreationFlag
{
    /// Default flag (Buffer will use aliased memory, buffer will not be cpu accessible until mapBuffer is called)
    BCF_NONE = 0x01,
    /// Buffer will allocate its own memory (COMMITTED resource)
    BCF_OWN_MEMORY_BIT = 0x02,
    /// Buffer will be persistently mapped
    BCF_PERSISTENT_MAP_BIT = 0x04,
    /// Use ESRAM to store this buffer
    BCF_ESRAM = 0x08,
    /// Flag to specify not to allocate descriptors for the resource
    BCF_NO_DESCRIPTOR_VIEW_CREATION = 0x10,
#ifdef CGPU_USE_METAL
    /* ICB Flags */
    /// Inherit pipeline in ICB
    BCF_ICB_INHERIT_PIPELINE = 0x100,
    /// Inherit pipeline in ICB
    BCF_ICB_INHERIT_BUFFERS = 0x200,
#endif
} ECGpuBufferCreationFlag;
typedef uint32_t CGpuBufferCreationFlags;

typedef enum ECGpuTextureCreationFlag
{
    /// Default flag (Texture will use default allocation strategy decided by the api specific allocator)
    TCF_NONE = 0,
    /// Texture will allocate its own memory (COMMITTED resource)
    TCF_OWN_MEMORY_BIT = 0x01,
    /// Texture will be allocated in memory which can be shared among multiple processes
    TCF_EXPORT_BIT = 0x02,
    /// Texture will be allocated in memory which can be shared among multiple gpus
    TCF_EXPORT_ADAPTER_BIT = 0x04,
    /// Texture will be imported from a handle created in another process
    TCF_IMPORT_BIT = 0x08,
    /// Use ESRAM to store this texture
    TCF_ESRAM = 0x10,
    /// Use on-tile memory to store this texture
    TCF_ON_TILE = 0x20,
    /// Prevent compression meta data from generating (XBox)
    TCF_NO_COMPRESSION = 0x40,
    /// Force 2D instead of automatically determining dimension based on width, height, depth
    TCF_FORCE_2D = 0x80,
    /// Force 3D instead of automatically determining dimension based on width, height, depth
    TCF_FORCE_3D = 0x100,
    /// Display target
    TCF_ALLOW_DISPLAY_TARGET = 0x200,
    /// Create an sRGB texture.
    TCF_SRGB = 0x400,
    /// Create a normal map texture
    TCF_NORMAL_MAP = 0x800,
    /// Fast clear
    TCF_FAST_CLEAR = 0x1000,
    /// Fragment mask
    TCF_FRAG_MASK = 0x2000
} ECGpuTextureCreationFlag;
typedef uint32_t CGpuTextureCreationFlags;

typedef enum ECGpuSampleCount
{
    SC_1 = 1,
    SC_2 = 2,
    SC_4 = 4,
    SC_8 = 8,
    SC_16 = 16,
} ECGpuSampleCount;

typedef enum ECGpuPipelineType
{
    PT_NONE = 0,
    PT_COMPUTE,
    PT_GRAPHICS,
    PT_RAYTRACING,
    PT_COUNT,
} ECGpuPipelineType;

typedef enum ECGpuResourceType
{
    RT_NONE = 0,
    RT_SAMPLER = 0x01,
    // SRV Read only texture
    RT_TEXTURE = (RT_SAMPLER << 1),
    /// UAV Texture
    RT_RW_TEXTURE = (RT_TEXTURE << 1),
    // SRV Read only buffer
    RT_BUFFER = (RT_RW_TEXTURE << 1),
    RT_BUFFER_RAW = (RT_BUFFER | (RT_BUFFER << 1)),
    /// UAV Buffer
    RT_RW_BUFFER = (RT_BUFFER << 2),
    RT_RW_BUFFER_RAW = (RT_RW_BUFFER | (RT_RW_BUFFER << 1)),
    /// Uniform buffer
    RT_UNIFORM_BUFFER = (RT_RW_BUFFER << 2),
    /// Push constant / Root constant
    RT_ROOT_CONSTANT = (RT_UNIFORM_BUFFER << 1),
    /// IA
    RT_VERTEX_BUFFER = (RT_ROOT_CONSTANT << 1),
    RT_INDEX_BUFFER = (RT_VERTEX_BUFFER << 1),
    RT_INDIRECT_BUFFER = (RT_INDEX_BUFFER << 1),
    /// Cubemap SRV
    RT_TEXTURE_CUBE = (RT_TEXTURE | (RT_INDIRECT_BUFFER << 1)),
    /// RTV / DSV per mip slice
    RT_RENDER_TARGET_MIP_SLICES = (RT_INDIRECT_BUFFER << 2),
    /// RTV / DSV per array slice
    RT_RENDER_TARGET_ARRAY_SLICES = (RT_RENDER_TARGET_MIP_SLICES << 1),
    /// RTV / DSV per depth slice
    RT_RENDER_TARGET_DEPTH_SLICES = (RT_RENDER_TARGET_ARRAY_SLICES << 1),
    RT_RAY_TRACING = (RT_RENDER_TARGET_DEPTH_SLICES << 1),
#if defined(CGPU_USE_VULKAN)
    /// Subpass input (descriptor type only available in Vulkan)
    RT_INPUT_ATTACHMENT = (RT_RAY_TRACING << 1),
    RT_TEXEL_BUFFER = (RT_INPUT_ATTACHMENT << 1),
    RT_RW_TEXEL_BUFFER = (RT_TEXEL_BUFFER << 1),
    RT_COMBINED_IMAGE_SAMPLER = (RT_RW_TEXEL_BUFFER << 1),
#endif
#if defined(CGPU_USE_METAL)
    RT_ARGUMENT_BUFFER = (RT_RAY_TRACING << 1),
    RT_INDIRECT_COMMAND_BUFFER = (RT_ARGUMENT_BUFFER << 1),
    RT_RENDER_PIPELINE_STATE = (RT_INDIRECT_COMMAND_BUFFER << 1),
#endif
} ECGpuResourceType;
typedef uint32_t CGpuResourceTypes;

#ifdef __cplusplus
} // end extern "C"
#endif
