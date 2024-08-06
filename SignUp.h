#pragma once
#include <wx/wx.h>
#include <string>

class SignUp : public wxPanel
{
private:
	void CreateSizers();
	void CreateControls();
	void handleSubmit(wxCommandEvent& event);
	void handleBack(wxCommandEvent& event);
	void OnPinInfoButton(wxCommandEvent& event);
	void OnPasswordInfoButton(wxCommandEvent& event);

	wxStaticText* headlineText;
	wxStaticText* firstNameLabel;
	wxTextCtrl* firstNameInput;
	wxStaticText* lastNameLabel;
	wxTextCtrl* lastNameInput;
	wxStaticText* emailLabel;
	wxTextCtrl* emailInput;
	wxStaticText* pinLabel;
	wxButton* pinInfoButton;
	wxTextCtrl* pinInput;
	wxStaticText* pinconfirmLabel;
	wxTextCtrl* pinconfirmInput;
	wxStaticText* passwordLabel;
	wxButton* passwordInfoButton;
	wxTextCtrl* passwordInput;
	wxStaticText* passwordconfirmLabel;
	wxTextCtrl* passwordconfirmInput;
	wxButton* submitButton;
	wxButton* backButton;

public:
	SignUp(wxWindow* parent);
};

