#pragma once
#include "wx/wx.h"

class ForgotPassword : public wxPanel
{
private:
	wxStaticText* headlineText;
	wxStaticText* emailLabel;
	wxTextCtrl* emailInput;
	wxStaticText* pinLabel;
	wxTextCtrl* pinInput;
	wxButton* submitButton;
	wxButton* backButton;

	void CreateControls();
	void CreateSizers();
	void HandleSubmit(wxCommandEvent& event);
	void HandleBack(wxCommandEvent& event);

public:
	ForgotPassword(wxWindow* parent);
};

