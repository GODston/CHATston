#pragma once
#include <vector>
#include <string>



namespace Models::Audio
{
    struct AudioDevice
    {
    public:
        std::string name;
        unsigned int id;
    };
    
    std::vector<AudioDevice> GetInputDevices();
    std::vector<AudioDevice> GetOutputDevices();
    bool GetDefaultInputDevice(AudioDevice* buffer);
    bool GetDefaultOutputDevice(AudioDevice* buffer);
    int GetDefaultInputDeviceID();
    int GetDefaultOutputDeviceID();
    
    class Recorder
    {
    public:
        Recorder();
        ~Recorder();

        bool startRecording(unsigned int sampleRate = 44100, unsigned int channels = 2);
        void stopRecording();

    private:
        static int recordCallback(void* outputBuffer, void* inputBuffer, unsigned int nBufferFrames,
                                  double streamTime, void* userData);
    };

    class Player
    {
        // Player class definition (to be implemented)
    };
} // namespace Models::Audio