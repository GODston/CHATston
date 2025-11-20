#pragma once
#include <wx/wx.h>

namespace Views::Controls::AudioDeviceSelectors
{
    class AudioDeviceSelectorsControl
    {
    public:
        wxPanel* panel_Main;

        AudioDeviceSelectorsControl(wxWindow* parent);
        ~AudioDeviceSelectorsControl();

        // Functions
        void ProbeDevices();
        int GetSelectedInputDeviceID() const;
        int GetSelectedOutputDeviceID() const;

    private:
        // Internal IDs for this control
        enum
        {
            ID_InputDeviceCombo = wxID_HIGHEST + 1,
            ID_OutputDeviceCombo
        };
        
        // Private attributes
        int SelectedInputDeviceID = -1;
        int SelectedOutputDeviceID = -1;

        // UI Elements
        wxBoxSizer* sizer_Main;
        //// Input audio device selector
        wxPanel* panel_InputDevice;
        wxBoxSizer* sizer_InputDevice;
        wxStaticText* label_InputDevice;
        wxComboBox* combo_InputDevice;
        //// Output audio device selector
        wxPanel* panel_OutputDevice;
        wxBoxSizer* sizer_OutputDevice;
        wxStaticText* label_OutputDevice;
        wxComboBox* combo_OutputDevice;

        // Event handlers
        void OnInputDeviceChange(wxCommandEvent& event);
        void OnOutputDeviceChange(wxCommandEvent& event);

    };
}