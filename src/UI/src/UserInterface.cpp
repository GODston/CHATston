#include "../headers/UserInterface.h"

namespace UI
{
    // Private: Store reference to main frame
    namespace
    {
        wxFrame* mainFrame = nullptr;
    }

    void Initialize(wxFrame* frame)
    {
        mainFrame = frame;
    }

    void SetStatusMessage(const std::string& message)
    {
        if (mainFrame && mainFrame->GetStatusBar())
        {
            mainFrame->SetStatusText(message);
        }
    }

    void ShowInfo(const std::string& title, const std::string& message)
    {
        wxMessageBox(message, title, wxOK | wxICON_INFORMATION, mainFrame);
    }

    void ShowWarning(const std::string& title, const std::string& message)
    {
        wxMessageBox(message, title, wxOK | wxICON_WARNING, mainFrame);
    }

    void ShowError(const std::string& title, const std::string& message)
    {
        wxMessageBox(message, title, wxOK | wxICON_ERROR, mainFrame);
    }

    bool ShowConfirm(const std::string& title, const std::string& message)
    {
        int result = wxMessageBox(message, title, wxYES_NO | wxICON_QUESTION, mainFrame);
        return result == wxYES;
    }

    wxFrame* GetMainFrame()
    {
        return mainFrame;
    }
}