#pragma once
#include <wx/wx.h>
#include <wx/scrolwin.h> // Include the header for wxScrolledWindow

class UserSettings : public wxScrolledWindow // Change inheritance to wxScrolledWindow
{
private:
    static wxStaticText* headLineText;

    wxStaticText* currentPasswordLabel;
    wxTextCtrl* currentPasswordEntry;

    static wxStaticText* currentEmailLabel;
    wxTextCtrl* changeEmailEntry;

    wxStaticText* changePinLabel;
    wxTextCtrl* changePin;
    wxStaticText* confirmChangePinLabel;
    wxTextCtrl* confirmChangePin;

    wxStaticText* changePasswordLabel;
    wxTextCtrl* changePassword;
    wxStaticText* confirmChangePasswordLabel;
    wxTextCtrl* confirmChangePassword;

    wxButton* applyButton;

    bool updateData();

    void updateSettings(wxCommandEvent& event);
    void createControls();
    void createSizer();

public:
    UserSettings(wxWindow* parent);

    static void setUpData();
    static void setCurrentEmailText();

    static std::string currentPassword;
    static std::string currentEmail;
    static std::string currentPin;
    static std::string firstName;
};