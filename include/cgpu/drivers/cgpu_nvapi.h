#pragma once
#ifdef __cplusplus
    #if defined(_WIN32) && !defined(DURANGO)
        #include "nvapi/nvapi.h"
        #define NVAPI
    #endif
extern "C" {
#endif
#include "cgpu/api.h"

struct ID3D12Device;

ECGPUNvAPI_Status cgpu_nvapi_init(struct CGPUInstance* Inst);
uint32_t cgpu_nvapi_get_driver_version();
uint64_t cgpu_nvapi_d3d12_query_cpu_visible_vram(struct ID3D12Device* Device);
void cgpu_nvapi_exit();

#ifdef __cplusplus
}
#endif