#pragma once
#include "cgpu_config.h"

#ifdef __cplusplus
extern "C" {
#endif

// enums
typedef enum ECGpuVertexFormat {
	VF_UCHAR2,
	VF_UCHAR4,
	VF_CHAR2,
	VF_CHAR4,
	VF_UCHAR2_NORM,
	VF_UCHAR4_NORM,
	VF_CHAR2_NORM,
	VF_CHAR4_NORM,
	VF_USHORT2,
	VF_USHORT4,
	VF_SHORT2,
	VF_SHORT4,
	VF_USHORT2_NORM,
	VF_USHORT4_NORM,
	VF_SHORT2_NORM,
	VF_SHORT4_NORM,
	VF_HALF2,
	VF_HALF4,
	VF_FLOAT,
	VF_FLOAT2,
	VF_FLOAT3,
	VF_FLOAT4,
	VF_UINT,
	VF_UINT2,
	VF_UINT3,
	VF_UINT4,
	VF_INT,
	VF_INT2,
	VF_INT3,
	VF_INT4,
	VF_COUNT,

	VF_R8G8_UNORM = VF_UCHAR2_NORM,
	VF_R8G8_NORM = VF_CHAR2_NORM,
	VF_R8G8B8A8_UNORM = VF_UCHAR4_NORM,
	VF_R8G8B8A8_NORM = VF_CHAR4_NORM,
	VF_R16G16_UNORM = VF_USHORT2_NORM,
	VF_R16G16_NORM = VF_SHORT2_NORM,
	VF_R16G16B16A16_UNORM = VF_USHORT4_NORM,
	VF_R16G16B16A16_NORM = VF_SHORT4_NORM,
	VF_R32G32B32A32_UINT = VF_UINT4,
	VF_R32G32B32A32_INT = VF_INT4,
	VF_R32G32B32A32_SINT = VF_INT4,
	VF_R32G32_UINT = VF_UINT2,
	VF_R32G32_INT = VF_INT2,
	VF_R32G32_SINT = VF_INT2,
} ECGpuVertexFormat;

typedef enum ECGpuPixelFormat {
	PF_R32G32B32A32_UINT = 0,
	PF_R32G32B32A32_SINT = 1,
	PF_R32G32B32A32_FLOAT = 2,
	PF_R32G32B32_UINT = 3,
	PF_R32G32B32_SINT = 4,
	PF_R32G32B32_FLOAT = 5,
	PF_R16G16B16A16_UNORM = 6,
	PF_R16G16B16A16_SNORM = 7,
	PF_R16G16B16A16_UINT = 8,
	PF_R16G16B16A16_SINT = 9,
	PF_R16G16B16A16_FLOAT = 10,
	PF_R32G32_UINT = 11,
	PF_R32G32_SINT = 12,
	PF_R32G32_FLOAT = 13,
	PF_R10G10B10A2_UNORM = 14,
	PF_R10G10B10A2_UINT = 15,
	PF_R9G9B9E5_UFLOAT = 16,
	PF_R8G8B8A8_UNORM = 17,
	PF_R8G8B8A8_SNORM = 18,
	PF_R8G8B8A8_UINT = 19,
	PF_R8G8B8A8_SINT = 20,
	PF_R8G8B8A8_UNORM_SRGB = 21,
	PF_B8G8R8A8_UNORM = 22,
	PF_B8G8R8A8_UNORM_SRGB = 23,
	PF_R11G11B10_FLOAT = 24,
	PF_R16G16_UNORM = 25,
	PF_R16G16_SNORM = 26,
	PF_R16G16_UINT = 27,
	PF_R16G16_SINT = 28,
	PF_R16G16_FLOAT = 29,
	PF_R32_UINT = 30,
	PF_R32_SINT = 31,
	PF_R32_FLOAT = 32,
	PF_B5G5R5A1_UNORM = 33,
	PF_B5G6R5_UNORM = 34,
	PF_R8G8_UNORM = 35,
	PF_R8G8_SNORM = 36,
	PF_R8G8_UINT = 37,
	PF_R8G8_SINT = 38,
	PF_R16_UNORM = 39,
	PF_R16_SNORM = 40,
	PF_R16_UINT = 41,
	PF_R16_SINT = 42,
	PF_R16_FLOAT = 43,
	PF_R8_UNORM = 44,
	PF_R8_SNORM = 45,
	PF_R8_UINT = 46,
	PF_R8_SINT = 47,
	PF_D24_UNORM_S8_UINT = 49,
	PF_D32_FLOAT = 50,
	PF_D16_UNORM = 51,
	PF_BC1_UNORM = 52,
	PF_BC1_UNORM_SRGB = 53,
	PF_BC2_UNORM = 54,
	PF_BC2_UNORM_SRGB = 55,
	PF_BC3_UNORM = 56,
	PF_BC3_UNORM_SRGB = 57,
	PF_BC4_UNORM = 58,
	PF_BC4_SNORM = 59,
	PF_BC5_UNORM = 60,
	PF_BC5_SNORM = 61,
	PF_BC6H_UF16 = 62,
	PF_BC6H_SF16 = 63,
	PF_BC7_UNORM = 64,
	PF_BC7_UNORM_SRGB = 65,
	PF_Count = PF_BC7_UNORM_SRGB + 1
} ECGpuPixelFormat;

typedef enum ECGpuShaderStage {
    SS_NONE    = 0,

    SS_VERT    = 0X00000001,
    SS_TESC    = 0X00000002,
    SS_TESE    = 0X00000004,
    SS_GEOM    = 0X00000008,
    SS_FRAG    = 0X00000010,
    SS_COMPUTE = 0X00000020,

    SS_ALL_GRAPHICS = (uint32_t)SS_VERT | (uint32_t)SS_TESC | (uint32_t)SS_TESE | (uint32_t)SS_GEOM | (uint32_t)SS_FRAG,
    SS_COUNT = 6 
} ECGpuShaderStage;
typedef uint32_t ECGpuShaderStages;

#ifdef __cplusplus
} // end extern "C"
#endif