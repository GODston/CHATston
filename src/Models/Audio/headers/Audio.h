#pragma once
#include <vector>
#include <string>
#include <functional>

namespace Models::Audio
{
    struct AudioDevice
    {
    public:
        std::string name;
        unsigned int id;
    };
    
    void ChangeSampleRate(unsigned int newSampleRate);
    void ChangeChannels(unsigned int newChannels);
    std::vector<AudioDevice> GetInputDevices();
    std::vector<AudioDevice> GetOutputDevices();
    bool GetDefaultInputDevice(AudioDevice* buffer);
    bool GetDefaultOutputDevice(AudioDevice* buffer);
    int GetDefaultInputDeviceID();
    int GetDefaultOutputDeviceID();
    
    class Recorder
    {
    public:
        Recorder(int sourceID, unsigned int deviceID);
        ~Recorder();

        void ChangeRecordingDevice(int deviceID);
        bool startRecording();
        void stopRecording();
        
        // Access individual recorder's data (optional - for debugging)
        const std::vector<float>& getRecordedData() const { return recordedData; }
        bool isRecording() const { return recording; }
        int getSourceID() const { return sourceID; }

    private:
        int sourceID;  // Unique identifier for this recorder
        unsigned int recordingDeviceID;
        bool recording;
        uint32_t sequenceNumber;
        std::vector<float> recordedData;  // Optional: keep local copy
        
        static int recordCallback(void* outputBuffer, void* inputBuffer, unsigned int nBufferFrames,
                                  double streamTime, unsigned int status, void* userData);
    };

    class Player
    {
    public:
        Player(unsigned int deviceID);
        ~Player();

        void ChangePlaybackDevice(unsigned int deviceID);
        bool startPlayback();
        void stopPlayback();
        bool isPlaying() const { return playing; }
        
    private:
        unsigned int playbackDeviceID;
        bool playing;
        
        static int playCallback(void* outputBuffer, void* inputBuffer, unsigned int nBufferFrames,
                               double streamTime, unsigned int status, void* userData);
    };
} // namespace Models::Audio