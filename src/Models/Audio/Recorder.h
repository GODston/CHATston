#include <RtAudio.h>
#include <string>
#include <map>

namespace Models::Audio
{
    extern std::map<std::string, int> InputDevices;
    extern std::map<std::string, int> OutputDevices;
    extern std::string DefaultInputDevice;
    extern std::string DefaultOutputDevice;

    void ProbeDevices();
    
    class Recorder
    {
    public:
        Recorder();
        ~Recorder();

        bool startRecording(unsigned int sampleRate = 44100, unsigned int channels = 2);
        void stopRecording();

    private:
        RtAudio audio;
        static int recordCallback(void* outputBuffer, void* inputBuffer, unsigned int nBufferFrames,
                                  double streamTime, RtAudioStreamStatus status, void* userData);
    };
} // namespace Models::Audio