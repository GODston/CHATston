#pragma once
#include <wx/wx.h>
#include <string>

namespace UI
{
    // Initialize the UI system with the main frame reference
    void Initialize(wxFrame* mainFrame);
    
    // Status bar operations
    void SetStatusMessage(const std::string& message);
    
    // Dialog operations
    void ShowInfo(const std::string& title, const std::string& message);
    void ShowWarning(const std::string& title, const std::string& message);
    void ShowError(const std::string& title, const std::string& message);
    bool ShowConfirm(const std::string& title, const std::string& message);
    
    // Get the main frame (if you need direct access)
    wxFrame* GetMainFrame();
}