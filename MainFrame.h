#pragma once

#include <wx/wx.h>
#include <wx/scrolwin.h> // Include the header for wxScrolledWindow

#include "ToDoList.h"
#include "GroceryList.h"
#include "ManageTasks.h"
#include "UserSettings.h"
#include "DatabaseConnect.h"
#include "SignUp.h"
#include "ForgotPassword.h"


class MainFrame : public wxFrame {
public:
    MainFrame(const wxString& title);
    wxPanel* GetLoginPanel() const { return loginPanel; }

private:
    // Panels and controls
    wxPanel* mainPanel;
    wxPanel* loginPanel;

    // Buttons
    wxButton* toDoButton;
    wxButton* groceryListButton;
    wxButton* toDoSettingsButton;
    wxButton* userSettingsButton;
    wxButton* logoutButton;

    //login Controls
    wxStaticText* headlineText;
    wxStaticText* emailLabel;
    wxTextCtrl* emailInput;
    wxStaticText* passwordLabel;
    wxTextCtrl* passwordInput;
    wxButton* submitButton;
    wxButton* signupButton;
    wxButton* forgotPasswordButton;

    // Pages
    ToDoList* toDoList;
    GroceryList* groceryList;
    ManageTasks* manageTasks;
    UserSettings* userSettings;
    SignUp* signUp;
    ForgotPassword* forgotPassword;

    // Methods for creating controls and sizers
    void CreateControls();
    void SetUpSizers();

    // Event handlers for button clicks
    void OnLoginClick(wxCommandEvent& event);
    void OnLoginEnter(wxCommandEvent& event);
    void OnLogin();
    void OnToDoButtonClick(wxCommandEvent& event);
    void OnGroceryListButtonClick(wxCommandEvent& event);
    void OnManageTasksButtonClick(wxCommandEvent& event);
    void OnUserSettingsButtonClick(wxCommandEvent& event);
    void OnSignupButtonClick(wxCommandEvent& event);
    void OnForgotPasswordButtonClick(wxCommandEvent& event);
    void OnLogoutButtonClick(wxCommandEvent& event);
};
