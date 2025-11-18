#include <wx/wx.h>
#include <thread>
#include "Views/Controls/AudioDeviceSelectors/headers/AudioDeviceSelectors.h"
#include "UI/headers/UserInterface.h"

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
    ~MyFrame();

private:
    // Private attributes

    // UI Elements
    wxPanel* panel_Main;
    wxBoxSizer* sizer_Main;

    //// Audio device selectors control
    Views::Controls::AudioDeviceSelectors::AudioDeviceSelectorsControl* audioDeviceSelectors;
    //// Record button
    wxButton* recordBtn;

    // Events
    void OnRecordBtn(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
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
    // Initialize UI system with this frame
    UI::Initialize(this);

    // Initialize controls
    panel_Main = new wxPanel(this);

    // Create audio device selectors control
    audioDeviceSelectors = new Views::Controls::AudioDeviceSelectors::AudioDeviceSelectorsControl(panel_Main);
    
    // Create UI elements and store references to ones you need later
    recordBtn = new wxButton(panel_Main, ID_RecordButton, "Record", wxDefaultPosition, wxSize(100, 30));
    
    // Create sizers for layout
    sizer_Main = new wxBoxSizer(wxVERTICAL);

    sizer_Main->Add(audioDeviceSelectors->panel_Main, 0, wxALL | wxEXPAND, 5);
    sizer_Main->Add(recordBtn, 0, wxALL | wxCENTER, 10);
    
    panel_Main->SetSizer(sizer_Main);
    
    // Set minimum frame size
    wxSize minSize(500, 200);
    SetSize(minSize);
    SetMinSize(minSize);

    CreateStatusBar();

    Bind(wxEVT_BUTTON, &MyFrame::OnRecordBtn, this, ID_RecordButton);

    // Load audio devices
    audioDeviceSelectors->ProbeDevices();
}

MyFrame::~MyFrame()
{
    // Delete non-wx custom objects
    delete audioDeviceSelectors;
}

void MyFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}

void MyFrame::OnRecordBtn(wxCommandEvent& event)
{

}
#pragma endregion