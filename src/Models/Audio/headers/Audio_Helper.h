#pragma once
#include "Audio.h"
#include <map>
#include <RtAudio.h>

namespace Models::Audio
{
    extern RtAudio audio;
    extern std::map<int, RtAudio::DeviceInfo> InputDevices;
    extern std::map<int, RtAudio::DeviceInfo> OutputDevices;
    extern int DefaultInputDeviceID;
    extern int DefaultOutputDeviceID;
    extern bool DevicesProbed;
}