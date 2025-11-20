#include "../headers/AudioDeviceSelectors.h"
#include "Models/Audio/headers/Audio.h"
#include "UI/headers/UserInterface.h"
#include <thread>

namespace Views::Controls::AudioDeviceSelectors
{
    wxFrameBase* parentFrame = nullptr;

    AudioDeviceSelectorsControl::AudioDeviceSelectorsControl(wxWindow* parent)
    {
        // Initialize controls
        parentFrame = dynamic_cast<wxFrameBase*>(parent);
        panel_Main = new wxPanel(parent);
        //// Input audio device selector
        panel_InputDevice = new wxPanel(panel_Main);
        label_InputDevice = new wxStaticText(panel_InputDevice, wxID_ANY, "Input Device:");
        combo_InputDevice = new wxComboBox(panel_InputDevice, ID_InputDeviceCombo, "Select Input...", wxDefaultPosition, wxSize(200, -1), 0, nullptr, wxCB_READONLY);
        //// Output audio device selector
        panel_OutputDevice = new wxPanel(panel_Main);
        label_OutputDevice = new wxStaticText(panel_OutputDevice, wxID_ANY, "Output Device:");
        combo_OutputDevice = new wxComboBox(panel_OutputDevice, ID_OutputDeviceCombo, "Select Output...", wxDefaultPosition, wxSize(200, -1), 0, nullptr, wxCB_READONLY);
        
        // Create sizers for layout
        sizer_Main = new wxBoxSizer(wxVERTICAL);
        sizer_InputDevice = new wxBoxSizer(wxHORIZONTAL);
        sizer_OutputDevice = new wxBoxSizer(wxHORIZONTAL);
        
        // Add elements with proper spacing and alignment
        sizer_InputDevice->Add(label_InputDevice, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
        sizer_InputDevice->Add(combo_InputDevice, 1, wxALL | wxEXPAND, 5);
        panel_InputDevice->SetSizer(sizer_InputDevice);
        
        sizer_OutputDevice->Add(label_OutputDevice, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
        sizer_OutputDevice->Add(combo_OutputDevice, 1, wxALL | wxEXPAND, 5);
        panel_OutputDevice->SetSizer(sizer_OutputDevice);

        sizer_Main->Add(panel_InputDevice, 0, wxALL | wxEXPAND, 5);
        sizer_Main->Add(panel_OutputDevice, 0, wxALL | wxEXPAND, 5);
        
        panel_Main->SetSizer(sizer_Main);

        panel_Main->Bind(wxEVT_COMBOBOX, &AudioDeviceSelectorsControl::OnInputDeviceChange, this, ID_InputDeviceCombo);
        panel_Main->Bind(wxEVT_COMBOBOX, &AudioDeviceSelectorsControl::OnOutputDeviceChange, this, ID_OutputDeviceCombo);
    }

    AudioDeviceSelectorsControl::~AudioDeviceSelectorsControl()
    {
        // Destructor implementation
    }

    void AudioDeviceSelectorsControl::ProbeDevices()
    {
        // Run ProbeDevices in a separate thread
        std::thread deviceThread([this]() {
            UI::SetStatusMessage("Probing audio devices...");
            Models::Audio::AudioDevice defaultInputDevice;
            Models::Audio::AudioDevice defaultOutputDevice;
            std::vector<Models::Audio::AudioDevice> inputDevices = Models::Audio::GetInputDevices();
            std::vector<Models::Audio::AudioDevice> outputDevices = Models::Audio::GetOutputDevices();
            if(!inputDevices.empty() && !outputDevices.empty())
            {
                bool foundDefaultInput = Models::Audio::GetDefaultInputDevice(&defaultInputDevice);
                bool foundDefaultOutput = Models::Audio::GetDefaultOutputDevice(&defaultOutputDevice);
                if(foundDefaultInput && foundDefaultOutput)
                {
                    wxTheApp->CallAfter([this, inputDevices, outputDevices, defaultInputDevice, defaultOutputDevice]() {
                    // Clear existing items
                    combo_InputDevice->Clear();
                    combo_OutputDevice->Clear();

                    // Populate input devices
                    for (const auto& device : inputDevices) {
                        int idx = combo_InputDevice->Append(device.name);
                        combo_InputDevice->SetClientData(idx, reinterpret_cast<void*>(static_cast<intptr_t>(device.id)));
                    }
                    
                    // Populate output devices
                    for (const auto& device : outputDevices) {
                        int idx = combo_OutputDevice->Append(device.name);
                        combo_OutputDevice->SetClientData(idx, reinterpret_cast<void*>(static_cast<intptr_t>(device.id)));
                    }
                    
                    // Set default selections
                    int inputIdx = combo_InputDevice->FindString(defaultInputDevice.name);
                    if (inputIdx != wxNOT_FOUND) {
                        combo_InputDevice->SetSelection(inputIdx);
                        SelectedInputDeviceID = defaultInputDevice.id;
                    }

                    int outputIdx = combo_OutputDevice->FindString(defaultOutputDevice.name);
                    if (outputIdx != wxNOT_FOUND) {
                        combo_OutputDevice->SetSelection(outputIdx);
                        SelectedOutputDeviceID = defaultOutputDevice.id;
                    }

                    UI::SetStatusMessage("Device search completed.");
                    });
                }
                else if(foundDefaultInput)
                {
                    wxTheApp->CallAfter([]() {
                        UI::SetStatusMessage("Failed to get default input device.");
                    });
                    return;
                }
                else if(foundDefaultOutput)
                {
                    wxTheApp->CallAfter([]() {
                        UI::SetStatusMessage("Failed to get default output device.");
                    });
                    return;
                }
                else
                {
                    wxTheApp->CallAfter([]() {
                        UI::SetStatusMessage("Failed to get default audio devices.");
                    });
                    return;
                }
            }
            else if(inputDevices.empty() && outputDevices.empty())
            {
                wxTheApp->CallAfter([]() {
                    UI::SetStatusMessage("No audio devices found.");
                });
                return;
            }
            else if(inputDevices.empty())
            {
                wxTheApp->CallAfter([]() {
                    UI::SetStatusMessage("No input audio devices found.");
                });
                return;
            }
            else if(outputDevices.empty())
            {
                wxTheApp->CallAfter([]() {
                    UI::SetStatusMessage("No output audio devices found.");
                });
                return;
            }
        });
    
        // Detach the thread so it runs independently
        deviceThread.detach();
    }

    void AudioDeviceSelectorsControl::OnInputDeviceChange(wxCommandEvent& event)
    {
        int selection = combo_InputDevice->GetSelection();
        if (selection != wxNOT_FOUND) {
            void* data = combo_InputDevice->GetClientData(selection);
            SelectedInputDeviceID = static_cast<int>(reinterpret_cast<intptr_t>(data));
        }
    }

    void AudioDeviceSelectorsControl::OnOutputDeviceChange(wxCommandEvent& event)
    {
        int selection = combo_OutputDevice->GetSelection();
        if (selection != wxNOT_FOUND) {
            void* data = combo_OutputDevice->GetClientData(selection);
            SelectedOutputDeviceID = static_cast<int>(reinterpret_cast<intptr_t>(data));
        }
    }

    int AudioDeviceSelectorsControl::GetSelectedInputDeviceID() const
    {
        return SelectedInputDeviceID;
    }

    int AudioDeviceSelectorsControl::GetSelectedOutputDeviceID() const
    {
        return SelectedOutputDeviceID;
    }
}