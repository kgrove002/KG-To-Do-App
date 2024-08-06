#include "ManageTasks.h"
#include "TaskPanel.h"
#include <wx/sizer.h>
#include "DatabaseConnect.h"

// Define static member variables
TaskPanel* ManageTasks::SundayPanel = nullptr;
TaskPanel* ManageTasks::MondayPanel = nullptr;
TaskPanel* ManageTasks::TuesdayPanel = nullptr;
TaskPanel* ManageTasks::WednesdayPanel = nullptr;
TaskPanel* ManageTasks::ThursdayPanel = nullptr;
TaskPanel* ManageTasks::FridayPanel = nullptr;
TaskPanel* ManageTasks::SaturdayPanel = nullptr;

// Constructor
ManageTasks::ManageTasks(wxWindow* parent) : wxPanel(parent, wxID_ANY) {
    CreateControls(); // Create UI controls (buttons, panels)
    CreateSizers();   // Create layout sizers
}

// Function to switch between different day panels
void ManageTasks::SwitchDays(int dayIndex)
{
    // Hide all panels
    for (size_t i = 0; i < panels.size(); i++) {
        panels[i]->Hide();
    }
    // Show the selected day's panel
    panels[dayIndex]->Show();
    containerPanel->Layout(); // Layout the container panel to fit the shown panel
}

// Function to create UI controls (buttons and panels)
void ManageTasks::CreateControls()
{
    // Create buttons for each day of the week
    SundayButton = new wxButton(this, wxID_ANY, "Sunday");
    MondayButton = new wxButton(this, wxID_ANY, "Monday");
    TuesdayButton = new wxButton(this, wxID_ANY, "Tuesday");
    WednesdayButton = new wxButton(this, wxID_ANY, "Wednesday");
    ThursdayButton = new wxButton(this, wxID_ANY, "Thursday");
    FridayButton = new wxButton(this, wxID_ANY, "Friday");
    SaturdayButton = new wxButton(this, wxID_ANY, "Saturday");

    // Container panel to hold task panels
    containerPanel = new wxPanel(this);

    // Create TaskPanel instances for each day
    SundayPanel = new TaskPanel(containerPanel, "Sunday Tasks", "Sunday");
    MondayPanel = new TaskPanel(containerPanel, "Monday Tasks", "Monday");
    TuesdayPanel = new TaskPanel(containerPanel, "Tuesday Tasks", "Tuesday");
    WednesdayPanel = new TaskPanel(containerPanel, "Wednesday Tasks", "Wednesday");
    ThursdayPanel = new TaskPanel(containerPanel, "Thursday Tasks", "Thursday");
    FridayPanel = new TaskPanel(containerPanel, "Friday Tasks", "Friday");
    SaturdayPanel = new TaskPanel(containerPanel, "Saturday Tasks", "Saturday");

    // Hide all panels except the first one (SundayPanel)
    MondayPanel->Hide();
    TuesdayPanel->Hide();
    WednesdayPanel->Hide();
    ThursdayPanel->Hide();
    FridayPanel->Hide();
    SaturdayPanel->Hide();

    // Store all panels in an array for easier access
    panels = { SundayPanel, MondayPanel, TuesdayPanel, WednesdayPanel, ThursdayPanel, FridayPanel, SaturdayPanel };

    // Bind each button to SwitchDays function with lambda functions
    SundayButton->Bind(wxEVT_BUTTON, [=](wxCommandEvent&) { SwitchDays(0); SundayPanel->LoadTasksFromDatabase("Sunday"); });
    MondayButton->Bind(wxEVT_BUTTON, [=](wxCommandEvent&) { SwitchDays(1); MondayPanel->LoadTasksFromDatabase("Monday"); });
    TuesdayButton->Bind(wxEVT_BUTTON, [=](wxCommandEvent&) { SwitchDays(2); TuesdayPanel->LoadTasksFromDatabase("Tuesday"); });
    WednesdayButton->Bind(wxEVT_BUTTON, [=](wxCommandEvent&) { SwitchDays(3); WednesdayPanel->LoadTasksFromDatabase("Wednesday"); });
    ThursdayButton->Bind(wxEVT_BUTTON, [=](wxCommandEvent&) { SwitchDays(4); ThursdayPanel->LoadTasksFromDatabase("Thursday"); });
    FridayButton->Bind(wxEVT_BUTTON, [=](wxCommandEvent&) { SwitchDays(5); FridayPanel->LoadTasksFromDatabase("Friday"); });
    SaturdayButton->Bind(wxEVT_BUTTON, [=](wxCommandEvent&) { SwitchDays(6); SaturdayPanel->LoadTasksFromDatabase("Saturday"); });
}

// Function to create sizers for layout management
void ManageTasks::CreateSizers()
{
    buttons = { SundayButton, MondayButton, TuesdayButton, WednesdayButton, ThursdayButton, FridayButton, SaturdayButton };

    // Create a grid sizer for the buttons with 1 row and 7 columns
    wxGridSizer* gridSizer = new wxGridSizer(1, 7, 0, 0);

    // Add buttons to the grid sizer with spacing and styling
    for (int i = 0; i < buttons.size(); i++) {
        gridSizer->Add(buttons[i], wxSizerFlags(1).Border(wxALL, 10).Expand());
        buttons[i]->SetBackgroundColour(*wxBLUE);     // Set background color of buttons
        buttons[i]->SetForegroundColour(*wxWHITE);    // Set foreground (text) color of buttons

        // Bind mouse hover and exit events for button color change
        buttons[i]->Bind(wxEVT_ENTER_WINDOW, &ManageTasks::mouseHover, this);
        buttons[i]->Bind(wxEVT_LEAVE_WINDOW, &ManageTasks::mouseExit, this);
    }

    // Create a vertical box sizer for the task panels
    wxBoxSizer* dayPanelSizer = new wxBoxSizer(wxVERTICAL);
    // Add each day's panel to the dayPanelSizer with border and expansion
    dayPanelSizer->Add(SundayPanel, wxSizerFlags(1).Expand().Border(wxALL, 10));
    dayPanelSizer->Add(MondayPanel, wxSizerFlags(1).Expand().Border(wxALL, 10));
    dayPanelSizer->Add(TuesdayPanel, wxSizerFlags(1).Expand().Border(wxALL, 10));
    dayPanelSizer->Add(WednesdayPanel, wxSizerFlags(1).Expand().Border(wxALL, 10));
    dayPanelSizer->Add(ThursdayPanel, wxSizerFlags(1).Expand().Border(wxALL, 10));
    dayPanelSizer->Add(FridayPanel, wxSizerFlags(1).Expand().Border(wxALL, 10));
    dayPanelSizer->Add(SaturdayPanel, wxSizerFlags(1).Expand().Border(wxALL, 10));

    containerPanel->SetSizer(dayPanelSizer); // Set the sizer for the container panel

    // Create a vertical box sizer for the ManageTasks panel
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(gridSizer, wxSizerFlags().Expand()); // Add grid sizer for buttons
    mainSizer->Add(containerPanel, wxSizerFlags(1).Expand().Border(wxALL, 10)); // Add container panel with border

    SetSizer(mainSizer);     // Set the main sizer for ManageTasks panel
    mainSizer->SetSizeHints(this); // Set size hints to manage panel sizing
}

// Event handler for mouse hover over buttons
void ManageTasks::mouseHover(wxMouseEvent& event)
{
    wxButton* hoveredButton = dynamic_cast<wxButton*>(event.GetEventObject());
    if (hoveredButton)
    {
        hoveredButton->SetForegroundColour(*wxBLACK);   // Change text color on hover
        hoveredButton->Refresh();   // Refresh button appearance
    }
    event.Skip();
}

// Event handler for mouse exit from buttons
void ManageTasks::mouseExit(wxMouseEvent& event)
{
    wxButton* hoveredButton = dynamic_cast<wxButton*>(event.GetEventObject());
    if (hoveredButton)
    {
        hoveredButton->SetForegroundColour(*wxWHITE);   // Restore original text color on exit
        hoveredButton->Refresh();   // Refresh button appearance
    }
    event.Skip();
}

// Function to load tasks for all days from the database
void ManageTasks::LoadTasksForAllDays() {
    DatabaseConnect dbConnect;  // Create DatabaseConnect instance
    DatabaseConnect::Status status = dbConnect.connectToDataBase();  // Connect to database

    // Check if database connection was successful
    if (status == DatabaseConnect::SUCCESS) {
        // Load tasks for each day's panel from the database
        SundayPanel->LoadTasksFromDatabase("Sunday");
        MondayPanel->LoadTasksFromDatabase("Monday");
        TuesdayPanel->LoadTasksFromDatabase("Tuesday");
        WednesdayPanel->LoadTasksFromDatabase("Wednesday");
        ThursdayPanel->LoadTasksFromDatabase("Thursday");
        FridayPanel->LoadTasksFromDatabase("Friday");
        SaturdayPanel->LoadTasksFromDatabase("Saturday");
    }
    else {
        // Display error message if database connection failed
        wxMessageBox("Connection Failed. Please try again!", "Error", wxOK | wxICON_ERROR);
    }
}
