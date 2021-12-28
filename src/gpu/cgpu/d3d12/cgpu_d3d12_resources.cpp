#include "math/common.h"
#include "cgpu/backend/d3d12/cgpu_d3d12.h"
#include "d3d12_utils.h"
#include <dxcapi.h>

// Inline Utils
D3D12_RESOURCE_DESC D3D12Util_CreateBufferDesc(CGpuAdapter_D3D12* A, CGpuDevice_D3D12* D, const struct CGpuBufferDescriptor* desc);
D3D12MA::ALLOCATION_DESC D3D12Util_CreateAllocationDesc(const struct CGpuBufferDescriptor* desc);

// Buffer APIs
cgpu_static_assert(sizeof(CGpuBuffer_D3D12) <= 8 * sizeof(uint64_t), "Acquire Single CacheLine"); // Cache Line
CGpuBufferId cgpu_create_buffer_d3d12(CGpuDeviceId device, const struct CGpuBufferDescriptor* desc)
{
    CGpuBuffer_D3D12* B = new CGpuBuffer_D3D12();
    CGpuDevice_D3D12* D = (CGpuDevice_D3D12*)device;
    CGpuAdapter_D3D12* A = (CGpuAdapter_D3D12*)device->adapter;
    D3D12_RESOURCE_DESC bufDesc = D3D12Util_CreateBufferDesc(A, D, desc);
    uint64_t allocationSize = bufDesc.Width;
    // Handle Resource Start State
    ECGpuResourceState start_state = desc->start_state;
    if (desc->memory_usage == MEM_USAGE_CPU_TO_GPU || desc->memory_usage == MEM_USAGE_CPU_ONLY)
    {
        start_state = RESOURCE_STATE_GENERIC_READ;
    }
    else if (desc->memory_usage == MEM_USAGE_GPU_TO_CPU)
    {
        start_state = RESOURCE_STATE_COPY_DEST;
    }
    D3D12_RESOURCE_STATES res_states = D3D12Util_TranslateResourceState(start_state);

    // Do Allocation
    D3D12MA::ALLOCATION_DESC alloc_desc = D3D12Util_CreateAllocationDesc(desc);
    if (D3D12_HEAP_TYPE_DEFAULT != alloc_desc.HeapType &&
        (alloc_desc.Flags & D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS))
    {
        D3D12_HEAP_PROPERTIES heapProps = {};
        heapProps.Type = D3D12_HEAP_TYPE_CUSTOM;
        heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_COMBINE;
        heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
        heapProps.VisibleNodeMask = SINGLE_GPU_NODE_MASK;
        heapProps.CreationNodeMask = SINGLE_GPU_NODE_MASK;
        CHECK_HRESULT(D->pDxDevice->CreateCommittedResource(
            &heapProps, alloc_desc.ExtraHeapFlags, &bufDesc, res_states, NULL, IID_ARGS(&B->pDxResource)));
    }
    else
    {
        CHECK_HRESULT(D->pResourceAllocator->CreateResource(
            &alloc_desc, &bufDesc, res_states, NULL, &B->pDxAllocation, IID_ARGS(&B->pDxResource)));
    }

    // MemMaps
    if (desc->memory_usage != MEM_USAGE_GPU_ONLY && desc->flags & BCF_PERSISTENT_MAP_BIT)
        B->pDxResource->Map(0, NULL, &B->super.cpu_mapped_address);
    B->mDxGpuAddress = B->pDxResource->GetGPUVirtualAddress();
#if defined(XBOX)
    B->super.cpu_mapped_address->pCpuMappedAddress = (void*)B->mDxGpuAddress;
#endif

    // Create Descriptors
    if (!(desc->flags & BCF_NO_DESCRIPTOR_VIEW_CREATION))
    {
        D3D12Util_DescriptorHeap* pHeap = D->pCPUDescriptorHeaps[D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV];
        uint32_t handleCount = ((desc->descriptors & RT_UNIFORM_BUFFER) ? 1 : 0) +
                               ((desc->descriptors & RT_BUFFER) ? 1 : 0) +
                               ((desc->descriptors & RT_RW_BUFFER) ? 1 : 0);
        B->mDxDescriptorHandles = D3D12Util_ConsumeDescriptorHandles(pHeap, handleCount).mCpu;
        // Create CBV
        if (desc->descriptors & RT_UNIFORM_BUFFER)
        {
            D3D12_CPU_DESCRIPTOR_HANDLE cbv = { B->mDxDescriptorHandles.ptr };
            B->mDxSrvOffset = pHeap->mDescriptorSize * 1;

            D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
            cbvDesc.BufferLocation = B->mDxGpuAddress;
            cbvDesc.SizeInBytes = (UINT)allocationSize;
            D3D12Util_CreateCBV(D, &cbvDesc, &cbv);
        }
        // Create SRV
        if (desc->descriptors & RT_BUFFER)
        {
            D3D12_CPU_DESCRIPTOR_HANDLE srv = { B->mDxDescriptorHandles.ptr + B->mDxSrvOffset };
            B->mDxUavOffset = B->mDxSrvOffset + pHeap->mDescriptorSize * 1;

            D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
            srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
            srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
            srvDesc.Buffer.FirstElement = desc->first_element;
            srvDesc.Buffer.NumElements = (UINT)(desc->elemet_count);
            srvDesc.Buffer.StructureByteStride = (UINT)(desc->element_stride);
            srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
            srvDesc.Format = (DXGI_FORMAT)DXGIUtil_TranslatePixelFormat(desc->format);
            if (RT_BUFFER_RAW == (desc->descriptors & RT_BUFFER_RAW))
            {
                if (desc->format != PF_UNDEFINED)
                    cgpu_warn("Raw buffers use R32 typeless format. Format will be ignored");
                srvDesc.Format = DXGI_FORMAT_R32_TYPELESS;
                srvDesc.Buffer.Flags |= D3D12_BUFFER_SRV_FLAG_RAW;
            }
            // Cannot create a typed StructuredBuffer
            if (srvDesc.Format != DXGI_FORMAT_UNKNOWN)
            {
                srvDesc.Buffer.StructureByteStride = 0;
            }
            D3D12Util_CreateSRV(D, B->pDxResource, &srvDesc, &srv);
        }
        // Create UAV
        if (desc->descriptors & RT_RW_BUFFER)
        {
            D3D12_CPU_DESCRIPTOR_HANDLE uav = { B->mDxDescriptorHandles.ptr + B->mDxUavOffset };

            D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
            uavDesc.Format = DXGI_FORMAT_UNKNOWN;
            uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
            uavDesc.Buffer.FirstElement = desc->first_element;
            uavDesc.Buffer.NumElements = (UINT)(desc->elemet_count);
            uavDesc.Buffer.StructureByteStride = (UINT)(desc->element_stride);
            uavDesc.Buffer.CounterOffsetInBytes = 0;
            uavDesc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
            if (RT_RW_BUFFER_RAW == (desc->descriptors & RT_RW_BUFFER_RAW))
            {
                if (desc->format != PF_UNDEFINED)
                    cgpu_warn("Raw buffers use R32 typeless format. Format will be ignored");
                uavDesc.Format = DXGI_FORMAT_R32_TYPELESS;
                uavDesc.Buffer.Flags |= D3D12_BUFFER_UAV_FLAG_RAW;
            }
            else if (desc->format != PF_UNDEFINED)
            {
                uavDesc.Format = (DXGI_FORMAT)DXGIUtil_TranslatePixelFormat(desc->format);
                D3D12_FEATURE_DATA_FORMAT_SUPPORT FormatSupport = { uavDesc.Format, D3D12_FORMAT_SUPPORT1_NONE,
                    D3D12_FORMAT_SUPPORT2_NONE };
                HRESULT hr =
                    D->pDxDevice->CheckFeatureSupport(D3D12_FEATURE_FORMAT_SUPPORT, &FormatSupport, sizeof(FormatSupport));
                if (!SUCCEEDED(hr) || !(FormatSupport.Support2 & D3D12_FORMAT_SUPPORT2_UAV_TYPED_LOAD) ||
                    !(FormatSupport.Support2 & D3D12_FORMAT_SUPPORT2_UAV_TYPED_STORE))
                {
                    // Format does not support UAV Typed Load
                    cgpu_warn("Cannot use Typed UAV for buffer format %u", (uint32_t)desc->format);
                    uavDesc.Format = DXGI_FORMAT_UNKNOWN;
                }
            }
            // Cannot create a typed RWStructuredBuffer
            if (uavDesc.Format != DXGI_FORMAT_UNKNOWN)
            {
                uavDesc.Buffer.StructureByteStride = 0;
            }
            CGpuBuffer_D3D12* pCountBuffer = (CGpuBuffer_D3D12*)desc->count_buffer;
            ID3D12Resource* pCounterResource = pCountBuffer ? pCountBuffer->pDxResource : NULL;
            D3D12Util_CreateUAV(D, B->pDxResource, pCounterResource, &uavDesc, &uav);
        }
    }
    // Set Debug Name
    if (device->adapter->instance->enable_set_name && desc->name)
    {
        wchar_t debugName[MAX_GPU_DEBUG_NAME_LENGTH] = {};
        mbstowcs(debugName, desc->name, MAX_GPU_DEBUG_NAME_LENGTH);
        if (B->pDxAllocation)
        {
            B->pDxAllocation->SetName(debugName);
        }
        B->pDxResource->SetName(debugName);
    }
    // Set Buffer Object Props
    B->super.size = (uint32_t)desc->size;
    B->super.memory_usage = desc->memory_usage;
    B->super.descriptors = desc->descriptors;
    return &B->super;
}

void cgpu_map_buffer_d3d12(CGpuBufferId buffer, const struct CGpuBufferRange* range)
{
    CGpuBuffer_D3D12* B = (CGpuBuffer_D3D12*)buffer;
    cgpu_assert(B->super.memory_usage != MEM_USAGE_GPU_ONLY && "Trying to map non-cpu accessible resource");

    D3D12_RANGE dxrange = { 0, B->super.size };
    if (range)
    {
        dxrange.Begin += range->offset;
        dxrange.End = dxrange.Begin + range->size;
    }
    CHECK_HRESULT(B->pDxResource->Map(0, &dxrange, &B->super.cpu_mapped_address));
}

void cgpu_unmap_buffer_d3d12(CGpuBufferId buffer)
{
    CGpuBuffer_D3D12* B = (CGpuBuffer_D3D12*)buffer;
    cgpu_assert(B->super.memory_usage != MEM_USAGE_GPU_ONLY && "Trying to unmap non-cpu accessible resource");

    B->pDxResource->Unmap(0, NULL);
    B->super.cpu_mapped_address = NULL;
}

void cgpu_cmd_transfer_buffer_to_buffer_d3d12(CGpuCommandBufferId cmd, const struct CGpuBufferToBufferTransfer* desc)
{
    CGpuCommandBuffer_D3D12* Cmd = (CGpuCommandBuffer_D3D12*)cmd;
    CGpuBuffer_D3D12* Src = (CGpuBuffer_D3D12*)desc->src;
    CGpuBuffer_D3D12* Dst = (CGpuBuffer_D3D12*)desc->dst;
#if defined(XBOX)
    Cmd->mDma.pDxCmdList->CopyBufferRegion(Dst->pDxResource, desc->dst_offset, Src->pDxResource, desc->src_offset, desc->size);
#else
    Cmd->pDxCmdList->CopyBufferRegion(Dst->pDxResource, desc->dst_offset, Src->pDxResource, desc->src_offset, desc->size);
#endif
}

void cgpu_free_buffer_d3d12(CGpuBufferId buffer)
{
    CGpuBuffer_D3D12* B = (CGpuBuffer_D3D12*)buffer;
    CGpuDevice_D3D12* D = (CGpuDevice_D3D12*)B->super.device;
    if (B->mDxDescriptorHandles.ptr != D3D12_GPU_VIRTUAL_ADDRESS_NULL)
    {
        uint32_t handleCount = ((B->super.descriptors & RT_UNIFORM_BUFFER) ? 1 : 0) +
                               ((B->super.descriptors & RT_BUFFER) ? 1 : 0) +
                               ((B->super.descriptors & RT_RW_BUFFER) ? 1 : 0);
        D3D12Util_ReturnDescriptorHandles(
            D->pCPUDescriptorHeaps[D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV], B->mDxDescriptorHandles,
            handleCount);
    }
    SAFE_RELEASE(B->pDxAllocation)
    SAFE_RELEASE(B->pDxResource)
    delete B;
}

// Texture/TextureView APIs
CGpuTextureViewId cgpu_create_texture_view_d3d12(CGpuDeviceId device, const struct CGpuTextureViewDescriptor* desc)
{
    CGpuTextureView_D3D12* TV = cgpu_new<CGpuTextureView_D3D12>();
    CGpuTexture_D3D12* T = (CGpuTexture_D3D12*)desc->texture;
    CGpuDevice_D3D12* D = (CGpuDevice_D3D12*)device;
    // Consume handles
    const auto usages = desc->usages;
    uint32_t handleCount = ((usages & TVU_SRV) ? 1 : 0) +
                           ((usages & TVU_UAV) ? 1 : 0);
    if (handleCount > 0)
    {
        D3D12Util_DescriptorHeap* pHeap = D->pCPUDescriptorHeaps[D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV];
        TV->mDxDescriptorHandles = D3D12Util_ConsumeDescriptorHandles(pHeap, handleCount).mCpu;
        TV->mDxSrvOffset = 0;
        uint64_t CurrentOffsetCursor = TV->mDxSrvOffset + pHeap->mDescriptorSize * 1;
        // Create SRV
        if (usages & TVU_SRV)
        {
            D3D12_CPU_DESCRIPTOR_HANDLE srv = { TV->mDxDescriptorHandles.ptr + TV->mDxSrvOffset };
            D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
            srvDesc.Format = (DXGI_FORMAT)DXGIUtil_TranslatePixelFormat(desc->format);
            srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
            switch (desc->dims)
            {
                case TEX_DIMENSION_1D: {
                    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE1D;
                    srvDesc.Texture1D.MipLevels = desc->mip_level_count;
                    srvDesc.Texture1D.MostDetailedMip = desc->base_mip_level;
                }
                break;
                case TEX_DIMENSION_1D_ARRAY: {
                    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE1DARRAY;
                    srvDesc.Texture1DArray.MipLevels = desc->mip_level_count;
                    srvDesc.Texture1DArray.MostDetailedMip = desc->base_mip_level;
                    srvDesc.Texture1DArray.FirstArraySlice = desc->base_array_layer;
                    srvDesc.Texture1DArray.ArraySize = desc->array_layer_count;
                }
                break;
                case TEX_DIMENSION_2DMS: {
                    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DMS;
                }
                break;
                case TEX_DIMENSION_2D: {
                    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
                    srvDesc.Texture2D.MipLevels = desc->mip_level_count;
                    srvDesc.Texture2D.MostDetailedMip = desc->base_mip_level;
                    srvDesc.Texture2D.PlaneSlice = 0;
                }
                break;
                case TEX_DIMENSION_2DMS_ARRAY: {
                    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DMSARRAY;
                    srvDesc.Texture2DMSArray.ArraySize = desc->array_layer_count;
                    srvDesc.Texture2DMSArray.FirstArraySlice = desc->base_array_layer;
                }
                break;
                case TEX_DIMENSION_2D_ARRAY: {
                    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2DARRAY;
                    srvDesc.Texture2DArray.MipLevels = desc->mip_level_count;
                    srvDesc.Texture2DArray.MostDetailedMip = desc->base_mip_level;
                    srvDesc.Texture2DArray.PlaneSlice = 0;
                    srvDesc.Texture2DArray.FirstArraySlice = desc->base_array_layer;
                    srvDesc.Texture2DArray.ArraySize = desc->array_layer_count;
                }
                break;
                case TEX_DIMENSION_3D: {
                    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE3D;
                    srvDesc.Texture3D.MipLevels = desc->mip_level_count;
                    srvDesc.Texture3D.MostDetailedMip = desc->base_mip_level;
                }
                break;
                case TEX_DIMENSION_CUBE: {
                    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
                    srvDesc.TextureCube.MipLevels = desc->mip_level_count;
                    srvDesc.TextureCube.MostDetailedMip = desc->base_mip_level;
                }
                break;
                case TEX_DIMENSION_CUBE_ARRAY: {
                    srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBEARRAY;
                    srvDesc.TextureCubeArray.MipLevels = desc->mip_level_count;
                    srvDesc.TextureCubeArray.MostDetailedMip = desc->base_mip_level;
                    srvDesc.TextureCubeArray.NumCubes = desc->array_layer_count;
                    srvDesc.TextureCubeArray.First2DArrayFace = desc->array_layer_count;
                }
                break;
                default:
                    cgpu_assert(0 && "Unsupported texture dimension!");
                    break;
            }
            D3D12Util_CreateSRV(D, T->pDxResource, &srvDesc, &srv);
        }
        // Create UAV
        if (usages & TVU_UAV)
        {
            TV->mDxUavOffset = CurrentOffsetCursor;
            CurrentOffsetCursor += pHeap->mDescriptorSize * 1;
            D3D12_CPU_DESCRIPTOR_HANDLE uav = { TV->mDxDescriptorHandles.ptr + TV->mDxUavOffset };
            D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
            uavDesc.Format = (DXGI_FORMAT)DXGIUtil_TranslatePixelFormat(desc->format);
            cgpu_assert(desc->mip_level_count <= 1 && "UAV must be created with non-multi mip slices!");
            switch (desc->dims)
            {
                case TEX_DIMENSION_1D: {
                    uavDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE1D;
                    uavDesc.Texture1D.MipSlice = desc->base_mip_level;
                }
                break;
                case TEX_DIMENSION_1D_ARRAY: {
                    uavDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE1DARRAY;
                    uavDesc.Texture1DArray.MipSlice = desc->base_mip_level;
                    uavDesc.Texture1DArray.FirstArraySlice = desc->base_array_layer;
                    uavDesc.Texture1DArray.ArraySize = desc->array_layer_count;
                }
                break;
                case TEX_DIMENSION_2D: {
                    uavDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
                    uavDesc.Texture2D.MipSlice = desc->base_mip_level;
                    uavDesc.Texture2D.PlaneSlice = 0;
                }
                break;
                case TEX_DIMENSION_2D_ARRAY: {
                    uavDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2DARRAY;
                    uavDesc.Texture2DArray.MipSlice = desc->base_mip_level;
                    uavDesc.Texture2DArray.PlaneSlice = 0;
                    uavDesc.Texture2DArray.FirstArraySlice = desc->base_array_layer;
                    uavDesc.Texture2DArray.ArraySize = desc->array_layer_count;
                }
                break;
                case TEX_DIMENSION_3D: {
                    uavDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE3D;
                    uavDesc.Texture3D.MipSlice = desc->base_mip_level;
                    uavDesc.Texture3D.FirstWSlice = desc->base_array_layer;
                    uavDesc.Texture3D.WSize = desc->array_layer_count;
                }
                break;
                default:
                    cgpu_assert(0 && "Unsupported texture dimension!");
                    break;
            }
            D3D12Util_CreateUAV(D, T->pDxResource, CGPU_NULLPTR, &uavDesc, &uav);
        }
    }
    // Create RTV
    if (usages & TVU_RTV)
    {
        D3D12Util_DescriptorHeap* pRtvHeap = D->pCPUDescriptorHeaps[D3D12_DESCRIPTOR_HEAP_TYPE_RTV];
        TV->mDxRtxDescriptorHandle = D3D12Util_ConsumeDescriptorHandles(pRtvHeap, 1).mCpu;
        D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
        rtvDesc.Format = (DXGI_FORMAT)DXGIUtil_TranslatePixelFormat(desc->format);
        switch (desc->dims)
        {
            case TEX_DIMENSION_1D: {
                rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE1D;
                rtvDesc.Texture1D.MipSlice = desc->base_mip_level;
            }
            break;
            case TEX_DIMENSION_1D_ARRAY: {
                rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE1DARRAY;
                rtvDesc.Texture1DArray.MipSlice = desc->base_mip_level;
                rtvDesc.Texture1DArray.FirstArraySlice = desc->base_array_layer;
                rtvDesc.Texture1DArray.ArraySize = desc->array_layer_count;
            }
            break;
            case TEX_DIMENSION_2DMS: {
                rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DMS;
            }
            break;
            case TEX_DIMENSION_2D: {
                rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
                rtvDesc.Texture2D.MipSlice = desc->base_mip_level;
                rtvDesc.Texture2D.PlaneSlice = 0;
            }
            break;
            case TEX_DIMENSION_2DMS_ARRAY: {
                rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DMSARRAY;
                rtvDesc.Texture2DMSArray.FirstArraySlice = desc->base_array_layer;
                rtvDesc.Texture2DMSArray.ArraySize = desc->array_layer_count;
            }
            case TEX_DIMENSION_2D_ARRAY: {
                rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DARRAY;
                rtvDesc.Texture2DArray.MipSlice = desc->base_mip_level;
                rtvDesc.Texture2DArray.PlaneSlice = 0;
                rtvDesc.Texture2DArray.FirstArraySlice = desc->base_array_layer;
                rtvDesc.Texture2DArray.ArraySize = desc->array_layer_count;
            }
            break;
            case TEX_DIMENSION_3D: {
                rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE3D;
                rtvDesc.Texture3D.MipSlice = desc->base_mip_level;
                rtvDesc.Texture3D.FirstWSlice = desc->base_array_layer;
                rtvDesc.Texture3D.WSize = desc->array_layer_count;
            }
            break;
            default:
                cgpu_assert(0 && "Unsupported texture dimension!");
                break;
        }
        D3D12Util_CreateRTV(D, T->pDxResource, &rtvDesc, &TV->mDxRtxDescriptorHandle);
    }
    return &TV->super;
}

void cgpu_free_texture_view_d3d12(CGpuTextureViewId view)
{
    CGpuTextureView_D3D12* TV = (CGpuTextureView_D3D12*)view;
    CGpuDevice_D3D12* D = (CGpuDevice_D3D12*)view->device;
    if (TV->mDxDescriptorHandles.ptr != D3D12_GPU_VIRTUAL_ADDRESS_NULL)
    {
        const auto usages = TV->super.info.usages;
        uint32_t handleCount = ((usages & TVU_SRV) ? 1 : 0) +
                               ((usages & TVU_UAV) ? 1 : 0) +
                               ((usages & TVU_RTV) ? 1 : 0);
        D3D12Util_ReturnDescriptorHandles(
            D->pCPUDescriptorHeaps[D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV], TV->mDxDescriptorHandles,
            handleCount);
    }
}

// Shader APIs
#ifndef DXC_CP_ACP
    #define DXC_CP_ACP 0
#endif

template <typename T, typename... Args>
auto try_invoke_pinned_api(T* loader, Args&&... args)
    -> decltype(loader->CreateBlobWithEncodingFromPinned(std::forward<Args>(args)...), bool())
{
    loader->CreateBlobWithEncodingFromPinned(std::forward<Args>(args)...);
    return true;
};
template <typename T>
bool try_invoke_pinned_api(T* loader, ...) { return false; }

struct DxilMinimalHeader {
    UINT32 four_cc;
    UINT32 hash_digest[4];
};

inline bool is_dxil_signed(const void* buffer)
{
    const DxilMinimalHeader* header = reinterpret_cast<const DxilMinimalHeader*>(buffer);
    bool has_digest = false;
    has_digest |= header->hash_digest[0] != 0x0;
    has_digest |= header->hash_digest[1] != 0x0;
    has_digest |= header->hash_digest[2] != 0x0;
    has_digest |= header->hash_digest[3] != 0x0;
    return has_digest;
}

CGpuShaderLibraryId cgpu_create_shader_library_d3d12(
    CGpuDeviceId device, const struct CGpuShaderLibraryDescriptor* desc)
{
    CGpuDevice_D3D12* D = (CGpuDevice_D3D12*)device;
    CGpuShaderLibrary_D3D12* S = new CGpuShaderLibrary_D3D12();
    IDxcLibrary* pUtils;
    DxcCreateInstance(CLSID_DxcLibrary, IID_PPV_ARGS(&pUtils));
    if (!try_invoke_pinned_api(pUtils, desc->code, (uint32_t)desc->code_size, DXC_CP_ACP, &S->pShaderBlob))
    {
        pUtils->CreateBlobWithEncodingOnHeapCopy(desc->code, (uint32_t)desc->code_size, DXC_CP_ACP, &S->pShaderBlob);
    }
    // Validate & Signing
    // if (!is_dxil_signed(desc->code)) cgpu_assert(0 && "The dxil shader is not signed!");
    // Reflection
    D3D12Util_InitializeShaderReflection(D, S, desc);
    pUtils->Release();
    return &S->super;
}

void cgpu_free_shader_library_d3d12(CGpuShaderLibraryId shader_library)
{
    CGpuShaderLibrary_D3D12* S = (CGpuShaderLibrary_D3D12*)shader_library;
    if (S->pShaderBlob != CGPU_NULLPTR)
    {
        S->pShaderBlob->Release();
    }
    delete S;
}

// Util Implementations
inline D3D12_RESOURCE_DESC D3D12Util_CreateBufferDesc(
    CGpuAdapter_D3D12* A, CGpuDevice_D3D12* D, const struct CGpuBufferDescriptor* desc)
{
    DECLARE_ZERO(D3D12_RESOURCE_DESC, bufDesc);
    uint64_t allocationSize = desc->size;
    // Align the buffer size to multiples of the dynamic uniform buffer minimum size
    if (desc->descriptors & RT_UNIFORM_BUFFER)
    {
        uint64_t minAlignment = A->adapter_detail.uniform_buffer_alignment;
        allocationSize = smath_round_up_64(allocationSize, minAlignment);
    }
    bufDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    // Alignment must be 64KB (D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT) or 0, which is effectively 64KB.
    // https://msdn.microsoft.com/en-us/library/windows/desktop/dn903813(v=vs.85).aspx
    bufDesc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
    bufDesc.Width = allocationSize;
    bufDesc.Height = 1;
    bufDesc.DepthOrArraySize = 1;
    bufDesc.MipLevels = 1;
    bufDesc.Format = DXGI_FORMAT_UNKNOWN;
    bufDesc.SampleDesc.Count = 1;
    bufDesc.SampleDesc.Quality = 0;
    bufDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    bufDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
    if (desc->descriptors & RT_RW_BUFFER)
    {
        bufDesc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
    }
    // Adjust for padding
    UINT64 padded_size = 0;
    D->pDxDevice->GetCopyableFootprints(&bufDesc, 0, 1, 0, NULL, NULL, NULL, &padded_size);
    allocationSize = (uint64_t)padded_size;
    bufDesc.Width = allocationSize;
    // Mark DENY_SHADER_RESOURCE
    if (desc->memory_usage == MEM_USAGE_GPU_TO_CPU)
    {
        bufDesc.Flags |= D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE;
    }
    return bufDesc;
}

inline D3D12MA::ALLOCATION_DESC D3D12Util_CreateAllocationDesc(const struct CGpuBufferDescriptor* desc)
{
    // Alloc Info
    DECLARE_ZERO(D3D12MA::ALLOCATION_DESC, alloc_desc)
    if (desc->memory_usage == MEM_USAGE_CPU_ONLY || desc->memory_usage == MEM_USAGE_CPU_TO_GPU)
        alloc_desc.HeapType = D3D12_HEAP_TYPE_UPLOAD;
    else if (desc->memory_usage == MEM_USAGE_GPU_TO_CPU)
        alloc_desc.HeapType = D3D12_HEAP_TYPE_READBACK;
    else
        alloc_desc.HeapType = D3D12_HEAP_TYPE_DEFAULT;

    if (desc->flags & BCF_OWN_MEMORY_BIT)
        alloc_desc.Flags |= D3D12MA::ALLOCATION_FLAG_COMMITTED;
    return alloc_desc;
}

// Descriptor Heap
D3D12Util_DescriptorHandle D3D12Util_ConsumeDescriptorHandles(D3D12Util_DescriptorHeap* pHeap, uint32_t descriptorCount)
{
    if (pHeap->mUsedDescriptors + descriptorCount > pHeap->mDesc.NumDescriptors)
    {
#ifdef CGPU_THREAD_SAFETY
        SMutexLock lock(*pHeap->pMutex);
#endif
        if ((pHeap->mDesc.Flags & D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE))
        {
            uint32_t currentOffset = pHeap->mUsedDescriptors;
            (void)currentOffset;
            D3D12_DESCRIPTOR_HEAP_DESC desc = pHeap->mDesc;
            while (pHeap->mUsedDescriptors + descriptorCount > desc.NumDescriptors)
            {
                desc.NumDescriptors <<= 1;
            }
            ID3D12Device* pDevice = pHeap->pDevice;
            SAFE_RELEASE(pHeap->pCurrentHeap);
            pDevice->CreateDescriptorHeap(&desc, IID_ARGS(&pHeap->pCurrentHeap));
            pHeap->mDesc = desc;
            pHeap->mStartHandle.mCpu = pHeap->pCurrentHeap->GetCPUDescriptorHandleForHeapStart();
            pHeap->mStartHandle.mGpu = pHeap->pCurrentHeap->GetGPUDescriptorHandleForHeapStart();

            uint32_t* rangeSizes = (uint32_t*)alloca(pHeap->mUsedDescriptors * sizeof(uint32_t));
#ifdef CGPU_THREAD_SAFETY
            uint32_t usedDescriptors = skr_atomic32_load_relaxed(&pHeap->mUsedDescriptors);
#else
            uint32_t usedDescriptors = pHeap->mUsedDescriptors;
#endif
            for (uint32_t i = 0; i < pHeap->mUsedDescriptors; ++i)
                rangeSizes[i] = 1;
            pDevice->CopyDescriptors(
                1, &pHeap->mStartHandle.mCpu, &usedDescriptors, pHeap->mUsedDescriptors, pHeap->pHandles, rangeSizes, pHeap->mDesc.Type);
            D3D12_CPU_DESCRIPTOR_HANDLE* pNewHandles =
                (D3D12_CPU_DESCRIPTOR_HANDLE*)cgpu_calloc(pHeap->mDesc.NumDescriptors, sizeof(D3D12_CPU_DESCRIPTOR_HANDLE));
            memcpy(pNewHandles, pHeap->pHandles, pHeap->mUsedDescriptors * sizeof(D3D12_CPU_DESCRIPTOR_HANDLE));
            cgpu_free(pHeap->pHandles);
            pHeap->pHandles = pNewHandles;
        }
        else if (pHeap->mFreeList.size() >= descriptorCount)
        {
            if (descriptorCount == 1)
            {
                D3D12Util_DescriptorHandle ret = pHeap->mFreeList.back();
                pHeap->mFreeList.pop_back();
                return ret;
            }

            // search for continuous free items in the list
            uint32_t freeCount = 1;
            for (size_t i = pHeap->mFreeList.size() - 1; i > 0; --i)
            {
                size_t index = i - 1;
                D3D12Util_DescriptorHandle mDescHandle = pHeap->mFreeList[index];
                if (mDescHandle.mCpu.ptr + pHeap->mDescriptorSize == pHeap->mFreeList[i].mCpu.ptr)
                    ++freeCount;
                else
                    freeCount = 1;

                if (freeCount == descriptorCount)
                {
                    pHeap->mFreeList.erase(pHeap->mFreeList.begin() + index, pHeap->mFreeList.begin() + index + descriptorCount);
                    return mDescHandle;
                }
            }
        }
    }
#ifdef CGPU_THREAD_SAFETY
    uint32_t usedDescriptors = skr_atomic32_add_relaxed(&pHeap->mUsedDescriptors, descriptorCount);
#else
    uint32_t usedDescriptors = pHeap->mUsedDescriptors = pHeap->mUsedDescriptors + descriptorCount;
#endif
    cgpu_assert(usedDescriptors + descriptorCount <= pHeap->mDesc.NumDescriptors);
    D3D12Util_DescriptorHandle ret = {
        { pHeap->mStartHandle.mCpu.ptr + usedDescriptors * pHeap->mDescriptorSize },
        { pHeap->mStartHandle.mGpu.ptr + usedDescriptors * pHeap->mDescriptorSize },
    };
    return ret;
}

void D3D12Util_CreateDescriptorHeap(ID3D12Device* pDevice,
    const D3D12_DESCRIPTOR_HEAP_DESC* pDesc, struct D3D12Util_DescriptorHeap** ppDescHeap)
{
    uint32_t numDescriptors = pDesc->NumDescriptors;
    D3D12Util_DescriptorHeap* pHeap = (D3D12Util_DescriptorHeap*)cgpu_calloc(1, sizeof(*pHeap));
#ifdef CGPU_THREAD_SAFETY
    pHeap->pMutex = (SMutex*)cgpu_calloc(1, sizeof(SMutex));
    skr_init_mutex(pHeap->pMutex);
#endif
    pHeap->pDevice = pDevice;

    // Keep 32 aligned for easy remove
    numDescriptors = smath_round_up(numDescriptors, 32);

    D3D12_DESCRIPTOR_HEAP_DESC Desc = *pDesc;
    Desc.NumDescriptors = numDescriptors;
    pHeap->mDesc = Desc;

    CHECK_HRESULT(pDevice->CreateDescriptorHeap(&Desc, IID_ARGS(&pHeap->pCurrentHeap)));

    pHeap->mStartHandle.mCpu = pHeap->pCurrentHeap->GetCPUDescriptorHandleForHeapStart();
    if (pHeap->mDesc.Flags & D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE)
    {
        pHeap->mStartHandle.mGpu = pHeap->pCurrentHeap->GetGPUDescriptorHandleForHeapStart();
    }
    pHeap->mDescriptorSize = pDevice->GetDescriptorHandleIncrementSize(pHeap->mDesc.Type);
    if (Desc.Flags & D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE)
        pHeap->pHandles = (D3D12_CPU_DESCRIPTOR_HANDLE*)cgpu_calloc(Desc.NumDescriptors, sizeof(D3D12_CPU_DESCRIPTOR_HANDLE));

    *ppDescHeap = pHeap;
}

void D3D12Util_ReturnDescriptorHandles(
    struct D3D12Util_DescriptorHeap* pHeap, D3D12_CPU_DESCRIPTOR_HANDLE handle, uint32_t count)
{
    cgpu_assert((pHeap->mDesc.Flags & D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE) == 0);
#ifdef CGPU_THREAD_SAFETY
    SMutexLock lock(*pHeap->pMutex);
#endif
    for (uint32_t i = 0; i < count; ++i)
    {
        DECLARE_ZERO(D3D12Util_DescriptorHandle, Free)
        Free.mCpu = { handle.ptr + pHeap->mDescriptorSize * i };
        Free.mGpu = { D3D12_GPU_VIRTUAL_ADDRESS_NULL };
        pHeap->mFreeList.push_back(Free);
    }
}

void D3D12Util_ResetDescriptorHeap(struct D3D12Util_DescriptorHeap* pHeap)
{
    pHeap->mUsedDescriptors = 0;
    pHeap->mFreeList.clear();
}

static void D3D12Util_FreeDescriptorHeap(D3D12Util_DescriptorHeap* pHeap)
{
    if (pHeap == nullptr) return;
    SAFE_RELEASE(pHeap->pCurrentHeap);

// Need delete since object frees allocated memory in destructor
#ifdef CGPU_THREAD_SAFETY
    skr_destroy_mutex(pHeap->pMutex);
    cgpu_free(pHeap->pMutex);
#endif

    pHeap->mFreeList.~vector();

    cgpu_free(pHeap->pHandles);
    cgpu_free(pHeap);
}