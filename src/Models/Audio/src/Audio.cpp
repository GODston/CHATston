#include "../headers/Audio_Helper.h"


namespace Models::Audio
{
    bool DevicesProbed = false;
    RtAudio audioInput;
    RtAudio audioOutput;
    std::map<int, RtAudio::DeviceInfo> InputDevices = {};
    std::map<int, RtAudio::DeviceInfo> OutputDevices = {};
    int DefaultInputDeviceID = -1;
    int DefaultOutputDeviceID = -1;
    unsigned int sampleRate = 44100;
    unsigned int channels = 2;

    void ChangeSampleRate(unsigned int newSampleRate)
    {
        std::lock_guard<std::mutex> lock(mixerMutex);
        sampleRate = newSampleRate;
    }

    void ChangeChannels(unsigned int newChannels)
    {
        std::lock_guard<std::mutex> lock(mixerMutex);
        channels = newChannels;
    }

    #pragma region Audio Mixer implementation
    std::mutex mixerMutex;
    std::vector<float> mixedBuffer;
    std::map<int, SourceState> sourceStates;

    void MixAudio(int sourceID, const float* data, size_t numFrames, uint32_t sequenceNumber)
    {
        std::lock_guard<std::mutex> lock(mixerMutex);
        
        // Initialize source state if new
        if (sourceStates.find(sourceID) == sourceStates.end())
        {
            sourceStates[sourceID] = {sequenceNumber, std::vector<float>(channels, 0.0f), true};
        }
        
        SourceState& state = sourceStates[sourceID];
        
        // Detect packet loss
        if (state.initialized && sequenceNumber != state.lastSequence + 1)
        {
            uint32_t lostPackets = sequenceNumber - state.lastSequence - 1;
            
            // Conceal lost packets (up to a reasonable limit)
            if (lostPackets > 0 && lostPackets < 10)
            {
                for (uint32_t i = 0; i < lostPackets; ++i)
                {
                    ConcealAudioLoss(sourceID, numFrames);
                }
            }
        }
        
        // Add actual audio data
        size_t numSamples = numFrames * channels;
        size_t currentSize = mixedBuffer.size();
        
        if (currentSize < numSamples)
        {
            mixedBuffer.resize(numSamples, 0.0f);
            for (size_t i = 0; i < numSamples; ++i)
            {
                mixedBuffer[i] = data[i];
            }
        }
        else
        {
            for (size_t i = 0; i < numSamples && i < currentSize; ++i)
            {
                mixedBuffer[i] += data[i];
            }
        }
        
        // Update state: save last samples and sequence
        state.lastSequence = sequenceNumber;
        state.lastSamples.assign(data + (numSamples - channels), data + numSamples);
        state.initialized = true;
    }

    void ConcealAudioLoss(int sourceID, size_t numFrames)
    {
        // Packet Loss Concealment using fade-out of last sample
        SourceState& state = sourceStates[sourceID];
        
        size_t numSamples = numFrames * channels;
        size_t currentSize = mixedBuffer.size();
        
        // Generate concealment audio (linear fade to zero)
        std::vector<float> concealedAudio(numSamples);
        for (size_t frame = 0; frame < numFrames; ++frame)
        {
            float fadeAmount = 1.0f - (static_cast<float>(frame) / numFrames);
            for (size_t ch = 0; ch < channels; ++ch)
            {
                concealedAudio[frame * channels + ch] = state.lastSamples[ch] * fadeAmount;
            }
        }
        
        // Mix concealed audio
        if (currentSize < numSamples)
        {
            mixedBuffer.resize(numSamples, 0.0f);
            for (size_t i = 0; i < numSamples; ++i)
            {
                mixedBuffer[i] = concealedAudio[i];
            }
        }
        else
        {
            for (size_t i = 0; i < numSamples && i < currentSize; ++i)
            {
                mixedBuffer[i] += concealedAudio[i];
            }
        }
    }

    size_t GetMixedAudio(float* buffer, size_t numFrames)
    {
        std::lock_guard<std::mutex> lock(mixerMutex);
        
        size_t numSamples = numFrames * channels;
        size_t samplesToRead = std::min(numSamples, mixedBuffer.size());
        
        for (size_t i = 0; i < samplesToRead; ++i)
        {
            buffer[i] = mixedBuffer[i];
        }
        
        if (samplesToRead > 0)
        {
            mixedBuffer.erase(mixedBuffer.begin(), mixedBuffer.begin() + samplesToRead);
        }
        
        return samplesToRead / channels;
    }

    void Clear()
    {
        std::lock_guard<std::mutex> lock(mixerMutex);
        mixedBuffer.clear();
    }

    size_t GetBufferSize() 
    {
        std::lock_guard<std::mutex> lock(mixerMutex);
        return mixedBuffer.size() / channels;
    }
    #pragma endregion

    #pragma region Private declarations
    bool ProbeDevicesIfNeeded();
    #pragma endregion

    #pragma region Public definitions
    bool ProbeDevices()
    {
        InputDevices.clear();
        OutputDevices.clear();
        DevicesProbed = false;

        // Doesnt matter which instance of RtAudio we use here
        std::vector<unsigned int> deviceIds = audioInput.getDeviceIds();
        
        for (unsigned int id : deviceIds)
        {
            RtAudio::DeviceInfo info = audioInput.getDeviceInfo(id);
            if (info.inputChannels > 0)
            {
                Models::Audio::InputDevices[id] = info;
                if(info.isDefaultInput || Models::Audio::DefaultInputDeviceID == -1)
                {
                    Models::Audio::DefaultInputDeviceID = id;
                }
            }
            if (info.outputChannels > 0)
            {
                Models::Audio::OutputDevices[id] = info;
                if(info.isDefaultOutput || Models::Audio::DefaultOutputDeviceID == -1)
                {
                    Models::Audio::DefaultOutputDeviceID = id;
                }
            }
        }

        return DevicesProbed = true;
    }

    std::vector<AudioDevice> GetInputDevices()
    {
        std::vector<AudioDevice> devices;
        if(!ProbeDevicesIfNeeded())
        {
            return devices;
        }

        for(const auto& pair : InputDevices)
        {
            AudioDevice device;
            device.name = pair.second.name;
            device.id = pair.first;
            devices.push_back(device);
        }

        return devices;
    }

    std::vector<AudioDevice> GetOutputDevices()
    {
        std::vector<AudioDevice> devices;
        if(!ProbeDevicesIfNeeded())
        {
            return devices;
        }

        for(const auto& pair : OutputDevices)
        {
            AudioDevice device;
            device.name = pair.second.name;
            device.id = pair.first;
            devices.push_back(device);
        }

        return devices;
    }

    bool GetDefaultInputDevice(AudioDevice* buffer)
    {
        if(!ProbeDevicesIfNeeded())
        {
            return false;
        }

        if(buffer && DefaultInputDeviceID >= 0)
        {
            buffer->name = InputDevices[DefaultInputDeviceID].name;
            buffer->id = DefaultInputDeviceID;
        }

        return true;
    }

    bool GetDefaultOutputDevice(AudioDevice* buffer)
    {
        if(!ProbeDevicesIfNeeded())
        {
            return false;
        }

        if(buffer && DefaultOutputDeviceID >= 0)
        {
            buffer->name = OutputDevices[DefaultOutputDeviceID].name;
            buffer->id = DefaultOutputDeviceID;
        }

        return true;
    }

    int GetDefaultInputDeviceID()
    {
        return DefaultInputDeviceID;
    }

    int GetDefaultOutputDeviceID()
    {
        return DefaultOutputDeviceID;
    }
    #pragma endregion
    
    #pragma region Private definitions
    bool ProbeDevicesIfNeeded()
    {
        if(!DevicesProbed)
        {
            return ProbeDevices();
        }
        return true;
    }
    #pragma endregion
}