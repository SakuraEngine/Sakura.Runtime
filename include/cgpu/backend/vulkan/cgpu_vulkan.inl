/* clang-format off */

inline static VkFormat pf_translate_to_vulkan(const ECGpuPixelFormat fmt)
{
switch (fmt) {

	case PF_UNDEFINED: return VK_FORMAT_UNDEFINED;
	case PF_G4R4_UNORM: return VK_FORMAT_R4G4_UNORM_PACK8;
	case PF_A4B4G4R4_UNORM: return VK_FORMAT_R4G4B4A4_UNORM_PACK16;
	case PF_A4R4G4B4_UNORM: return VK_FORMAT_B4G4R4A4_UNORM_PACK16;
	case PF_R5G6B5_UNORM: return VK_FORMAT_B5G6R5_UNORM_PACK16;
	case PF_B5G6R5_UNORM: return VK_FORMAT_R5G6B5_UNORM_PACK16;
	case PF_A1B5G5R5_UNORM: return VK_FORMAT_R5G5B5A1_UNORM_PACK16;
	case PF_A1R5G5B5_UNORM: return VK_FORMAT_B5G5R5A1_UNORM_PACK16;
	case PF_B5G5R5A1_UNORM: return VK_FORMAT_A1R5G5B5_UNORM_PACK16;
	case PF_A2B10G10R10_UNORM: return VK_FORMAT_A2B10G10R10_UNORM_PACK32;

	case PF_R8_UNORM: return VK_FORMAT_R8_UNORM;
	case PF_R8_SNORM: return VK_FORMAT_R8_SNORM;
	case PF_R8_UINT: return VK_FORMAT_R8_UINT;
	case PF_R8_SINT: return VK_FORMAT_R8_SINT;
	case PF_R8_SRGB: return VK_FORMAT_R8_SRGB;
	case PF_R8G8_UNORM: return VK_FORMAT_R8G8_UNORM;
	case PF_R8G8_SNORM: return VK_FORMAT_R8G8_SNORM;
	case PF_R8G8_UINT: return VK_FORMAT_R8G8_UINT;
	case PF_R8G8_SINT: return VK_FORMAT_R8G8_SINT;
	case PF_R8G8_SRGB: return VK_FORMAT_R8G8_SRGB;
	case PF_R8G8B8_UNORM: return VK_FORMAT_R8G8B8_UNORM;
	case PF_R8G8B8_SNORM: return VK_FORMAT_R8G8B8_SNORM;
	case PF_R8G8B8_UINT: return VK_FORMAT_R8G8B8_UINT;
	case PF_R8G8B8_SINT: return VK_FORMAT_R8G8B8_SINT;
	case PF_R8G8B8_SRGB: return VK_FORMAT_R8G8B8_SRGB;
	case PF_B8G8R8_UNORM: return VK_FORMAT_B8G8R8_UNORM;
	case PF_B8G8R8_SNORM: return VK_FORMAT_B8G8R8_SNORM;
	case PF_B8G8R8_UINT: return VK_FORMAT_B8G8R8_UINT;
	case PF_B8G8R8_SINT: return VK_FORMAT_B8G8R8_SINT;
	case PF_B8G8R8_SRGB: return VK_FORMAT_B8G8R8_SRGB;
	case PF_R8G8B8A8_UNORM: return VK_FORMAT_R8G8B8A8_UNORM;
	case PF_R8G8B8A8_SNORM: return VK_FORMAT_R8G8B8A8_SNORM;
	case PF_R8G8B8A8_UINT: return VK_FORMAT_R8G8B8A8_UINT;
	case PF_R8G8B8A8_SINT: return VK_FORMAT_R8G8B8A8_SINT;
	case PF_R8G8B8A8_SRGB: return VK_FORMAT_R8G8B8A8_SRGB;
	case PF_B8G8R8A8_UNORM: return VK_FORMAT_B8G8R8A8_UNORM;
	case PF_B8G8R8A8_SNORM: return VK_FORMAT_B8G8R8A8_SNORM;
	case PF_B8G8R8A8_UINT: return VK_FORMAT_B8G8R8A8_UINT;
	case PF_B8G8R8A8_SINT: return VK_FORMAT_B8G8R8A8_SINT;
	case PF_B8G8R8A8_SRGB: return VK_FORMAT_B8G8R8A8_SRGB;
	case PF_R16_UNORM: return VK_FORMAT_R16_UNORM;
	case PF_R16_SNORM: return VK_FORMAT_R16_SNORM;
	case PF_R16_UINT: return VK_FORMAT_R16_UINT;
	case PF_R16_SINT: return VK_FORMAT_R16_SINT;
	case PF_R16_SFLOAT: return VK_FORMAT_R16_SFLOAT;
	case PF_R16G16_UNORM: return VK_FORMAT_R16G16_UNORM;
	case PF_R16G16_SNORM: return VK_FORMAT_R16G16_SNORM;
	case PF_R16G16_UINT: return VK_FORMAT_R16G16_UINT;
	case PF_R16G16_SINT: return VK_FORMAT_R16G16_SINT;
	case PF_R16G16_SFLOAT: return VK_FORMAT_R16G16_SFLOAT;
	case PF_R16G16B16_UNORM: return VK_FORMAT_R16G16B16_UNORM;
	case PF_R16G16B16_SNORM: return VK_FORMAT_R16G16B16_SNORM;
	case PF_R16G16B16_UINT: return VK_FORMAT_R16G16B16_UINT;
	case PF_R16G16B16_SINT: return VK_FORMAT_R16G16B16_SINT;
	case PF_R16G16B16_SFLOAT: return VK_FORMAT_R16G16B16_SFLOAT;
	case PF_R16G16B16A16_UNORM: return VK_FORMAT_R16G16B16A16_UNORM;
	case PF_R16G16B16A16_SNORM: return VK_FORMAT_R16G16B16A16_SNORM;
	case PF_R16G16B16A16_UINT: return VK_FORMAT_R16G16B16A16_UINT;
	case PF_R16G16B16A16_SINT: return VK_FORMAT_R16G16B16A16_SINT;
	case PF_R16G16B16A16_SFLOAT: return VK_FORMAT_R16G16B16A16_SFLOAT;
	case PF_R32_UINT: return VK_FORMAT_R32_UINT;
	case PF_R32_SINT: return VK_FORMAT_R32_SINT;
	case PF_R32_SFLOAT: return VK_FORMAT_R32_SFLOAT;
	case PF_R32G32_UINT: return VK_FORMAT_R32G32_UINT;
	case PF_R32G32_SINT: return VK_FORMAT_R32G32_SINT;
	case PF_R32G32_SFLOAT: return VK_FORMAT_R32G32_SFLOAT;
	case PF_R32G32B32_UINT: return VK_FORMAT_R32G32B32_UINT;
	case PF_R32G32B32_SINT: return VK_FORMAT_R32G32B32_SINT;
	case PF_R32G32B32_SFLOAT: return VK_FORMAT_R32G32B32_SFLOAT;
	case PF_R32G32B32A32_UINT: return VK_FORMAT_R32G32B32A32_UINT;
	case PF_R32G32B32A32_SINT: return VK_FORMAT_R32G32B32A32_SINT;
	case PF_R32G32B32A32_SFLOAT: return VK_FORMAT_R32G32B32A32_SFLOAT;
	case PF_R64_UINT: return VK_FORMAT_R64_UINT;
	case PF_R64_SINT: return VK_FORMAT_R64_SINT;
	case PF_R64_SFLOAT: return VK_FORMAT_R64_SFLOAT;
	case PF_R64G64_UINT: return VK_FORMAT_R64G64_UINT;
	case PF_R64G64_SINT: return VK_FORMAT_R64G64_SINT;
	case PF_R64G64_SFLOAT: return VK_FORMAT_R64G64_SFLOAT;
	case PF_R64G64B64_UINT: return VK_FORMAT_R64G64B64_UINT;
	case PF_R64G64B64_SINT: return VK_FORMAT_R64G64B64_SINT;
	case PF_R64G64B64_SFLOAT: return VK_FORMAT_R64G64B64_SFLOAT;
	case PF_R64G64B64A64_UINT: return VK_FORMAT_R64G64B64A64_UINT;
	case PF_R64G64B64A64_SINT: return VK_FORMAT_R64G64B64A64_SINT;
	case PF_R64G64B64A64_SFLOAT: return VK_FORMAT_R64G64B64A64_SFLOAT;

	case PF_B10G10R10A2_UNORM: return VK_FORMAT_A2R10G10B10_UNORM_PACK32;
	case PF_B10G10R10A2_UINT: return VK_FORMAT_A2R10G10B10_UINT_PACK32;
	case PF_R10G10B10A2_UNORM: return VK_FORMAT_A2B10G10R10_UNORM_PACK32;
	case PF_R10G10B10A2_UINT: return VK_FORMAT_A2B10G10R10_UINT_PACK32;

	case PF_B10G11R11_UFLOAT: return VK_FORMAT_B10G11R11_UFLOAT_PACK32;
	case PF_E5B9G9R9_UFLOAT: return VK_FORMAT_E5B9G9R9_UFLOAT_PACK32;
			
	case PF_G16B16G16R16_422_UNORM: return VK_FORMAT_G16B16G16R16_422_UNORM;
	case PF_B16G16R16G16_422_UNORM: return VK_FORMAT_B16G16R16G16_422_UNORM;
	case PF_R12X4G12X4B12X4A12X4_UNORM_4PACK16: return VK_FORMAT_R12X4G12X4B12X4A12X4_UNORM_4PACK16;
	case PF_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16: return VK_FORMAT_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16;
	case PF_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16: return VK_FORMAT_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16;
	case PF_R10X6G10X6B10X6A10X6_UNORM_4PACK16: return VK_FORMAT_R10X6G10X6B10X6A10X6_UNORM_4PACK16;
	case PF_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16: return VK_FORMAT_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16;
	case PF_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16: return VK_FORMAT_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16;
	case PF_G8B8G8R8_422_UNORM: return VK_FORMAT_G8B8G8R8_422_UNORM;
	case PF_B8G8R8G8_422_UNORM: return VK_FORMAT_B8G8R8G8_422_UNORM;
	case PF_G8_B8_R8_3PLANE_420_UNORM: return VK_FORMAT_G8_B8_R8_3PLANE_420_UNORM;
	case PF_G8_B8R8_2PLANE_420_UNORM: return VK_FORMAT_G8_B8R8_2PLANE_420_UNORM;
	case PF_G8_B8_R8_3PLANE_422_UNORM: return VK_FORMAT_G8_B8_R8_3PLANE_422_UNORM;
	case PF_G8_B8R8_2PLANE_422_UNORM: return VK_FORMAT_G8_B8R8_2PLANE_422_UNORM;
	case PF_G8_B8_R8_3PLANE_444_UNORM: return VK_FORMAT_G8_B8_R8_3PLANE_444_UNORM;
	case PF_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16: return VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16;
	case PF_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16: return VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16;
	case PF_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16: return VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16;
	case PF_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16: return VK_FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16;
	case PF_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16: return VK_FORMAT_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16;
	case PF_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16: return VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16;
	case PF_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16: return VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16;
	case PF_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16: return VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16;
	case PF_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16: return VK_FORMAT_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16;
	case PF_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16: return VK_FORMAT_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16;
	case PF_G16_B16_R16_3PLANE_420_UNORM: return VK_FORMAT_G16_B16_R16_3PLANE_420_UNORM;
	case PF_G16_B16_R16_3PLANE_422_UNORM: return VK_FORMAT_G16_B16_R16_3PLANE_422_UNORM;
	case PF_G16_B16_R16_3PLANE_444_UNORM: return VK_FORMAT_G16_B16_R16_3PLANE_444_UNORM;
	case PF_G16_B16R16_2PLANE_420_UNORM: return VK_FORMAT_G16_B16R16_2PLANE_420_UNORM;
	case PF_G16_B16R16_2PLANE_422_UNORM: return VK_FORMAT_G16_B16R16_2PLANE_422_UNORM;

	case PF_D16_UNORM: return VK_FORMAT_D16_UNORM;
	case PF_X8_D24_UNORM: return VK_FORMAT_X8_D24_UNORM_PACK32;
	case PF_D32_SFLOAT: return VK_FORMAT_D32_SFLOAT;
	case PF_S8_UINT: return VK_FORMAT_S8_UINT;
	case PF_D16_UNORM_S8_UINT: return VK_FORMAT_D16_UNORM_S8_UINT;
	case PF_D24_UNORM_S8_UINT: return VK_FORMAT_D24_UNORM_S8_UINT;
	case PF_D32_SFLOAT_S8_UINT: return VK_FORMAT_D32_SFLOAT_S8_UINT;
	case PF_DXBC1_RGB_UNORM: return VK_FORMAT_BC1_RGB_UNORM_BLOCK;
	case PF_DXBC1_RGB_SRGB: return VK_FORMAT_BC1_RGB_SRGB_BLOCK;
	case PF_DXBC1_RGBA_UNORM: return VK_FORMAT_BC1_RGBA_UNORM_BLOCK;
	case PF_DXBC1_RGBA_SRGB: return VK_FORMAT_BC1_RGBA_SRGB_BLOCK;
	case PF_DXBC2_UNORM: return VK_FORMAT_BC2_UNORM_BLOCK;
	case PF_DXBC2_SRGB: return VK_FORMAT_BC2_SRGB_BLOCK;
	case PF_DXBC3_UNORM: return VK_FORMAT_BC3_UNORM_BLOCK;
	case PF_DXBC3_SRGB: return VK_FORMAT_BC3_SRGB_BLOCK;
	case PF_DXBC4_UNORM: return VK_FORMAT_BC4_UNORM_BLOCK;
	case PF_DXBC4_SNORM: return VK_FORMAT_BC4_SNORM_BLOCK;
	case PF_DXBC5_UNORM: return VK_FORMAT_BC5_UNORM_BLOCK;
	case PF_DXBC5_SNORM: return VK_FORMAT_BC5_SNORM_BLOCK;
	case PF_DXBC6H_UFLOAT: return VK_FORMAT_BC6H_UFLOAT_BLOCK;
	case PF_DXBC6H_SFLOAT: return VK_FORMAT_BC6H_SFLOAT_BLOCK;
	case PF_DXBC7_UNORM: return VK_FORMAT_BC7_UNORM_BLOCK;
	case PF_DXBC7_SRGB: return VK_FORMAT_BC7_SRGB_BLOCK;
	case PF_PVRTC1_2BPP_UNORM: return VK_FORMAT_PVRTC1_2BPP_UNORM_BLOCK_IMG;
	case PF_PVRTC1_4BPP_UNORM: return VK_FORMAT_PVRTC1_4BPP_UNORM_BLOCK_IMG;
	case PF_PVRTC1_2BPP_SRGB: return VK_FORMAT_PVRTC1_2BPP_SRGB_BLOCK_IMG;
	case PF_PVRTC1_4BPP_SRGB: return VK_FORMAT_PVRTC1_4BPP_SRGB_BLOCK_IMG;
	case PF_ETC2_R8G8B8_UNORM: return VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK;
	case PF_ETC2_R8G8B8A1_UNORM: return VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK;
	case PF_ETC2_R8G8B8A8_UNORM: return VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK;
	case PF_ETC2_R8G8B8_SRGB: return VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK;
	case PF_ETC2_R8G8B8A1_SRGB: return VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK;
	case PF_ETC2_R8G8B8A8_SRGB: return VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK;
	case PF_ETC2_EAC_R11_UNORM: return VK_FORMAT_EAC_R11_UNORM_BLOCK;
	case PF_ETC2_EAC_R11G11_UNORM: return VK_FORMAT_EAC_R11G11_UNORM_BLOCK;
	case PF_ETC2_EAC_R11_SNORM: return VK_FORMAT_EAC_R11_SNORM_BLOCK;
	case PF_ETC2_EAC_R11G11_SNORM: return VK_FORMAT_EAC_R11G11_SNORM_BLOCK;
	case PF_ASTC_4x4_UNORM: return VK_FORMAT_ASTC_4x4_UNORM_BLOCK;
	case PF_ASTC_4x4_SRGB: return VK_FORMAT_ASTC_4x4_SRGB_BLOCK;
	case PF_ASTC_5x4_UNORM: return VK_FORMAT_ASTC_5x4_UNORM_BLOCK;
	case PF_ASTC_5x4_SRGB: return VK_FORMAT_ASTC_5x4_SRGB_BLOCK;
	case PF_ASTC_5x5_UNORM: return VK_FORMAT_ASTC_5x5_UNORM_BLOCK;
	case PF_ASTC_5x5_SRGB: return VK_FORMAT_ASTC_5x5_SRGB_BLOCK;
	case PF_ASTC_6x5_UNORM: return VK_FORMAT_ASTC_6x5_UNORM_BLOCK;
	case PF_ASTC_6x5_SRGB: return VK_FORMAT_ASTC_6x5_SRGB_BLOCK;
	case PF_ASTC_6x6_UNORM: return VK_FORMAT_ASTC_6x6_UNORM_BLOCK;
	case PF_ASTC_6x6_SRGB: return VK_FORMAT_ASTC_6x6_SRGB_BLOCK;
	case PF_ASTC_8x5_UNORM: return VK_FORMAT_ASTC_8x5_UNORM_BLOCK;
	case PF_ASTC_8x5_SRGB: return VK_FORMAT_ASTC_8x5_SRGB_BLOCK;
	case PF_ASTC_8x6_UNORM: return VK_FORMAT_ASTC_8x6_UNORM_BLOCK;
	case PF_ASTC_8x6_SRGB: return VK_FORMAT_ASTC_8x6_SRGB_BLOCK;
	case PF_ASTC_8x8_UNORM: return VK_FORMAT_ASTC_8x8_UNORM_BLOCK;
	case PF_ASTC_8x8_SRGB: return VK_FORMAT_ASTC_8x8_SRGB_BLOCK;
	case PF_ASTC_10x5_UNORM: return VK_FORMAT_ASTC_10x5_UNORM_BLOCK;
	case PF_ASTC_10x5_SRGB: return VK_FORMAT_ASTC_10x5_SRGB_BLOCK;
	case PF_ASTC_10x6_UNORM: return VK_FORMAT_ASTC_10x6_UNORM_BLOCK;
	case PF_ASTC_10x6_SRGB: return VK_FORMAT_ASTC_10x6_SRGB_BLOCK;
	case PF_ASTC_10x8_UNORM: return VK_FORMAT_ASTC_10x8_UNORM_BLOCK;
	case PF_ASTC_10x8_SRGB: return VK_FORMAT_ASTC_10x8_SRGB_BLOCK;
	case PF_ASTC_10x10_UNORM: return VK_FORMAT_ASTC_10x10_UNORM_BLOCK;
	case PF_ASTC_10x10_SRGB: return VK_FORMAT_ASTC_10x10_SRGB_BLOCK;
	case PF_ASTC_12x10_UNORM: return VK_FORMAT_ASTC_12x10_UNORM_BLOCK;
	case PF_ASTC_12x10_SRGB: return VK_FORMAT_ASTC_12x10_SRGB_BLOCK;
	case PF_ASTC_12x12_UNORM: return VK_FORMAT_ASTC_12x12_UNORM_BLOCK;
	case PF_ASTC_12x12_SRGB: return VK_FORMAT_ASTC_12x12_SRGB_BLOCK;
	case PF_PVRTC2_2BPP_UNORM: return VK_FORMAT_PVRTC2_2BPP_UNORM_BLOCK_IMG;
	case PF_PVRTC2_4BPP_UNORM: return VK_FORMAT_PVRTC2_4BPP_UNORM_BLOCK_IMG;
	case PF_PVRTC2_2BPP_SRGB: return VK_FORMAT_PVRTC2_2BPP_SRGB_BLOCK_IMG;
	case PF_PVRTC2_4BPP_SRGB: return VK_FORMAT_PVRTC2_4BPP_SRGB_BLOCK_IMG;

	default: return VK_FORMAT_UNDEFINED;
	}
	return VK_FORMAT_UNDEFINED;
}

#undef trans_case
#undef trans_case2