#include "TaskPanel.h"
#include "DatabaseConnect.h"
#include <wx/wx.h>
#include <string>

// Constructor implementation
TaskPanel::TaskPanel(wxWindow* parent, const wxString& headline, const std::string& table) :
    wxScrolledWindow(parent), table(table)
{
    CreateControls(headline);   // Create UI controls with given headline
    CreateSizers();             // Create sizers to organize UI elements
}

// Method to create UI controls
void TaskPanel::CreateControls(const wxString headline)
{
    // Set the headline text
    headlineText = new wxStaticText(this, wxID_ANY, headline);
    headlineText->SetFont(wxFontInfo(wxSize(0, 48)));  // Set font for headline

    // Initialize member variables
    inputField = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    addButton = new wxButton(this, wxID_ANY, "Add");
    listBox = new wxListBox(this, wxID_ANY);
    clearButton = new wxButton(this, wxID_ANY, "Delete All Tasks");
    deleteButton = new wxButton(this, wxID_ANY, "Delete Selected Task");

    listBox->SetFont(wxFontInfo(wxSize(0, 36)));  // Set font for list box items

    addButton->SetBackgroundColour(*wxGREEN);   // Set background color for Add button
    clearButton->SetBackgroundColour(*wxRED);   // Set background color for Delete All button
    deleteButton->SetBackgroundColour(*wxYELLOW);  // Set background color for Delete Selected button

    // Bind events to corresponding methods
    addButton->Bind(wxEVT_BUTTON, &TaskPanel::AddNewTaskButton, this);
    deleteButton->Bind(wxEVT_BUTTON, &TaskPanel::DeleteTask, this);
    clearButton->Bind(wxEVT_BUTTON, &TaskPanel::ClearAllTasks, this);
    inputField->Bind(wxEVT_TEXT_ENTER, &TaskPanel::AddNewTaskEnter, this);
}

// Method to create sizers for organizing UI elements
void TaskPanel::CreateSizers()
{
    // Create the main vertical box sizer
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // Add headline text to the main sizer and center it horizontally
    mainSizer->Add(headlineText, wxSizerFlags().CenterHorizontal());
    mainSizer->AddSpacer(25); // Add space after headline

    // Create flex grid sizer for input field and Add button
    wxFlexGridSizer* inputSizer = new wxFlexGridSizer(1, 2, 0, 25);
    inputSizer->AddGrowableCol(0, 3);  // Growable column for input field
    inputSizer->AddGrowableCol(1, 1);  // Growable column for Add button
    inputSizer->Add(inputField, 1, wxEXPAND);   // Add input field to sizer
    inputSizer->Add(addButton, 1, wxEXPAND);    // Add Add button to sizer
    mainSizer->Add(inputSizer, wxSizerFlags().Expand()); // Add input sizer to main sizer

    mainSizer->AddSpacer(15); // Add space after input field and Add button

    // Add list box to the main sizer with expandable proportion
    mainSizer->Add(listBox, wxSizerFlags(1).Expand().Proportion(1));

    mainSizer->AddSpacer(25); // Add space after list box

    // Create flex grid sizer for Delete Selected and Delete All buttons
    wxFlexGridSizer* buttonSizer = new wxFlexGridSizer(1, 2, 0, 100);
    buttonSizer->AddGrowableCol(0, 1);  // Growable column for Delete Selected button
    buttonSizer->AddGrowableCol(1, 1);  // Growable column for Delete All button
    buttonSizer->Add(deleteButton, 1, wxEXPAND);   // Add Delete Selected button to sizer
    buttonSizer->Add(clearButton, 1, wxEXPAND);    // Add Delete All button to sizer
    mainSizer->Add(buttonSizer, wxSizerFlags().CenterHorizontal()); // Add button sizer centered horizontally

    SetScrollRate(5, 5);    // Set scrolling rate
    FitInside();            // Ensure scrolled window fits its children

    SetSizer(mainSizer);    // Set main sizer for the panel
    mainSizer->SetSizeHints(this);  // Set size hints for the panel
}

// Method to load tasks from the database and populate the list box
void TaskPanel::LoadTasksFromDatabase(const std::string& table)
{
    listBox->Clear();   // Clear existing items in list box

    // Create DatabaseConnect instance
    DatabaseConnect dbConnect;
    DatabaseConnect::Status status = dbConnect.connectToDataBase();  // Connect to database

    // Check if database connection was successful
    if (status == DatabaseConnect::SUCCESS)
    {
        // Load items from database
        auto [loadedTasks, loadedIsChecked] = dbConnect.LoadItemsFromdb(table);

        // Populate the list box with loaded tasks and synchronize tasks and isChecked vectors
        tasks.clear();
        isChecked.clear();
        for (size_t i = 0; i < loadedTasks.size(); ++i) {
            listBox->Append(loadedTasks[i]);
            tasks.push_back(loadedTasks[i]);
            isChecked.push_back(loadedIsChecked[i]);
        }
    }
    else {
        // Display error message if database connection failed
        wxMessageBox("There was a connection error. Please try again!", "Error", wxOK | wxICON_INFORMATION);
    }
}

// Method to handle Add button click event
void TaskPanel::AddNewTaskButton(wxCommandEvent& event)
{
    AddNewTask();   // Call method to add new task
    event.Skip();   // Allow event to propagate
}

// Method to handle Enter key press event in input field
void TaskPanel::AddNewTaskEnter(wxCommandEvent& event)
{
    AddNewTask();   // Call method to add new task
    event.Skip();   // Allow event to propagate
}

// Method to add a new task to the list box and database
void TaskPanel::AddNewTask()
{
    DatabaseConnect dbConnect;  // Create DatabaseConnect instance
    DatabaseConnect::Status status = dbConnect.connectToDataBase();  // Connect to database

    // Check if database connection was successful
    if (status == DatabaseConnect::SUCCESS)
    {
        // Get the task from the input field
        wxString task = inputField->GetValue();

        if (!task.IsEmpty()) {
            // Add the task to the list box
            listBox->Append(task);
            tasks.push_back(task.ToStdString());  // Add task to tasks vector
            isChecked.push_back(false);  // Default to unchecked

            // Clear the input field
            inputField->Clear();

            // Add the new task to the database
            DatabaseConnect::AddNewItemToDB(task.ToStdString(), table);
        }
    }
    else {
        // Display error message if database connection failed
        wxMessageBox("There was a connection error. Please try again!", "Error", wxOK | wxICON_INFORMATION);
    }
}

// Method to handle Delete Selected Task button click event
void TaskPanel::DeleteTask(wxCommandEvent& event)
{
    DatabaseConnect dbConnect;  // Create DatabaseConnect instance
    DatabaseConnect::Status status = dbConnect.connectToDataBase();  // Connect to database

    // Check if database connection was successful
    if (status == DatabaseConnect::SUCCESS)
    {
        int selectedIndex = listBox->GetSelection(); // Get index of selected item in list box

        if (selectedIndex == wxNOT_FOUND) {
            return; // Return if no item is selected
        }

        // Get the task description to identify the task in the database
        std::string taskDescription = tasks[selectedIndex];

        // Delete the selected task from the list box
        listBox->Delete(selectedIndex);
        tasks.erase(tasks.begin() + selectedIndex);  // Remove task from tasks vector
        isChecked.erase(isChecked.begin() + selectedIndex);  // Remove checked state from isChecked vector

        // Delete the task from the database
        DatabaseConnect::DeleteSelectedItemFromDB(taskDescription, table);

        event.Skip();   // Allow event to propagate
    }
    else {
        // Display error message if database connection failed
        wxMessageBox("There was a connection error. Please try again!", "Error", wxOK | wxICON_INFORMATION);
    }
}

// Method to handle Delete All Tasks button click event
void TaskPanel::ClearAllTasks(wxCommandEvent& event)
{
    DatabaseConnect dbConnect;  // Create DatabaseConnect instance
    DatabaseConnect::Status status = dbConnect.connectToDataBase();  // Connect to database

    // Check if database connection was successful
    if (status == DatabaseConnect::SUCCESS)
    {
        if (listBox->IsEmpty()) {
            return; // Return if list box is already empty
        }

        // Show confirmation dialog
        wxMessageDialog dialog(this, "Are you sure you want to clear all tasks?", "Clear", wxYES_NO | wxCANCEL);
        int result = dialog.ShowModal();

        if (result == wxID_YES) {
            listBox->Clear();   // Clear all items from list box
            tasks.clear();      // Clear all tasks from tasks vector
            isChecked.clear();  // Clear all checked states from isChecked vector
        }

        // Delete all items from the database table
        DatabaseConnect::DeleteAllItemsFromTable(table);

        event.Skip();   // Allow event to propagate
    }
    else {
        // Display error message if database connection failed
        wxMessageBox("There was a connection error. Please try again!", "Error", wxOK | wxICON_INFORMATION);
    }
}

// Method to handle mouse hover event for buttons
void TaskPanel::mouseHover(wxMouseEvent& event)
{
    wxButton* hoveredButton = dynamic_cast<wxButton*>(event.GetEventObject());
    if (hoveredButton)
    {
        hoveredButton->SetForegroundColour(*wxBLACK);  // Change foreground color on hover
        hoveredButton->Refresh();   // Refresh button appearance
    }
    event.Skip();   // Allow event to propagate
}

// Method to handle mouse exit event for buttons
void TaskPanel::mouseExit(wxMouseEvent& event)
{
    wxButton* hoveredButton = dynamic_cast<wxButton*>(event.GetEventObject());
    if (hoveredButton)
    {
        hoveredButton->SetForegroundColour(*wxWHITE);  // Restore foreground color on exit
        hoveredButton->Refresh();   // Refresh button appearance
    }
    event.Skip();   // Allow event to propagate
}
