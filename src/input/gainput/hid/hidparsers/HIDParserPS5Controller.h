#pragma once

// #include "../../../../../../../../OS/Core/Config.h"
#include "platform/configure.h"

struct HIDDeviceInfo;
struct HIDController;

bool HIDIsSupportedPS5Controller(HIDDeviceInfo* devInfo);
int HIDOpenPS5Controller(HIDDeviceInfo* devInfo, HIDController* controller, uint8_t playerNum);
