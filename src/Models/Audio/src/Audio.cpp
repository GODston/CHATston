#include "../headers/Audio_Helper.h"

namespace Models::Audio
{
    bool DevicesProbed = false;
    RtAudio audio;
    std::map<int, RtAudio::DeviceInfo> InputDevices = {};
    std::map<int, RtAudio::DeviceInfo> OutputDevices = {};
    int DefaultInputDeviceID = -1;
    int DefaultOutputDeviceID = -1;

    #pragma region Private declarations
    bool ProbeDevicesIfNeeded();
    #pragma endregion

    #pragma region Public definitions
    bool ProbeDevices()
    {
        InputDevices.clear();
        OutputDevices.clear();
        DevicesProbed = false;

        std::vector<unsigned int> deviceIds = audio.getDeviceIds();
        
        for (unsigned int id : deviceIds)
        {
            RtAudio::DeviceInfo info = audio.getDeviceInfo(id);

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