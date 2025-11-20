#include "../headers/Audio_Helper.h"
#include <iostream>

namespace Models::Audio
{
    // Player class implementations
    Player::Player(unsigned int deviceID)
        : playbackDeviceID(deviceID), playing(false) {}

    Player::~Player()
    {
        stopPlayback();
    }

    void Player::ChangePlaybackDevice(unsigned int deviceID)
    {
        playbackDeviceID = deviceID;
        // Additional logic to handle device change during playback can be added here
    }

    bool Player::startPlayback()
    {
        if (playing) {
            return false; // Already playing
        }
        
        try {
            RtAudio::StreamParameters params;
            params.deviceId = playbackDeviceID;
            params.nChannels = channels;
            params.firstChannel = 0;
            
            unsigned int bufferFrames = 256; // Buffer size
            
            audioOutput.openStream(&params, nullptr, RTAUDIO_FLOAT32,
                           sampleRate, &bufferFrames, &playCallback, this);
            
            audioOutput.startStream();
            playing = true;
            return true;
        }
        catch (...) {
            std::cerr << "RtAudio error starting playback" << std::endl;
            return false;
        }
    }

    void Player::stopPlayback()
    {
        if (!playing) {
            return;
        }
        
        try {
            if (audioOutput.isStreamRunning()) {
                audioOutput.stopStream();
            }
            if (audioOutput.isStreamOpen()) {
                audioOutput.closeStream();
            }
            playing = false;
        }
        catch (...) {
            // Handle error
        }
    }

    int Player::playCallback(void* outputBuffer, void* inputBuffer, unsigned int nBufferFrames,
                            double streamTime, unsigned int status, void* userData)
    {
        Player* player = static_cast<Player*>(userData);
        float* buffer = static_cast<float*>(outputBuffer);
        
        if (buffer && player) {
            // Get mixed audio from the global mixer
            size_t framesRead = GetMixedAudio(buffer, nBufferFrames);
            
            // If we didn't get enough frames, fill the rest with silence
            if (framesRead < nBufferFrames) {
                size_t samplesToFill = (nBufferFrames - framesRead) * channels;
                size_t startSample = framesRead * channels;
                for (size_t i = startSample; i < startSample + samplesToFill; ++i) {
                    buffer[i] = 0.0f;
                }
            }
        }
        
        return 0;
    }
}