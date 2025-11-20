#pragma once
#include "Audio.h"
#include <map>
#include <RtAudio.h>
#include <mutex>

namespace Models::Audio
{
    extern RtAudio audioInput;
    extern RtAudio audioOutput;
    extern std::map<int, RtAudio::DeviceInfo> InputDevices;
    extern std::map<int, RtAudio::DeviceInfo> OutputDevices;
    extern int DefaultInputDeviceID;
    extern int DefaultOutputDeviceID;
    extern bool DevicesProbed;

#pragma region  Audio Mixer
    struct SourceState {
        uint32_t lastSequence;
        std::vector<float> lastSamples;  // Last frame for interpolation
        bool initialized;
    };

    extern std::mutex mixerMutex;
    extern unsigned int sampleRate;
    extern unsigned int channels;
    extern std::vector<float> mixedBuffer;
    extern std::map<int, SourceState> sourceStates;  // Track each source

    void MixAudio(int sourceID, const float* data, size_t numFrames, uint32_t sequenceNumber);
    size_t GetMixedAudio(float* buffer, size_t numFrames);
    void ConcealAudioLoss(int sourceID, size_t numFrames);
#pragma endregion
}