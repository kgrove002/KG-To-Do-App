#include "MainFrame.h"

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
    // Set the application icon (ensure icon path is correct)
    wxIcon appIcon;
    if (appIcon.LoadFile("favicon.ico", wxBITMAP_TYPE_ICO)) {
        SetIcon(appIcon);
    }
    else {
        wxLogError("Failed to load icon: favicon.ico");
    }

    // Create and arrange controls
    CreateControls();
    SetUpSizers();

    // Initially show login panel only
    mainPanel->Hide();
    loginPanel->Show();

    // Set initial size hint (adjust as needed)
    SetMinSize(wxSize(1300, 600));
    SetSize(wxSize(1300, 600)); // Set initial size
}

void MainFrame::CreateControls() {
    wxFont mainFont(wxFontInfo(wxSize(0, 24)));

    // Main Panel
    mainPanel = new wxPanel(this);
    mainPanel->SetFont(mainFont);
    mainPanel->SetBackgroundColour(*wxLIGHT_GREY);

    // Buttons
    toDoButton = new wxButton(mainPanel, wxID_ANY, "To Do");
    groceryListButton = new wxButton(mainPanel, wxID_ANY, "Grocery");
    toDoSettingsButton = new wxButton(mainPanel, wxID_ANY, "Manage Tasks");
    userSettingsButton = new wxButton(mainPanel, wxID_ANY, "User Settings");
    logoutButton = new wxButton(mainPanel, wxID_ANY, "Logout");

    std::vector<wxButton*> buttons = { toDoButton, groceryListButton, toDoSettingsButton, userSettingsButton, logoutButton };
    for (auto button : buttons) {
        button->SetBackgroundColour(*wxWHITE);
    }

    // Pages
    toDoList = new ToDoList(mainPanel);
    groceryList = new GroceryList(mainPanel);
    manageTasks = new ManageTasks(mainPanel);
    userSettings = new UserSettings(mainPanel);

    // Initially show only ToDoList
    toDoList->Show();
    groceryList->Hide();
    manageTasks->Hide();
    userSettings->Hide();

    // Login Panel
    loginPanel = new wxPanel(this);
    loginPanel->SetFont(mainFont);

    headlineText = new wxStaticText(loginPanel, wxID_ANY, "Welcome Back\nTo The\nKG To Do App");
    emailLabel = new wxStaticText(loginPanel, wxID_ANY, "Email: ");
    emailInput = new wxTextCtrl(loginPanel, wxID_ANY, "", wxDefaultPosition, wxSize(300, -1)); // Adjust size here
    passwordLabel = new wxStaticText(loginPanel, wxID_ANY, "Password: ");
    passwordInput = new wxTextCtrl(loginPanel, wxID_ANY, "", wxDefaultPosition, wxSize(300, -1), wxTE_PROCESS_ENTER | wxTE_PASSWORD); // Adjust size here
    submitButton = new wxButton(loginPanel, wxID_ANY, "Submit");
    signupButton = new wxButton(loginPanel, wxID_ANY, "Sign Up");
    forgotPasswordButton = new wxButton(loginPanel, wxID_ANY, "Forgot Password");

    // Bind button events
    toDoButton->Bind(wxEVT_BUTTON, &MainFrame::OnToDoButtonClick, this);
    groceryListButton->Bind(wxEVT_BUTTON, &MainFrame::OnGroceryListButtonClick, this);
    toDoSettingsButton->Bind(wxEVT_BUTTON, &MainFrame::OnManageTasksButtonClick, this);
    userSettingsButton->Bind(wxEVT_BUTTON, &MainFrame::OnUserSettingsButtonClick, this);
    logoutButton->Bind(wxEVT_BUTTON, &MainFrame::OnLogoutButtonClick, this);
    submitButton->Bind(wxEVT_BUTTON, &MainFrame::OnLoginClick, this);
    passwordInput->Bind(wxEVT_TEXT_ENTER, &MainFrame::OnLoginEnter, this);
    signupButton->Bind(wxEVT_BUTTON, &MainFrame::OnSignupButtonClick, this);
    forgotPasswordButton->Bind(wxEVT_BUTTON, &MainFrame::OnForgotPasswordButtonClick, this);

    headlineText->SetFont(wxFontInfo(wxSize(0, 36)));
    headlineText->SetWindowStyle(wxALIGN_CENTER);

    signUp = new SignUp(this);
    forgotPassword = new ForgotPassword(this);

    signUp->Hide();
    forgotPassword->Hide();
}

void MainFrame::SetUpSizers() {
    // Create a sizer for the main frame
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // Main Panel Sizer
    wxBoxSizer* mainPanelSizer = new wxBoxSizer(wxVERTICAL);

    // Arrange buttons in a grid
    wxGridSizer* buttonGrid = new wxGridSizer(1, 5, 10, 10);
    buttonGrid->Add(toDoButton, wxSizerFlags().Proportion(1).Expand().Border(wxALL, 10));
    buttonGrid->Add(groceryListButton, wxSizerFlags().Proportion(1).Expand().Border(wxALL, 10));
    buttonGrid->Add(toDoSettingsButton, wxSizerFlags().Proportion(1).Expand().Border(wxALL, 10));
    buttonGrid->Add(userSettingsButton, wxSizerFlags().Proportion(1).Expand().Border(wxALL, 10));
    buttonGrid->Add(logoutButton, wxSizerFlags().Proportion(1).Expand().Border(wxALL, 10));

    // Add buttons and pages to the main panel sizer
    mainPanelSizer->Add(buttonGrid, wxSizerFlags().Center());
    mainPanelSizer->Add(toDoList, wxSizerFlags().Proportion(1).Expand().Border(wxALL, 10));
    mainPanelSizer->Add(groceryList, wxSizerFlags().Proportion(1).Expand().Border(wxALL, 10));
    mainPanelSizer->Add(manageTasks, wxSizerFlags().Proportion(1).Expand().Border(wxALL, 10));
    mainPanelSizer->Add(userSettings, wxSizerFlags().Proportion(1).Expand().Border(wxALL, 10));

    mainPanel->SetSizer(mainPanelSizer);

    // Login Panel Sizer
    wxBoxSizer* loginSizer = new wxBoxSizer(wxVERTICAL);

    // Add a spacer at the top
    loginSizer->AddSpacer(20);

    // Center the headline text
    wxBoxSizer* headlineSizer = new wxBoxSizer(wxHORIZONTAL);
    headlineSizer->AddStretchSpacer(1);
    headlineSizer->Add(headlineText, 0, wxALIGN_CENTER);
    headlineSizer->AddStretchSpacer(1);

    loginSizer->Add(headlineSizer, 0, wxEXPAND | wxALL, 10);

    // Create sizer for email and password fields
    wxFlexGridSizer* gridSizer = new wxFlexGridSizer(3, 2, 10, 10); // 3 rows, 2 columns, 10px gap

    // Add email label and input to gridSizer
    gridSizer->Add(emailLabel, 0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
    gridSizer->Add(emailInput, 1, wxEXPAND);

    // Add password label and input to gridSizer
    gridSizer->Add(passwordLabel, 0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
    gridSizer->Add(passwordInput, 1, wxEXPAND);

    // Add a spacer for the submit button
    gridSizer->AddSpacer(0);  // Add a spacer to align submit button with inputs

    // Add submit button spanning across two columns
    gridSizer->Add(submitButton, 1, wxEXPAND);
    // Make the second column growable
    gridSizer->AddGrowableCol(1, 1);

    // Add gridSizer to loginSizer
    loginSizer->Add(gridSizer, 1, wxALL | wxEXPAND, 10);

    // Create a sizer for additional buttons
    wxBoxSizer* additionalButtonSizer = new wxBoxSizer(wxHORIZONTAL);
    additionalButtonSizer->Add(signupButton, 0, wxALIGN_CENTER | wxALL, 5);
    additionalButtonSizer->Add(forgotPasswordButton, 0, wxALIGN_CENTER | wxALL, 5);

    loginSizer->Add(additionalButtonSizer, 0, wxALIGN_CENTER);

    // Add stretchable space to push everything to the center
    wxBoxSizer* centerSizer = new wxBoxSizer(wxVERTICAL);
    centerSizer->AddStretchSpacer(1);
    centerSizer->Add(loginSizer, 0, wxALIGN_CENTER);
    centerSizer->AddStretchSpacer(1);

    // Set the main sizer for the login panel
    loginPanel->SetSizer(centerSizer);

    // Add main panel and login panel to main sizer
    mainSizer->Add(mainPanel, wxSizerFlags().Proportion(1).Expand());
    mainSizer->Add(loginPanel, wxSizerFlags().Proportion(1).Expand());
    mainSizer->Add(signUp, wxSizerFlags().Proportion(1).Expand());
    mainSizer->Add(forgotPassword, wxSizerFlags().Proportion(1).Expand());

    this->SetSizer(mainSizer);
    mainSizer->SetSizeHints(this);
}

// Event handler for ToDo button click
void MainFrame::OnToDoButtonClick(wxCommandEvent& event) {
    toDoList->Show();
    groceryList->Hide();
    manageTasks->Hide();
    userSettings->Hide();
    toDoList->loadSavedTasks();
    mainPanel->Layout();
}

// Event handler for Grocery List button click
void MainFrame::OnGroceryListButtonClick(wxCommandEvent& event) {
    toDoList->Hide();
    groceryList->Show();
    manageTasks->Hide();
    userSettings->Hide();
    mainPanel->Layout();
}

// Event handler for Manage Tasks button click
void MainFrame::OnManageTasksButtonClick(wxCommandEvent& event) {
    toDoList->Hide();
    groceryList->Hide();
    manageTasks->Show();
    userSettings->Hide();
    mainPanel->Layout();
}

// Event handler for User Settings button click
void MainFrame::OnUserSettingsButtonClick(wxCommandEvent& event) {
    toDoList->Hide();
    groceryList->Hide();
    manageTasks->Hide();
    userSettings->Show();
    mainPanel->Layout();
}

// Event handler for Logout button click
void MainFrame::OnLogoutButtonClick(wxCommandEvent& event) {
    mainPanel->Hide();
    loginPanel->Show();
    signUp->Hide();
    Layout();
}

// Event handler for Sign Up button click
void MainFrame::OnSignupButtonClick(wxCommandEvent& event) {
    loginPanel->Hide();
    signUp->Show();
    signUp->GetParent()->Layout();
    Layout();
}

// Event handler for Forgot Password button click
void MainFrame::OnForgotPasswordButtonClick(wxCommandEvent& event) {
    loginPanel->Hide();
    forgotPassword->Show();
    forgotPassword->GetParent()->Layout();
    Layout();
}

// Event handler for Login button click
void MainFrame::OnLoginClick(wxCommandEvent& event) {
    OnLogin();
}

// Event handler for pressing Enter in the password field
void MainFrame::OnLoginEnter(wxCommandEvent& event) {
    OnLogin();
}

// Function to handle login
void MainFrame::OnLogin() {
    wxString email = emailInput->GetValue();
    wxString password = passwordInput->GetValue();

    DatabaseConnect dbConnect;
    DatabaseConnect::Status status = dbConnect.connectToDataBase();
    if (status == DatabaseConnect::SUCCESS) {
        if (!DatabaseConnect::verifyUser(email.ToStdString(), password.ToStdString())) {
            wxMessageBox("Your email or password was incorrect! Please try again!", "Error", wxOK | wxICON_INFORMATION);
        }
        else {
            DatabaseConnect::setupUserInfo(email.ToStdString(), password.ToStdString());
            toDoList->loadSavedTasks();
            groceryList->loadSavedTasks();
            manageTasks->LoadTasksForAllDays();
            userSettings->setUpData();
            userSettings->setCurrentEmailText();

            // Show main panel and hide login panel
            loginPanel->Hide();
            mainPanel->Show();

            // Show the appropriate initial panel (e.g., ToDoList)
            toDoList->Show();
            groceryList->Hide();
            manageTasks->Hide();
            userSettings->Hide();

            // Update frame layout to ensure proper sizing
            Layout();  // This should trigger a layout update

            emailInput->Clear();
            passwordInput->Clear();
        }
    }
    else {
        wxMessageBox("Connection Failed. Please try again!", "Error", wxOK | wxICON_ERROR);
    }
}
