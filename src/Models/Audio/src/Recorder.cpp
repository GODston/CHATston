#include "../headers/Audio_Helper.h"
#include <iostream>

namespace Models::Audio
{
    // Recorder class implementations
    Recorder::Recorder(int sourceID, unsigned int deviceID)
        : sourceID(sourceID), recordingDeviceID(deviceID), recording(false) {}

    Recorder::~Recorder()
    {
        // Destructor - ensure recording is stopped
        stopRecording();
    }

    bool Recorder::startRecording()
    {
        if (recording) {
            return false; // Already recording
        }
        
        try {
            RtAudio::StreamParameters params;
            params.deviceId = recordingDeviceID;
            params.nChannels = channels;
            params.firstChannel = 0;
            
            unsigned int bufferSize = 256;
            
            audioInput.openStream(nullptr, &params, RTAUDIO_FLOAT32,
                           sampleRate, &bufferSize, &recordCallback, this);
            
            recordedData.clear();
            audioInput.startStream();
            recording = true;
            return true;
        }
        catch (...) {
            std::cerr << "RtAudio error starting recording" << std::endl;
            return false;
        }
    }

    void Recorder::stopRecording()
    {
        if (!recording) {
            return;
        }
        
        try {
            if (audioInput.isStreamRunning()) {
                audioInput.stopStream();
            }
            if (audioInput.isStreamOpen()) {
                audioInput.closeStream();
            }
            recording = false;
        }
        catch (...) {
            //use ui library to show error;
        }
    }

    void Recorder::ChangeRecordingDevice(int deviceID)
    {
        recordingDeviceID = deviceID;
        // Additional logic to handle device change during recording can be added here
    }

    int Recorder::recordCallback(void* outputBuffer, void* inputBuffer, unsigned int nBufferFrames,
                                  double streamTime, unsigned int status, void* userData)
    {
        Recorder* recorder = static_cast<Recorder*>(userData);
        float* buffer = static_cast<float*>(inputBuffer);
        
        if (buffer && recorder) {
            // Send to global mixer
            MixAudio(recorder->sourceID, buffer, nBufferFrames, recorder->sequenceNumber++);
            
            // Optionally keep local copy
            size_t numSamples = nBufferFrames * channels;
            recorder->recordedData.insert(recorder->recordedData.end(), 
                                         buffer, buffer + numSamples);
        }
        
        return 0;
    }
}