#include "signUp.h"
#include "HandleEmail.h"
#include <curl/curl.h>
#include <wx/regex.h>
#include "MainFrame.h"

SignUp::SignUp(wxWindow* parent) : wxPanel(parent)
{
    CreateControls();
    CreateSizers();
}

void SignUp::CreateControls()
{
    headlineText = new wxStaticText(this, wxID_ANY, "Sign up for\nKG To Do APP!");
    firstNameLabel = new wxStaticText(this, wxID_ANY, "First Name:");
    firstNameInput = new wxTextCtrl(this, wxID_ANY, "");
    lastNameLabel = new wxStaticText(this, wxID_ANY, "Last Name:");
    lastNameInput = new wxTextCtrl(this, wxID_ANY, "");
    emailLabel = new wxStaticText(this, wxID_ANY, "Email:");
    emailInput = new wxTextCtrl(this, wxID_ANY, "");
    pinLabel = new wxStaticText(this, wxID_ANY, "PIN:");
    pinInfoButton = new wxButton(this, wxID_ANY, "Info");
    pinInput = new wxTextCtrl(this, wxID_ANY, "");
    pinconfirmLabel = new wxStaticText(this, wxID_ANY, "Comfirm PIN");
    pinconfirmInput = new wxTextCtrl(this, wxID_ANY, "");
    passwordLabel = new wxStaticText(this, wxID_ANY, "Password:");
    passwordInfoButton = new wxButton(this, wxID_ANY, "Info");
    passwordInput = new wxTextCtrl(this, wxID_ANY, "");
    passwordconfirmLabel = new wxStaticText(this, wxID_ANY, "Comfirm Password");
    passwordconfirmInput = new wxTextCtrl(this, wxID_ANY, "");
    submitButton = new wxButton(this, wxID_ANY, "SIGN UP!");
    backButton = new wxButton(this, wxID_ANY, "Go Back");

    headlineText->SetFont(wxFontInfo(wxSize(0, 36)));
    firstNameLabel->SetFont(wxFontInfo(wxSize(0, 24)));
    firstNameInput->SetFont(wxFontInfo(wxSize(0, 24)));
    lastNameLabel->SetFont(wxFontInfo(wxSize(0, 24)));
    lastNameInput->SetFont(wxFontInfo(wxSize(0, 24)));
    emailLabel->SetFont(wxFontInfo(wxSize(0, 24)));
    emailInput->SetFont(wxFontInfo(wxSize(0, 24)));
    pinLabel->SetFont(wxFontInfo(wxSize(0, 24)));
    pinInfoButton->SetFont(wxFontInfo(wxSize(0, 24)));
    pinInput->SetFont(wxFontInfo(wxSize(0, 24)));
    pinconfirmLabel->SetFont(wxFontInfo(wxSize(0, 24)));
    pinconfirmInput->SetFont(wxFontInfo(wxSize(0, 24)));
    passwordLabel->SetFont(wxFontInfo(wxSize(0, 24)));
    passwordInfoButton->SetFont(wxFontInfo(wxSize(0, 24)));
    passwordInput->SetFont(wxFontInfo(wxSize(0, 24)));
    passwordconfirmLabel->SetFont(wxFontInfo(wxSize(0, 24)));
    passwordconfirmInput->SetFont(wxFontInfo(wxSize(0, 24)));
    submitButton->SetFont(wxFontInfo(wxSize(0, 24)));
    backButton->SetFont(wxFontInfo(wxSize(0, 24)));

    submitButton->Bind(wxEVT_BUTTON, &SignUp::handleSubmit, this);
    backButton->Bind(wxEVT_BUTTON, &SignUp::handleBack, this);
    pinInfoButton->Bind(wxEVT_BUTTON, &SignUp::OnPinInfoButton, this);
    passwordInfoButton->Bind(wxEVT_BUTTON, &SignUp::OnPasswordInfoButton, this);

}

void SignUp::CreateSizers()
{
    wxBoxSizer* signupSizer = new wxBoxSizer(wxVERTICAL);

    // Create sizer for the form fields
    wxFlexGridSizer* gridSizer = new wxFlexGridSizer(15, 2, 10, 10); // 15 rows, 2 columns, 10px gap

    signupSizer->Add(headlineText, wxSizerFlags().CenterHorizontal());

    // Add first name label and input to gridSizer
    gridSizer->Add(firstNameLabel, 0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
    gridSizer->Add(firstNameInput, 1, wxEXPAND);

    // Add last name label and input to gridSizer
    gridSizer->Add(lastNameLabel, 0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
    gridSizer->Add(lastNameInput, 1, wxEXPAND);

    // Add email label and input to gridSizer
    gridSizer->Add(emailLabel, 0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
    gridSizer->Add(emailInput, 1, wxEXPAND);

    // Add PIN label and input to gridSizer
    gridSizer->Add(pinLabel, 0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
    wxBoxSizer* pinSizer = new wxBoxSizer(wxHORIZONTAL);
    pinSizer->Add(pinInput, 1, wxEXPAND);
    pinSizer->Add(pinInfoButton, 0, wxLEFT, 5);
    gridSizer->Add(pinSizer, 1, wxEXPAND);

    // Add confirm PIN input to gridSizer
    gridSizer->Add(pinconfirmLabel, 0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
    gridSizer->Add(pinconfirmInput, 1, wxEXPAND);

    // Add password label and input to gridSizer
    gridSizer->Add(passwordLabel, 0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
    wxBoxSizer* passwordSizer = new wxBoxSizer(wxHORIZONTAL);
    passwordSizer->Add(passwordInput, 1, wxEXPAND);
    passwordSizer->Add(passwordInfoButton, 0, wxLEFT, 5);
    gridSizer->Add(passwordSizer, 1, wxEXPAND);

    // Add confirm password input to gridSizer
    gridSizer->Add(passwordconfirmLabel, 0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
    gridSizer->Add(passwordconfirmInput, 1, wxEXPAND);

    // Add submit button spanning across two columns
    gridSizer->Add(backButton, -1, wxEXPAND);
    gridSizer->Add(submitButton, -1, wxEXPAND);

    // Make the second column growable
    gridSizer->AddGrowableCol(1, 1);

    // Add gridSizer to signupSizer
    signupSizer->Add(gridSizer, 1, wxALL | wxEXPAND, 10);

    // Add stretchable space to push everything to the center
    wxBoxSizer* centerSizer = new wxBoxSizer(wxHORIZONTAL);
    centerSizer->AddStretchSpacer(1);
    centerSizer->Add(signupSizer, 2, wxALIGN_CENTER);
    centerSizer->AddStretchSpacer(1);

    // Set the main sizer for the sign-up panel
    this->SetSizer(centerSizer);
    centerSizer->SetSizeHints(this);
}

void SignUp::handleSubmit(wxCommandEvent& event)
{
    wxString firstName = firstNameInput->GetValue();
    wxString lastName = lastNameInput->GetValue();
    wxString email = emailInput->GetValue();
    wxString pin = pinInput->GetValue();
    wxString confirmPin = pinconfirmInput->GetValue();
    wxString password = passwordInput->GetValue();
    wxString confirmPassword = passwordconfirmInput->GetValue();

    DatabaseConnect dbConnect;
    DatabaseConnect::Status status = dbConnect.connectToDataBase();

    if (status == DatabaseConnect::SUCCESS) {
        // Regular expression for validating email
        wxRegEx emailRegex("^[\\w\\.=-]+@[\\w\\.-]+\\.[a-zA-Z]{2,4}$");
        // Regular expression for validating password (at least 8 characters, 1 number)
        wxRegEx passwordRegex("^(?=.*\\d).{8,}$");

        if (!DatabaseConnect::checkForExistingEmail(email.ToStdString())) {
            wxMessageBox("Sorry, that email is already in use", "Error", wxOK | wxICON_ERROR);
        }
        else if (firstName.IsEmpty()) {
            wxMessageBox("Please enter your first name", "Error", wxOK | wxICON_ERROR);
        }
        else if (lastName.IsEmpty()) {
            wxMessageBox("Please enter your last name", "Error", wxOK | wxICON_ERROR);
        }
        else if (email.IsEmpty() || !emailRegex.Matches(email)) {
            wxMessageBox("Please enter a valid email address", "Error", wxOK | wxICON_ERROR);
        }
        else if (pin.IsEmpty() || pin.length() != 4 || !pin.IsNumber()) {
            wxMessageBox("PIN must be a 4-digit number", "Error", wxOK | wxICON_ERROR);
        }
        else if (pin != confirmPin) {
            wxMessageBox("Your PINs did not match", "Error", wxOK | wxICON_ERROR);
        }
        else if (password.IsEmpty() || !passwordRegex.Matches(password)) {
            wxMessageBox("Password must be at least 8 characters long and contain at least 1 number", "Error", wxOK | wxICON_ERROR);
        }
        else if (password != confirmPassword) {
            wxMessageBox("Your passwords did not match", "Error", wxOK | wxICON_ERROR);
        }
        else {
            try {
                DatabaseConnect::createNewUser(firstName.ToStdString(), lastName.ToStdString(), pin.ToStdString(), password.ToStdString(), email.ToStdString());

                // If all checks pass, proceed with the signup process
                if (HandleEmail::HandleSignup(firstName.ToStdString(), password.ToStdString(), pin.ToStdString(), email.ToStdString()) == CURLE_OK) {
                    wxMessageBox("Signup Successful.\nPlease keep your pin handy as it is the only way to recover your account.\nPlease enjoy the app.\nAn email has been sent to you with your account details.\nPlease check your junk folder as it can sometimes be determined as junk.\nIf you do not receive a welcome email and want one please email bqcritken@gmail.com for one.\nPlease allow up 5-15 minutes for your email to be sent to you.", "Confirmation", wxOK | wxICON_INFORMATION);
                }
                else {
                    wxMessageBox("Signup Successful, but email failed to send. If you would like your sign-up email, please email bqcritken@gmail.com for one.", "Error", wxOK | wxICON_ERROR);
                }
                firstNameInput->Clear();
                lastNameInput->Clear();
                emailInput->Clear();
                pinInput->Clear();
                pinconfirmInput->Clear();
                passwordInput->Clear();
                passwordconfirmInput->Clear();
            }
            catch (const std::exception& e) {
                wxMessageBox("Failed to create new user. Please try again!", "Error", wxOK | wxICON_ERROR);
            }
        }
    }
    else {
        wxMessageBox("Connection Failed. Please try again!", "Error", wxOK | wxICON_ERROR);
    }
}

void SignUp::handleBack(wxCommandEvent& event)
{
    // Hide the sign-up panel
    this->Hide();

    // Get the parent window (which should be the MainFrame)
    wxWindow* parent = GetParent();
    if (parent) {
        // Cast the parent to MainFrame and show the login panel
        MainFrame* mainFrame = dynamic_cast<MainFrame*>(parent);
        if (mainFrame) {
            mainFrame->GetLoginPanel()->Show();
            mainFrame->Layout();
        }
    }
}

void SignUp::OnPinInfoButton(wxCommandEvent& event)
{
    wxMessageBox("Your PIN must be a 4-digit number", "Information", wxOK | wxICON_INFORMATION);
}

void SignUp::OnPasswordInfoButton(wxCommandEvent& event)
{
    wxMessageBox("Your password must be at least 8 characters long and contain at least 1 number", "Information", wxOK | wxICON_INFORMATION);
}
