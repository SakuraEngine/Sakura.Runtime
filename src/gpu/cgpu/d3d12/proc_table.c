#include "cgpu/backend/d3d12/cgpu_d3d12.h"
#include "cgpu/backend/d3d12/cgpu_d3d12_surfaces.h"

const CGpuProcTable tbl_d3d12 = {
    .create_instance = &cgpu_create_instance_d3d12,
    .query_instance_features = &cgpu_query_instance_features_d3d12,
    .free_instance = &cgpu_free_instance_d3d12,

    .enum_adapters = &cgpu_enum_adapters_d3d12,
    .query_adapter_detail = &cgpu_query_adapter_detail_d3d12,
    .query_queue_count = &cgpu_query_queue_count_d3d12,

    .create_device = &cgpu_create_device_d3d12,
    .free_device = &cgpu_free_device_d3d12,

    .create_fence = &cgpu_create_fence_d3d12,
    .free_fence = &cgpu_free_fence_d3d12,
    .create_root_signature = &cgpu_create_root_signature_d3d12,
    .free_root_signature = &cgpu_free_root_signature_d3d12,

    .get_queue = &cgpu_get_queue_d3d12,
    .submit_queue = &cgpu_submit_queue_d3d12,
    .wait_queue_idle = &cgpu_wait_queue_idle_d3d12,
    .free_queue = &cgpu_free_queue_d3d12,

    .create_command_pool = &cgpu_create_command_pool_d3d12,
    .create_command_buffer = &cgpu_create_command_buffer_d3d12,
    .free_command_buffer = &cgpu_free_command_buffer_d3d12,
    .free_command_pool = &cgpu_free_command_pool_d3d12,

    .create_shader_library = &cgpu_create_shader_library_d3d12,
    .free_shader_library = &cgpu_free_shader_library_d3d12,

    .create_buffer = &cgpu_create_buffer_d3d12,
    .map_buffer = &cgpu_map_buffer_d3d12,
    .unmap_buffer = &cgpu_unmap_buffer_d3d12,
    .free_buffer = &cgpu_free_buffer_d3d12,

    .create_swapchain = &cgpu_create_swapchain_d3d12,
    .free_swapchain = &cgpu_free_swapchain_d3d12,

    .cmd_begin = &cgpu_cmd_begin_d3d12,
    .cmd_update_buffer = &cgpu_cmd_update_buffer_d3d12,
    .cmd_end = &cgpu_cmd_end_d3d12
};

const CGpuProcTable* CGPU_D3D12ProcTable()
{
    return &tbl_d3d12;
}

const CGpuSurfacesProcTable s_tbl_d3d12 = {
    //
    .free_surface = cgpu_free_surface_d3d12,
#if defined(_WIN32) || defined(_WIN64)
    .from_hwnd = cgpu_surface_from_hwnd_d3d12
#endif
    //
};

const CGpuSurfacesProcTable* CGPU_D3D12SurfacesProcTable()
{
    return &s_tbl_d3d12;
}
