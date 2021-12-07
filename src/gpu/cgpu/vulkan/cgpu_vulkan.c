#include "vulkan_utils.h"

const CGpuProcTable tbl_vk = {
    // Instance APIs
    .create_instance = &cgpu_create_instance_vulkan,
    .query_instance_features = &cgpu_query_instance_features_vulkan,
    .free_instance = &cgpu_free_instance_vulkan,

    // Adapter APIs
    .enum_adapters = &cgpu_enum_adapters_vulkan,
    .query_adapter_detail = &cgpu_query_adapter_detail_vulkan,
    .query_queue_count = &cgpu_query_queue_count_vulkan,

    // Device APIs
    .create_device = &cgpu_create_device_vulkan,
    .free_device = &cgpu_free_device_vulkan,

    // API Object APIs
    .create_fence = &cgpu_create_fence_vulkan,
    .free_fence = &cgpu_free_fence_vulkan,
    .create_root_signature = &cgpu_create_root_signature_vulkan,
    .free_root_signature = &cgpu_free_root_signature_vulkan,

    // Queue APIs
    .get_queue = &cgpu_get_queue_vulkan,
    .submit_queue = &cgpu_submit_queue_vulkan,
    .wait_queue_idle = &cgpu_wait_queue_idle_vulkan,
    .free_queue = &cgpu_free_queue_vulkan,

    // Command APIs
    .create_command_pool = &cgpu_create_command_pool_vulkan,
    .create_command_buffer = &cgpu_create_command_buffer_vulkan,
    .free_command_buffer = &cgpu_free_command_buffer_vulkan,
    .free_command_pool = &cgpu_free_command_pool_vulkan,

    // Shader APIs
    .create_shader_library = &cgpu_create_shader_library_vulkan,
    .free_shader_library = &cgpu_free_shader_library_vulkan,

    // Buffer APIs
    .create_buffer = &cgpu_create_buffer_vulkan,
    .map_buffer = &cgpu_map_buffer_vulkan,
    .unmap_buffer = &cgpu_unmap_buffer_vulkan,
    .free_buffer = &cgpu_free_buffer_vulkan,

    // Swapchain APIs
    .create_swapchain = &cgpu_create_swapchain_vulkan,
    .free_swapchain = &cgpu_free_swapchain_vulkan,

    // CMDs
    .cmd_begin = &cgpu_cmd_begin_vulkan,
    .cmd_update_buffer = &cgpu_cmd_update_buffer_vulkan,
    .cmd_end = &cgpu_cmd_end_vulkan
};

const CGpuProcTable* CGPU_VulkanProcTable() { return &tbl_vk; }

void cgpu_query_instance_features_vulkan(CGpuInstanceId instance, struct CGpuInstanceFeatures* features)
{
    features->specialization_constant = true;
}

void cgpu_enum_adapters_vulkan(CGpuInstanceId instance, CGpuAdapterId* const adapters, uint32_t* adapters_num)
{
    CGpuInstance_Vulkan* I = (CGpuInstance_Vulkan*)instance;
    *adapters_num = I->mPhysicalDeviceCount;
    if (adapters != CGPU_NULLPTR)
    {
        for (uint32_t i = 0; i < I->mPhysicalDeviceCount; i++)
        {
            adapters[i] = &I->pVulkanAdapters[i].super;
        }
    }
}

const CGpuAdapterDetail* cgpu_query_adapter_detail_vulkan(const CGpuAdapterId adapter)
{
    CGpuAdapter_Vulkan* a = (CGpuAdapter_Vulkan*)adapter;
    return &a->adapter_detail;
}

uint32_t cgpu_query_queue_count_vulkan(const CGpuAdapterId adapter, const ECGpuQueueType type)
{
    CGpuAdapter_Vulkan* a = (CGpuAdapter_Vulkan*)adapter;
    uint32_t count = 0;
    switch (type)
    {
        case ECGpuQueueType_Graphics: {
            for (uint32_t i = 0; i < a->mQueueFamiliesCount; i++)
            {
                const VkQueueFamilyProperties* prop = &a->pQueueFamilyProperties[i];
                if (prop->queueFlags & VK_QUEUE_GRAPHICS_BIT)
                {
                    count += prop->queueCount;
                }
            }
        }
        break;
        case ECGpuQueueType_Compute: {
            for (uint32_t i = 0; i < a->mQueueFamiliesCount; i++)
            {
                const VkQueueFamilyProperties* prop = &a->pQueueFamilyProperties[i];
                if (prop->queueFlags & VK_QUEUE_COMPUTE_BIT)
                {
                    if (!(prop->queueFlags & VK_QUEUE_GRAPHICS_BIT))
                    {
                        count += prop->queueCount;
                    }
                }
            }
        }
        break;
        case ECGpuQueueType_Transfer: {
            for (uint32_t i = 0; i < a->mQueueFamiliesCount; i++)
            {
                const VkQueueFamilyProperties* prop = &a->pQueueFamilyProperties[i];
                if (prop->queueFlags & VK_QUEUE_TRANSFER_BIT)
                {
                    if (!(prop->queueFlags & VK_QUEUE_COMPUTE_BIT))
                    {
                        if (!(prop->queueFlags & VK_QUEUE_GRAPHICS_BIT))
                        {
                            count += prop->queueCount;
                        }
                    }
                }
            }
        }
        break;
        default:
            assert(0 && "CGPU VULKAN: ERROR Queue Type!");
    }
    return count;
}

// API Objects APIs
CGpuFenceId cgpu_create_fence_vulkan(CGpuDeviceId device)
{
    CGpuDevice_Vulkan* D = (CGpuDevice_Vulkan*)device;
    CGpuFence_Vulkan* F = (CGpuFence_Vulkan*)cgpu_calloc(1, sizeof(CGpuFence_Vulkan));
    assert(F);
    VkFenceCreateInfo add_info = {
        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
    };

    CHECK_VKRESULT(D->mVkDeviceTable.vkCreateFence(
        D->pVkDevice, &add_info, GLOBAL_VkAllocationCallbacks, &F->pVkFence));
    F->mSubmitted = false;
    return &F->super;
}

void cgpu_free_fence_vulkan(CGpuFenceId fence)
{
    CGpuFence_Vulkan* F = (CGpuFence_Vulkan*)fence;
    const CGpuDevice_Vulkan* D = (CGpuDevice_Vulkan*)fence->device;
    D->mVkDeviceTable.vkDestroyFence(D->pVkDevice, F->pVkFence, GLOBAL_VkAllocationCallbacks);
    cgpu_free(F);
}

CGpuRootSignatureId cgpu_create_root_signature_vulkan(CGpuDeviceId device,
    const struct CGpuRootSignatureDescriptor* desc)
{
    const CGpuDevice_Vulkan* D = (CGpuDevice_Vulkan*)device;
    CGpuRootSignature_Vulkan* RS = (CGpuRootSignature_Vulkan*)cgpu_calloc(1, sizeof(CGpuRootSignature_Vulkan));
    DECLARE_ZERO_VLA(CGpuShaderReflection*, entry_reflections, desc->shaders_count)
    // pick shader reflection data
    for (uint32_t i = 0; i < desc->shaders_count; i++)
    {
        const CGpuPipelineShaderDescriptor* shader_desc = &desc->shaders[i];
        // find shader refl
        for (uint32_t j = 0; j < shader_desc->library->entrys_count; j++)
        {
            CGpuShaderReflection* temp_entry_reflcetion = &shader_desc->library->entry_reflections[j];
            if (strcmp(shader_desc->entry, temp_entry_reflcetion->entry_name) == 0)
            {
                entry_reflections[i] = temp_entry_reflcetion;
                break;
            }
        }
    }
    // pick binder infos
    uint32_t max_sets = 0;
    uint32_t max_binding = 0;
    // Collect Shader Resources
    for (uint32_t i = 0; i < desc->shaders_count; i++)
    {
        for (uint32_t j = 0; j < entry_reflections[i]->shader_resources_count; j++)
        {
            CGpuShaderResource* resource = &entry_reflections[i]->shader_resources[j];
            max_sets = max_sets > resource->set ? max_sets : resource->set;
            max_binding = max_binding > resource->binding ? max_binding : resource->binding;
        }
    }
    DECLARE_ZERO_VLA(uint32_t, valid_bindings, max_sets)
    DECLARE_ZERO_VLA(CGpuShaderResource, sig_reflections, max_sets * max_binding)
    for (uint32_t i = 0; i < desc->shaders_count; i++)
    {
        for (uint32_t j = 0; j < entry_reflections[i]->shader_resources_count; j++)
        {
            CGpuShaderResource* resource = &entry_reflections[i]->shader_resources[j];
            memcpy(&sig_reflections[resource->set * resource->binding], resource, sizeof(CGpuShaderResource));
            valid_bindings[resource->set] = valid_bindings[resource->set] > resource->binding ? valid_bindings[resource->set] : resource->binding;
        }
    }
    // Collect Secure Set Count
    // Create Pipeline Layout
    VkPipelineLayoutCreateInfo pipeline_info = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .pNext = NULL,
        .flags = 0,
        .setLayoutCount = 0,
        .pSetLayouts = NULL,
        .pushConstantRangeCount = 0,
        .pPushConstantRanges = NULL
    };
    CHECK_VKRESULT(D->mVkDeviceTable.vkCreatePipelineLayout(D->pVkDevice,
        &pipeline_info, GLOBAL_VkAllocationCallbacks, &RS->pipeline_layout));
    return &RS->super;
}

void cgpu_free_root_signature_vulkan(CGpuRootSignatureId signature)
{
    CGpuRootSignature_Vulkan* RS = (CGpuRootSignature_Vulkan*)signature;
    const CGpuDevice_Vulkan* D = (CGpuDevice_Vulkan*)signature->device;
    D->mVkDeviceTable.vkDestroyPipelineLayout(D->pVkDevice, RS->pipeline_layout, GLOBAL_VkAllocationCallbacks);
    cgpu_free(RS);
}

// Queue APIs
CGpuQueueId cgpu_get_queue_vulkan(CGpuDeviceId device, ECGpuQueueType type, uint32_t index)
{
    assert(device && "CGPU VULKAN: NULL DEVICE!");
    CGpuDevice_Vulkan* D = (CGpuDevice_Vulkan*)device;
    CGpuAdapter_Vulkan* A = (CGpuAdapter_Vulkan*)device->adapter;

    CGpuQueue_Vulkan Q = { .super = { .device = &D->super, .index = index, .type = type } };
    D->mVkDeviceTable.vkGetDeviceQueue(D->pVkDevice, (uint32_t)A->mQueueFamilyIndices[type], index, &Q.pVkQueue);
    Q.mVkQueueFamilyIndex = (uint32_t)A->mQueueFamilyIndices[type];
    Q.mTimestampPeriod = A->mPhysicalDeviceProps.properties.limits.timestampPeriod;

    CGpuQueue_Vulkan* RQ = (CGpuQueue_Vulkan*)cgpu_calloc(1, sizeof(CGpuQueue_Vulkan));
    memcpy(RQ, &Q, sizeof(Q));
    return &RQ->super;
}

void cgpu_submit_queue_vulkan(CGpuQueueId queue, const struct CGpuQueueSubmitDescriptor* desc)
{
    uint32_t CmdCount = desc->cmds_count;
    CGpuCommandBuffer_Vulkan** Cmds = (CGpuCommandBuffer_Vulkan**)desc->cmds;
    CGpuQueue_Vulkan* Q = (CGpuQueue_Vulkan*)queue;
    CGpuDevice_Vulkan* D = (CGpuDevice_Vulkan*)queue->device;
    CGpuFence_Vulkan* F = (CGpuFence_Vulkan*)desc->signal_fence;

    // ASSERT that given cmd list and given params are valid
    assert(CmdCount > 0);
    assert(Cmds);
    // execute given command list
    assert(Q->pVkQueue != VK_NULL_HANDLE);

    DECLARE_ZERO_VLA(VkCommandBuffer, vkcmds, CmdCount);
    for (uint32_t i = 0; i < CmdCount; ++i)
    {
        vkcmds[i] = Cmds[i]->pVkCmdBuf;
    }

    // TODO: Add Necessary Semaphores
    VkSubmitInfo submit_info = {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .pNext = NULL,
        .waitSemaphoreCount = 0,
        .pWaitSemaphores = NULL,
        .pWaitDstStageMask = 0,
        .commandBufferCount = CmdCount,
        .pCommandBuffers = vkcmds,
        .signalSemaphoreCount = 0,
        .pSignalSemaphores = NULL,
    };
    // TODO: Thread Safety ?
    CHECK_VKRESULT(D->mVkDeviceTable.vkQueueSubmit(
        Q->pVkQueue, 1, &submit_info, F ? F->pVkFence : VK_NULL_HANDLE));
    if (F)
        F->mSubmitted = true;
}

void cgpu_wait_queue_idle_vulkan(CGpuQueueId queue)
{
    CGpuQueue_Vulkan* Q = (CGpuQueue_Vulkan*)queue;
    CGpuDevice_Vulkan* D = (CGpuDevice_Vulkan*)queue->device;
    D->mVkDeviceTable.vkQueueWaitIdle(Q->pVkQueue);
}

void cgpu_free_queue_vulkan(CGpuQueueId queue)
{
    cgpu_free((void*)queue);
}

VkCommandPool allocate_transient_command_pool(CGpuDevice_Vulkan* D, CGpuQueueId queue)
{
    VkCommandPool P = VK_NULL_HANDLE;
    // CGpuQueue_Vulkan* Q = (CGpuQueue_Vulkan*)queue;
    CGpuAdapter_Vulkan* A = (CGpuAdapter_Vulkan*)queue->device->adapter;

    VkCommandPoolCreateInfo create_info = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .pNext = NULL,
        // transient.
        .flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT,
        .queueFamilyIndex = (uint32_t)A->mQueueFamilyIndices[queue->type]
    };
    CHECK_VKRESULT(D->mVkDeviceTable.vkCreateCommandPool(
        D->pVkDevice, &create_info, GLOBAL_VkAllocationCallbacks, &P));
    return P;
}

void free_transient_command_pool(CGpuDevice_Vulkan* D, VkCommandPool pool)
{
    D->mVkDeviceTable.vkDestroyCommandPool(D->pVkDevice, pool, GLOBAL_VkAllocationCallbacks);
}

CGpuCommandPoolId cgpu_create_command_pool_vulkan(CGpuQueueId queue, const CGpuCommandPoolDescriptor* desc)
{
    CGpuDevice_Vulkan* D = (CGpuDevice_Vulkan*)queue->device;
    CGpuCommandPool_Vulkan* P = (CGpuCommandPool_Vulkan*)cgpu_calloc(1, sizeof(CGpuCommandPool_Vulkan));
    P->pVkCmdPool = allocate_transient_command_pool(D, queue);
    return &P->super;
}

CGpuCommandBufferId cgpu_create_command_buffer_vulkan(CGpuCommandPoolId pool, const struct CGpuCommandBufferDescriptor* desc)
{
    assert(pool);
    CGpuCommandPool_Vulkan* P = (CGpuCommandPool_Vulkan*)pool;
    CGpuQueue_Vulkan* Q = (CGpuQueue_Vulkan*)P->super.queue;
    CGpuDevice_Vulkan* D = (CGpuDevice_Vulkan*)Q->super.device;
    CGpuCommandBuffer_Vulkan* Cmd = (CGpuCommandBuffer_Vulkan*)cgpu_calloc_aligned(
        1, sizeof(CGpuCommandBuffer_Vulkan), _Alignof(CGpuCommandBuffer_Vulkan));
    assert(Cmd);

    Cmd->pCmdPool = P;
    Cmd->mType = Q->super.type;
    Cmd->mNodeIndex = SINGLE_GPU_NODE_MASK;

    VkCommandBufferAllocateInfo alloc_info = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .pNext = NULL,
        .commandPool = P->pVkCmdPool,
        .level = desc->is_secondary ? VK_COMMAND_BUFFER_LEVEL_SECONDARY : VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1
    };
    CHECK_VKRESULT(D->mVkDeviceTable.vkAllocateCommandBuffers(D->pVkDevice, &alloc_info, &(Cmd->pVkCmdBuf)));
    return &Cmd->super;
}

void cgpu_free_command_buffer_vulkan(CGpuCommandBufferId cmd)
{
    CGpuCommandBuffer_Vulkan* Cmd = (CGpuCommandBuffer_Vulkan*)cmd;
    CGpuCommandPool_Vulkan* P = (CGpuCommandPool_Vulkan*)cmd->pool;
    CGpuQueue_Vulkan* Q = (CGpuQueue_Vulkan*)P->super.queue;
    CGpuDevice_Vulkan* D = (CGpuDevice_Vulkan*)Q->super.device;
    D->mVkDeviceTable.vkFreeCommandBuffers(D->pVkDevice, P->pVkCmdPool, 1, &(Cmd->pVkCmdBuf));
    cgpu_free(Cmd);
}

void cgpu_free_command_pool_vulkan(CGpuCommandPoolId pool)
{
    CGpuDevice_Vulkan* D = (CGpuDevice_Vulkan*)pool->queue->device;
    CGpuCommandPool_Vulkan* P = (CGpuCommandPool_Vulkan*)pool;
    free_transient_command_pool(D, P->pVkCmdPool);
    cgpu_free(P);
}

// CMDs
void cgpu_cmd_begin_vulkan(CGpuCommandBufferId cmd)
{
    CGpuCommandBuffer_Vulkan* Cmd = (CGpuCommandBuffer_Vulkan*)cmd;
    CGpuDevice_Vulkan* D = (CGpuDevice_Vulkan*)cmd->device;
    VkCommandBufferBeginInfo begin_info = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .pNext = NULL,
        .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
        .pInheritanceInfo = NULL
    };
    CHECK_VKRESULT(D->mVkDeviceTable.vkBeginCommandBuffer(Cmd->pVkCmdBuf, &begin_info));
    Cmd->pBoundPipelineLayout = CGPU_NULL;
}

void cgpu_cmd_end_vulkan(CGpuCommandBufferId cmd)
{
    CGpuCommandBuffer_Vulkan* Cmd = (CGpuCommandBuffer_Vulkan*)cmd;
    CGpuDevice_Vulkan* D = (CGpuDevice_Vulkan*)cmd->device;
    if (Cmd->pVkActiveRenderPass)
    {
        vkCmdEndRenderPass(Cmd->pVkCmdBuf);
    }
    Cmd->pVkActiveRenderPass = VK_NULL_HANDLE;
    CHECK_VKRESULT(D->mVkDeviceTable.vkEndCommandBuffer(Cmd->pVkCmdBuf));
}

#define clamp(x, min, max) (x) < (min) ? (min) : ((x) > (max) ? (max) : (x));
// SwapChain APIs
CGpuSwapChainId cgpu_create_swapchain_vulkan(CGpuDeviceId device, const CGpuSwapChainDescriptor* desc)
{
    CGpuDevice_Vulkan* D = (CGpuDevice_Vulkan*)device;
    CGpuAdapter_Vulkan* A = (CGpuAdapter_Vulkan*)device->adapter;
    // CGpuInstance_Vulkan* I = (CGpuInstance_Vulkan*)device->adapter->instance;
    VkSurfaceKHR vkSurface = (VkSurfaceKHR)desc->surface;

    VkSurfaceCapabilitiesKHR caps = { 0 };
    CHECK_VKRESULT(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(A->pPhysicalDevice, vkSurface, &caps));
    if ((caps.maxImageCount > 0) && (desc->imageCount > caps.maxImageCount))
    {
        ((CGpuSwapChainDescriptor*)desc)->imageCount = caps.maxImageCount;
    }
    else if (desc->imageCount < caps.minImageCount)
    {
        ((CGpuSwapChainDescriptor*)desc)->imageCount = caps.minImageCount;
    }

    // Surface format
    // Select a surface format, depending on whether HDR is available.
    DECLARE_ZERO(VkSurfaceFormatKHR, surface_format)
    surface_format.format = VK_FORMAT_UNDEFINED;
    uint32_t surfaceFormatCount = 0;
    CHECK_VKRESULT(vkGetPhysicalDeviceSurfaceFormatsKHR(
        A->pPhysicalDevice, vkSurface, &surfaceFormatCount, CGPU_NULLPTR));
    // Allocate and get surface formats
    DECLARE_ZERO_VLA(VkSurfaceFormatKHR, formats, surfaceFormatCount)
    CHECK_VKRESULT(vkGetPhysicalDeviceSurfaceFormatsKHR(
        A->pPhysicalDevice, vkSurface, &surfaceFormatCount, formats))

    const VkSurfaceFormatKHR hdrSurfaceFormat = {
        //
        VK_FORMAT_A2B10G10R10_UNORM_PACK32,
        VK_COLOR_SPACE_HDR10_ST2084_EXT
        //
    };
    if ((1 == surfaceFormatCount) && (VK_FORMAT_UNDEFINED == formats[0].format))
    {
        surface_format.format = VK_FORMAT_B8G8R8A8_UNORM;
        surface_format.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
    }
    else
    {
        VkFormat requested_format = VkUtil_FormatTranslateToVk(desc->format);
        VkColorSpaceKHR requested_color_space = requested_format == hdrSurfaceFormat.format ? hdrSurfaceFormat.colorSpace : VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
        for (uint32_t i = 0; i < surfaceFormatCount; ++i)
        {
            if ((requested_format == formats[i].format) && (requested_color_space == formats[i].colorSpace))
            {
                surface_format.format = requested_format;
                surface_format.colorSpace = requested_color_space;
                break;
            }
        }
        // Default to VK_FORMAT_B8G8R8A8_UNORM if requested format isn't found
        if (VK_FORMAT_UNDEFINED == surface_format.format)
        {
            surface_format.format = VK_FORMAT_B8G8R8A8_UNORM;
            surface_format.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
        }
    }
    assert(VK_FORMAT_UNDEFINED != surface_format.format);

    // The VK_PRESENT_MODE_FIFO_KHR mode must always be present as per spec
    // This mode waits for the vertical blank ("v-sync")
    VkPresentModeKHR present_mode = VK_PRESENT_MODE_FIFO_KHR;
    uint32_t swapChainImageCount = 0;
    // Get present mode count
    CHECK_VKRESULT(vkGetPhysicalDeviceSurfacePresentModesKHR(
        A->pPhysicalDevice, vkSurface, &swapChainImageCount, NULL));
    // Allocate and get present modes
    DECLARE_ZERO_VLA(VkPresentModeKHR, modes, swapChainImageCount)
    CHECK_VKRESULT(vkGetPhysicalDeviceSurfacePresentModesKHR(
        A->pPhysicalDevice, vkSurface, &swapChainImageCount, modes));
    // Select Preferred Present Mode
    VkPresentModeKHR preferredModeList[] = {
        VK_PRESENT_MODE_IMMEDIATE_KHR,    // normal
        VK_PRESENT_MODE_MAILBOX_KHR,      // low latency
        VK_PRESENT_MODE_FIFO_RELAXED_KHR, // minimize stuttering
        VK_PRESENT_MODE_FIFO_KHR          // low power consumption
    };
    const uint32_t preferredModeCount = CGPU_ARRAY_LEN(preferredModeList);
    uint32_t preferredModeStartIndex = desc->enableVsync ? 1 : 0;
    for (uint32_t j = preferredModeStartIndex; j < preferredModeCount; ++j)
    {
        VkPresentModeKHR mode = preferredModeList[j];
        uint32_t i = 0;
        for (i = 0; i < swapChainImageCount; ++i)
        {
            if (modes[i] == mode)
            {
                break;
            }
        }
        if (i < swapChainImageCount)
        {
            present_mode = mode;
            break;
        }
    }
    // Swapchain
    VkExtent2D extent;
    extent.width = clamp(desc->width, caps.minImageExtent.width, caps.maxImageExtent.width);
    extent.height = clamp(desc->height, caps.minImageExtent.height, caps.maxImageExtent.height);

    CGpuQueue_Vulkan* Q = (CGpuQueue_Vulkan*)desc->presentQueues[0];
    VkSharingMode sharing_mode = VK_SHARING_MODE_EXCLUSIVE;
    uint32_t presentQueueFamilyIndex = -1;
    // Check Queue Present Support.
    {
        VkBool32 sup = VK_FALSE;
        VkResult res =
            vkGetPhysicalDeviceSurfaceSupportKHR(A->pPhysicalDevice, Q->mVkQueueFamilyIndex, vkSurface, &sup);
        if ((VK_SUCCESS == res) && (VK_TRUE == sup))
        {
            presentQueueFamilyIndex = Q->mVkQueueFamilyIndex;
        }
        else
        {
            // Get queue family properties
            uint32_t queueFamilyPropertyCount = 0;
            vkGetPhysicalDeviceQueueFamilyProperties(A->pPhysicalDevice, &queueFamilyPropertyCount, NULL);
            DECLARE_ZERO_VLA(VkQueueFamilyProperties, queueFamilyProperties, queueFamilyPropertyCount)
            vkGetPhysicalDeviceQueueFamilyProperties(
                A->pPhysicalDevice, &queueFamilyPropertyCount, queueFamilyProperties);

            // Check if hardware provides dedicated present queue
            if (queueFamilyPropertyCount)
            {
                for (uint32_t index = 0; index < queueFamilyPropertyCount; ++index)
                {
                    VkBool32 supports_present = VK_FALSE;
                    VkResult res = vkGetPhysicalDeviceSurfaceSupportKHR(
                        A->pPhysicalDevice, index, vkSurface, &supports_present);
                    if ((VK_SUCCESS == res) && (VK_TRUE == supports_present) && Q->mVkQueueFamilyIndex != index)
                    {
                        presentQueueFamilyIndex = index;
                        break;
                    }
                }
                // If there is no dedicated present queue, just find the first available queue which supports
                // present
                if (presentQueueFamilyIndex == -1)
                {
                    for (uint32_t index = 0; index < queueFamilyPropertyCount; ++index)
                    {
                        VkBool32 supports_present = VK_FALSE;
                        VkResult res = vkGetPhysicalDeviceSurfaceSupportKHR(
                            A->pPhysicalDevice, index, vkSurface, &supports_present);
                        if ((VK_SUCCESS == res) && (VK_TRUE == supports_present))
                        {
                            presentQueueFamilyIndex = index;
                            break;
                        }
                        else
                        {
                            // No present queue family available. Something goes wrong.
                            assert(0);
                        }
                    }
                }
            }
        }
    }

    VkSurfaceTransformFlagBitsKHR pre_transform;
    // #TODO: Add more if necessary but identity should be enough for now
    if (caps.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
    {
        pre_transform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    }
    else
    {
        pre_transform = caps.currentTransform;
    }

    VkCompositeAlphaFlagBitsKHR compositeAlphaFlags[] = {
        VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR,
        VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
        VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,
    };
    VkCompositeAlphaFlagBitsKHR composite_alpha = VK_COMPOSITE_ALPHA_FLAG_BITS_MAX_ENUM_KHR;
    for (uint32_t _i = 0; _i < CGPU_ARRAY_LEN(compositeAlphaFlags); _i++)
    {
        if (caps.supportedCompositeAlpha & compositeAlphaFlags[_i])
        {
            composite_alpha = compositeAlphaFlags[_i];
            break;
        }
    }
    assert(composite_alpha != VK_COMPOSITE_ALPHA_FLAG_BITS_MAX_ENUM_KHR);

    CGpuSwapChain_Vulkan* S = (CGpuSwapChain_Vulkan*)cgpu_calloc(1, sizeof(CGpuSwapChain_Vulkan));
    S->super.device = device;
    VkSwapchainCreateInfoKHR swapChainCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .pNext = NULL,
        .flags = 0,
        .surface = vkSurface,
        .minImageCount = desc->imageCount,
        .imageFormat = surface_format.format,
        .imageColorSpace = surface_format.colorSpace,
        .imageExtent = extent,
        .imageArrayLayers = 1,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .imageSharingMode = sharing_mode,
        .queueFamilyIndexCount = 1,
        .pQueueFamilyIndices = &presentQueueFamilyIndex,
        .preTransform = pre_transform,
        .compositeAlpha = composite_alpha,
        .presentMode = present_mode,
        .clipped = VK_TRUE,
        .oldSwapchain = VK_NULL_HANDLE
    };
    VkResult res = D->mVkDeviceTable.vkCreateSwapchainKHR(
        D->pVkDevice, &swapChainCreateInfo, GLOBAL_VkAllocationCallbacks, &S->pVkSwapChain);
    if (VK_SUCCESS != res)
    {
        assert(0 && "fatal: vkCreateSwapchainKHR failed!");
    }
    S->pVkSurface = vkSurface;
    return &S->super;
}

void cgpu_free_swapchain_vulkan(CGpuSwapChainId swapchain)
{
    CGpuSwapChain_Vulkan* S = (CGpuSwapChain_Vulkan*)swapchain;
    CGpuDevice_Vulkan* D = (CGpuDevice_Vulkan*)swapchain->device;

    D->mVkDeviceTable.vkDestroySwapchainKHR(D->pVkDevice, S->pVkSwapChain, GLOBAL_VkAllocationCallbacks);

    cgpu_free((void*)swapchain);
}

// exts
#include "cgpu/extensions/cgpu_vulkan_exts.h"