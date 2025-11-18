#include "Recorder.h"
#include <iostream>


namespace Models::Audio
{
    // Define the extern variables
    std::map<std::string, int> InputDevices = {};
    std::map<std::string, int> OutputDevices = {};
    std::string DefaultInputDevice = "";
    std::string DefaultOutputDevice = "";

    // Private attributes
    RtAudio audio;

    void ProbeDevices()
    {
        std::vector<unsigned int> deviceIds = audio.getDeviceIds();
        
        std::cout << "Found " << deviceIds.size() << " audio devices:\n" << std::endl;
        
        for (unsigned int id : deviceIds)
        {
            RtAudio::DeviceInfo info = audio.getDeviceInfo(id);

            if (Models::Audio::DefaultInputDevice.empty() && info.inputChannels > 0)
            {
                Models::Audio::InputDevices[info.name] = id;
                if(info.isDefaultInput)
                {
                    Models::Audio::DefaultInputDevice = info.name;
                }
            }
            if (Models::Audio::DefaultOutputDevice.empty() && info.outputChannels > 0)
            {
                Models::Audio::OutputDevices[info.name] = id;
                if(info.isDefaultOutput)
                {
                    Models::Audio::DefaultOutputDevice = info.name;
                }
            }
        }
    }

    // Recorder class implementations
    Recorder::Recorder()
    {
        // Constructor implementation
    }

    Recorder::~Recorder()
    {
        // Destructor - ensure recording is stopped
        if (audio.isStreamOpen())
        {
            stopRecording();
        }
    }

    bool Recorder::startRecording(unsigned int sampleRate, unsigned int channels)
    {
        // TODO: Implement recording start logic
        // This would configure and open the audio stream
        return false;
    }

    void Recorder::stopRecording()
    {
        // TODO: Implement recording stop logic
        if (audio.isStreamRunning())
        {
            audio.stopStream();
        }
        if (audio.isStreamOpen())
        {
            audio.closeStream();
        }
    }

    int Recorder::recordCallback(void* outputBuffer, void* inputBuffer, unsigned int nBufferFrames,
                                  double streamTime, RtAudioStreamStatus status, void* userData)
    {
        // TODO: Implement audio callback
        // This is called by RtAudio when audio data is available
        return 0;
    }
}