#include "ForgotPassword.h"
#include "MainFrame.h"
#include "DatabaseConnect.h"
#include "HandleEmail.h"
#include <curl/curl.h>

ForgotPassword::ForgotPassword(wxWindow* parent) : wxPanel(parent)
{
    CreateControls();
    CreateSizers();
}

void ForgotPassword::CreateControls()
{
    headlineText = new wxStaticText(this, wxID_ANY, "Forgot Your Password\nPlease enter your information");
    emailLabel = new wxStaticText(this, wxID_ANY, "Your Email");
    emailInput = new wxTextCtrl(this, wxID_ANY, "");
    pinLabel = new wxStaticText(this, wxID_ANY, "Your Pin");
    pinInput = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD | wxTE_PROCESS_ENTER);
    submitButton = new wxButton(this, wxID_ANY, "Submit");
    backButton = new wxButton(this, wxID_ANY, "Go Back");

    submitButton->Bind(wxEVT_BUTTON, &ForgotPassword::HandleSubmit, this);
    pinInput->Bind(wxEVT_TEXT_ENTER, &ForgotPassword::HandleSubmit, this);
    backButton->Bind(wxEVT_BUTTON, &ForgotPassword::HandleBack, this);

    wxFont mainFont(wxFontInfo(wxSize(0, 24)));

    headlineText->SetFont(wxFontInfo(wxSize(0, 36)));
    emailLabel->SetFont(mainFont);
    emailInput->SetFont(mainFont);
    pinLabel->SetFont(mainFont);
    pinInput->SetFont(mainFont);
    submitButton->SetFont(mainFont);
    backButton->SetFont(mainFont);
}

void ForgotPassword::CreateSizers()
{
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // Add headline text to the mainSizer
    mainSizer->Add(headlineText, wxSizerFlags().CenterHorizontal());
    mainSizer->AddSpacer(25);

    // Create the grid sizer for the form fields
    wxFlexGridSizer* formSizer = new wxFlexGridSizer(3, 2, 40, 20); // 3 rows, 2 columns, 10px gap

    // Add email label and input to formSizer
    formSizer->Add(emailLabel, 0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
    formSizer->Add(emailInput, 1, wxEXPAND);

    // Add PIN label and input to formSizer
    formSizer->Add(pinLabel, 0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
    formSizer->Add(pinInput, 1, wxEXPAND);

    // Add back button and submit button to formSizer
    formSizer->Add(backButton, 0, wxEXPAND);
    formSizer->Add(submitButton, 0, wxEXPAND);

    // Make the second column growable
    formSizer->AddGrowableCol(1, 1);

    // Add formSizer to mainSizer with padding
    mainSizer->Add(formSizer, 1, wxALL | wxEXPAND, 10);

    // Create centerSizer to center the mainSizer horizontally
    wxBoxSizer* centerSizer = new wxBoxSizer(wxHORIZONTAL);
    centerSizer->AddStretchSpacer(1);
    centerSizer->Add(mainSizer, 2, wxALIGN_CENTER | wxLEFT | wxRIGHT, 100);
    centerSizer->AddStretchSpacer(1);

    // Set the main sizer for the sign-up panel
    this->SetSizer(centerSizer);
    centerSizer->SetSizeHints(this);
}


void ForgotPassword::HandleSubmit(wxCommandEvent& event)
{
    wxString email = emailInput->GetValue();
    wxString pin = pinInput->GetValue();

    try {
        if (DatabaseConnect::getUserPassword(email.ToStdString(), pin.ToStdString())) {
            std::string gotFirstName = DatabaseConnect::getFirstName();
            std::string gotPIN = DatabaseConnect::getPin();
            std::string gotPassword = DatabaseConnect::getPassword();
            std::string gotEmail = DatabaseConnect::getEmail();

            if (HandleEmail::HandleForgotPassword(gotFirstName, gotPassword, gotEmail) == CURLE_OK) {
                wxMessageBox("Password found successfully.\nAn email has been sent to you with your account details.\nPlease check your junk folder as it can sometimes be determined as junk.\nPlease allow 5-15 minutes for your password to be sent to you.\nIf you do not receive one please email bqcritken@gmail.com with your email and pin so they may assist you.", "Confirmation", wxOK | wxICON_INFORMATION);
                emailInput->Clear();
                pinInput->Clear();
            }
            else {
                wxMessageBox("Password was found successfully, but the email failed to send.\nPlease try again later or email bqcritken@gmail.com with your information to retrieve your information.", "Error", wxOK | wxICON_ERROR);
            }
        }
        else {
            wxMessageBox("That email and PIN combination does not exist. Please try again!", "Error", wxOK | wxICON_ERROR);
        }
    }
    catch (const std::exception&) {
        wxMessageBox("There was an error. Please try again!", "Error", wxOK | wxICON_ERROR);
    }
}


void ForgotPassword::HandleBack(wxCommandEvent& event)
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
