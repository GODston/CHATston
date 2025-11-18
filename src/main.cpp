#include <wx/wx.h>
#include <thread>
#include "Models/Audio/Recorder.h"

#pragma region Declarations
class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

class MyFrame : public wxFrame
{
public:
    MyFrame();

private:
    // Private attributes
    std::string SelectedInputDeviceIndex = "";
    std::string SelectedOutputDeviceIndex = "";

    // UI Elements
    wxPanel* panel_Main;
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
    //// Record button
    wxButton* recordBtn;
    

    // Functions
    void ProbeDevices(wxFrame* frame);

    // Events
    void OnRecordBtn(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnInputDeviceChange(wxCommandEvent& event);
    void OnOutputDeviceChange(wxCommandEvent& event);
};

enum
{
    ID_RecordButton,
    ID_InputDeviceCombo,
    ID_OutputDeviceCombo
};

wxIMPLEMENT_APP(MyApp);
#pragma endregion

#pragma region Implementations
bool MyApp::OnInit()
{
    MyFrame* frame = new MyFrame();
    frame->Show(true);
    return true;
}

MyFrame::MyFrame()
    : wxFrame(nullptr, wxID_ANY, "CHATston", wxDefaultPosition, wxDefaultSize, 
              wxDEFAULT_FRAME_STYLE & ~wxRESIZE_BORDER & ~wxMAXIMIZE_BOX)
{
    // Initialize controls
    panel_Main = new wxPanel(this);
    //// Input audio device selector
    panel_InputDevice = new wxPanel(panel_Main);
    label_InputDevice = new wxStaticText(panel_InputDevice, wxID_ANY, "Input Device:");
    combo_InputDevice = new wxComboBox(panel_InputDevice, ID_InputDeviceCombo, "Select Input...", wxDefaultPosition, wxSize(200, -1), 0, nullptr, wxCB_READONLY);
    //// Output audio device selector
    panel_OutputDevice = new wxPanel(panel_Main);
    label_OutputDevice = new wxStaticText(panel_OutputDevice, wxID_ANY, "Output Device:");
    combo_OutputDevice = new wxComboBox(panel_OutputDevice, ID_OutputDeviceCombo, "Select Output...", wxDefaultPosition, wxSize(200, -1), 0, nullptr, wxCB_READONLY);
    // Create UI elements and store references to ones you need later
    recordBtn = new wxButton(panel_Main, ID_RecordButton, "Record", wxDefaultPosition, wxSize(100, 30));
    
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
    sizer_Main->Add(recordBtn, 0, wxALL | wxCENTER, 10);
    
    panel_Main->SetSizer(sizer_Main);
    
    // Set minimum frame size
    wxSize minSize(500, 200);
    SetSize(minSize);
    SetMinSize(minSize);

    CreateStatusBar();
    
    ProbeDevices(this);

    Bind(wxEVT_BUTTON, &MyFrame::OnRecordBtn, this, ID_RecordButton);
    Bind(wxEVT_COMBOBOX, &MyFrame::OnInputDeviceChange, this, ID_InputDeviceCombo);
    Bind(wxEVT_COMBOBOX, &MyFrame::OnOutputDeviceChange, this, ID_OutputDeviceCombo);
}

void MyFrame::ProbeDevices(wxFrame* frame)
{
    // Run ProbeDevices in a separate thread
    std::thread deviceThread([this]() {
        SetStatusText("Probing audio devices...");
        Models::Audio::ProbeDevices();
        
        // Update UI on main thread
        wxTheApp->CallAfter([this, inputDevices = Models::Audio::InputDevices, outputDevices = Models::Audio::OutputDevices,
                defaultInputDevice = Models::Audio::DefaultInputDevice, defaultOutputDevice = Models::Audio::DefaultOutputDevice]() {
            // Clear existing items
            combo_InputDevice->Clear();
            combo_OutputDevice->Clear();
            
            // Populate input devices
            for (const auto& pair : inputDevices) {
                combo_InputDevice->Append(pair.first);
            }
            
            // Populate output devices
            for (const auto& pair : outputDevices) {
                combo_OutputDevice->Append(pair.first);
            }
            
            // Set default selections
            if (!defaultInputDevice.empty()) {
                SelectedInputDeviceIndex = defaultInputDevice;
                combo_InputDevice->SetSelection(combo_InputDevice->FindString(SelectedInputDeviceIndex));
            }
            if (!defaultOutputDevice.empty()) {
                SelectedOutputDeviceIndex = defaultOutputDevice;
                combo_OutputDevice->SetSelection(combo_OutputDevice->FindString(SelectedOutputDeviceIndex));
            }
            SetStatusText("Device probe completed.");
        });
    });
    
    // Detach the thread so it runs independently
    deviceThread.detach();
}

void MyFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}

void MyFrame::OnRecordBtn(wxCommandEvent& event)
{

}

void MyFrame::OnInputDeviceChange(wxCommandEvent& event)
{
    wxString selectedDevice = combo_InputDevice->GetStringSelection();
    SelectedInputDeviceIndex = selectedDevice.ToStdString();
}

void MyFrame::OnOutputDeviceChange(wxCommandEvent& event)
{
    wxString selectedDevice = combo_OutputDevice->GetStringSelection();
    SelectedOutputDeviceIndex = selectedDevice.ToStdString();
}
#pragma endregion