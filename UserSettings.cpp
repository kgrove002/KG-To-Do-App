#include "UserSettings.h"
#include "DatabaseConnect.h"
#include "wx/regex.h"

// Define static member variables
std::string UserSettings::currentPassword;
std::string UserSettings::currentEmail;
std::string UserSettings::currentPin;
std::string UserSettings::firstName;

wxStaticText* UserSettings::headLineText = nullptr;
wxStaticText* UserSettings::currentEmailLabel = nullptr;

// Constructor
UserSettings::UserSettings(wxWindow* parent) : wxScrolledWindow(parent, wxID_ANY) {
    createControls();   // Create UI controls (labels, text controls, button)
    createSizer();      // Create layout sizer
    setUpData();        // Set up initial data from the database
}

// Function to create UI controls
void UserSettings::createControls()
{
    // Static text for greeting with user's first name
    headLineText = new wxStaticText(this, wxID_ANY, "Welcome Back " + firstName + "!");

    // Labels and text controls for current password and email
    currentPasswordLabel = new wxStaticText(this, wxID_ANY, "Please enter your current password:");
    currentPasswordEntry = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);

    currentEmailLabel = new wxStaticText(this, wxID_ANY, "");
    changeEmailEntry = new wxTextCtrl(this, wxID_ANY);

    // Labels, text controls, and confirmation for changing PIN
    changePinLabel = new wxStaticText(this, wxID_ANY, "If you would like to change your PIN, please enter it here.");
    changePin = new wxTextCtrl(this, wxID_ANY);
    confirmChangePinLabel = new wxStaticText(this, wxID_ANY, "Please confirm your new PIN");
    confirmChangePin = new wxTextCtrl(this, wxID_ANY);

    // Labels, text controls, and confirmation for changing password
    changePasswordLabel = new wxStaticText(this, wxID_ANY, "If you would like to change your password, please enter it here.");
    changePassword = new wxTextCtrl(this, wxID_ANY);
    confirmChangePasswordLabel = new wxStaticText(this, wxID_ANY, "Please confirm your new Password");
    confirmChangePassword = new wxTextCtrl(this, wxID_ANY);

    // Apply button for updating settings
    applyButton = new wxButton(this, wxID_ANY, "Apply New Settings");
    applyButton->SetBackgroundColour(*wxBLUE);
    applyButton->SetForegroundColour(*wxWHITE);

    // Set fonts for all UI elements
    headLineText->SetFont(wxFontInfo(wxSize(0, 48)));

    currentPasswordLabel->SetFont(wxFontInfo(wxSize(0, 18)));
    currentPasswordEntry->SetFont(wxFontInfo(wxSize(0, 18)));
    currentEmailLabel->SetFont(wxFontInfo(wxSize(0, 18)));
    changeEmailEntry->SetFont(wxFontInfo(wxSize(0, 18)));

    changePinLabel->SetFont(wxFontInfo(wxSize(0, 18)));
    changePin->SetFont(wxFontInfo(wxSize(0, 18)));
    confirmChangePinLabel->SetFont(wxFontInfo(wxSize(0, 18)));
    confirmChangePin->SetFont(wxFontInfo(wxSize(0, 18)));

    changePasswordLabel->SetFont(wxFontInfo(wxSize(0, 18)));
    changePassword->SetFont(wxFontInfo(wxSize(0, 18)));
    confirmChangePasswordLabel->SetFont(wxFontInfo(wxSize(0, 18)));
    confirmChangePassword->SetFont(wxFontInfo(wxSize(0, 18)));

    // Bind applyButton to updateSettings function
    applyButton->Bind(wxEVT_BUTTON, &UserSettings::updateSettings, this);

    // Set background color for the panel
    this->SetBackgroundColour(*wxWHITE);
}

// Function to create layout sizers
void UserSettings::createSizer()
{
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // Add headline text centered horizontally
    mainSizer->Add(headLineText, wxSizerFlags().CenterHorizontal());
    mainSizer->AddSpacer(30);   // Add space below headline

    // Vertical sizer for form elements
    wxBoxSizer* formSizer = new wxBoxSizer(wxVERTICAL);

    // Add current password label and entry
    formSizer->Add(currentPasswordLabel);
    formSizer->AddSpacer(10);
    formSizer->Add(currentPasswordEntry, wxSizerFlags().Expand());
    formSizer->AddSpacer(20);

    // Add current email label and entry for changing email
    formSizer->Add(currentEmailLabel);
    formSizer->AddSpacer(10);
    formSizer->Add(changeEmailEntry, wxSizerFlags().Expand());
    formSizer->AddSpacer(20);

    // Add change PIN label, entry, confirm PIN label, and confirm PIN entry
    formSizer->Add(changePinLabel);
    formSizer->AddSpacer(10);
    formSizer->Add(changePin, wxSizerFlags().Expand());
    formSizer->AddSpacer(10);
    formSizer->Add(confirmChangePinLabel);
    formSizer->AddSpacer(10);
    formSizer->Add(confirmChangePin, wxSizerFlags().Expand());
    formSizer->AddSpacer(20);

    // Add change password label, entry, confirm password label, and confirm password entry
    formSizer->Add(changePasswordLabel);
    formSizer->AddSpacer(10);
    formSizer->Add(changePassword, wxSizerFlags().Expand());
    formSizer->AddSpacer(10);
    formSizer->Add(confirmChangePasswordLabel);
    formSizer->AddSpacer(10);
    formSizer->Add(confirmChangePassword, wxSizerFlags().Expand());
    formSizer->AddSpacer(20);

    // Add applyButton to the formSizer
    formSizer->Add(applyButton);

    // Add formSizer to mainSizer with margins
    mainSizer->Add(formSizer, 1, wxLEFT | wxRIGHT | wxEXPAND, 200);
    SetSizer(mainSizer);    // Set mainSizer for the panel

    // Set up scrolling for the window
    SetScrollRate(5, 5);
    FitInside();    // Ensure scrolled window fits its children

    mainSizer->SetSizeHints(this);

    setCurrentEmailText();   // Set current email text in the label
}

// Function to update the current email text in the label
void UserSettings::setCurrentEmailText()
{
    // Set current email label text based on currentEmail
    if (currentEmailLabel) {
        currentEmailLabel->SetLabel(wxString::Format("Your current email is %s. Would you like to change it?", currentEmail));
    }
    // Set headline text with user's first name
    if (headLineText) {
        headLineText->SetLabel(wxString::Format("Welcome Back %s.", firstName));
    }
}

// Function to update data in the database
bool UserSettings::updateData()
{
    DatabaseConnect dbConnect;  // Create DatabaseConnect instance
    DatabaseConnect::Status status = dbConnect.connectToDataBase();  // Connect to database

    // Check if database connection was successful
    if (status == DatabaseConnect::SUCCESS)
    {
        // Update email if changeEmailEntry is not empty
        if (!changeEmailEntry->IsEmpty()) {
            currentEmail = changeEmailEntry->GetValue().ToStdString();
            DatabaseConnect::updateEmail(currentEmail);
            setCurrentEmailText();   // Update current email text in label
        }

        // Update PIN if changePin is not empty
        if (!changePin->IsEmpty()) {
            currentPin = changePin->GetValue().ToStdString();
            DatabaseConnect::updatePin(currentPin);
        }

        // Update password if changePassword is not empty
        if (!changePassword->IsEmpty()) {
            currentPassword = changePassword->GetValue().ToStdString();
            DatabaseConnect::updatePassword(currentPassword);
        }

        return true;    // Return true for successful update
    }
    else {
        // Display error message if database connection failed
        wxMessageBox("There was a connection error. Please try again!", "Error", wxOK | wxICON_INFORMATION);
        return false;   // Return false for unsuccessful update
    }
}

// Function to set up initial data from the database
void UserSettings::setUpData()
{
    // Get current password, email, PIN, and first name from the database
    currentPassword = DatabaseConnect::getPassword();
    currentEmail = DatabaseConnect::getEmail();
    currentPin = DatabaseConnect::getPin();
    firstName = DatabaseConnect::getFirstName();
}

// Function to handle update settings button click event
void UserSettings::updateSettings(wxCommandEvent& event)
{
    wxString passwordInput = currentPasswordEntry->GetValue();    // Get current password input
    wxString newEmail = changeEmailEntry->GetValue();             // Get new email input
    wxString newPin = changePin->GetValue();                     // Get new PIN input
    wxString confirmNewPin = confirmChangePin->GetValue();       // Get confirm new PIN input
    wxString newPassword = changePassword->GetValue();           // Get new password input
    wxString confirmNewPassword = confirmChangePassword->GetValue(); // Get confirm new password input

    // Regular expression for validating email format
    wxRegEx emailRegex("^[\\w\\.=-]+@[\\w\\.-]+\\.[a-zA-Z]{2,4}$");
    // Regular expression for validating password format (at least 8 characters, 1 number)
    wxRegEx passwordRegex("^(?=.*\\d).{8,}$");

    // Check if current password matches the entered password
    if (passwordInput == currentPassword) {
        // Check if new email is provided and valid
        if (!newEmail.IsEmpty() && !emailRegex.Matches(newEmail)) {
            wxMessageBox("Please enter a valid email address", "Error", wxOK | wxICON_ERROR);
            return;
        }

        // Check if new email already exists in the database
        if (!newEmail.IsEmpty() && !DatabaseConnect::checkForExistingEmail(newEmail.ToStdString())) {
            wxMessageBox("This email address is already in use", "Error", wxOK | wxICON_ERROR);
            return;
        }

        // Check if new PIN is provided and is a 4-digit number
        if (!newPin.IsEmpty()) {
            if (newPin.length() != 4 || !newPin.IsNumber()) {
                wxMessageBox("PIN must be a 4-digit number", "Error", wxOK | wxICON_ERROR);
                return;
            }

            // Check if new PIN matches the confirm PIN
            if (newPin != confirmNewPin) {
                wxMessageBox("Your pins did not match", "Error", wxOK | wxICON_ERROR);
                return;
            }
        }

        // Check if new password is provided and meets criteria
        if (!newPassword.IsEmpty()) {
            if (!passwordRegex.Matches(newPassword)) {
                wxMessageBox("Password must be at least 8 characters long and contain at least 1 number", "Error", wxOK | wxICON_ERROR);
                return;
            }

            // Check if new password matches the confirm password
            if (newPassword != confirmNewPassword) {
                wxMessageBox("Your passwords did not match", "Error", wxOK | wxICON_ERROR);
                return;
            }
        }

        // If all checks pass, update data in the database
        if (updateData()) {
            wxMessageBox("Update Successful", "Confirmation", wxOK | wxICON_INFORMATION);
            currentPasswordEntry->Clear();
            changeEmailEntry->Clear();
            changePin->Clear();
            confirmChangePin->Clear();
            changePassword->Clear();
            confirmChangePassword->Clear();
        }
    }
    else {
        // Display error message if current password is incorrect
        wxMessageBox("Current password is incorrect", "Error", wxOK | wxICON_ERROR);
    }
}
