#pragma once
#include "cgpu/api.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct CGPUDREDSettings {
    struct ID3D12DeviceRemovedExtendedDataSettings* pDredSettings;
} CGPUDREDSettings;
typedef struct CGPUDREDSettings* CGPUDREDSettingsId;

// Modifications to DRED settings have no effect on devices already created.
// But subsequent calls to D3D12CreateDevice use the most recent DRED settings.
RUNTIME_API CGPUDREDSettingsId cgpu_d3d12_enable_DRED();

// Modifications to DRED settings have no effect on devices already created.
// But subsequent calls to D3D12CreateDevice use the most recent DRED settings.
RUNTIME_API void cgpu_d3d12_disable_DRED(CGPUDREDSettingsId settings);

#ifdef __cplusplus
} // end extern "C"
#endif