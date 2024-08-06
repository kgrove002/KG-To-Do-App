#include "ToDoList.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include "DatabaseConnect.h"

// Constructor for ToDoList class
ToDoList::ToDoList(wxWindow* parent) : wxScrolledWindow(parent, wxID_ANY) {
    setDayOfWeek();         // Set the current day of the week and related table name
    CreateControls();       // Create UI controls
    CreateSizers();         // Create sizers to organize UI elements
    updateGauge();          // Update progress gauge based on loaded tasks
}

// Set the day of the week and related table name
void ToDoList::setDayOfWeek() {
    auto now = std::chrono::system_clock::now();        // Get current time
    auto now_local = std::chrono::system_clock::to_time_t(now);   // Convert to local time
    std::tm local_tm = *std::localtime(&now_local);      // Convert to tm structure

    int wday = local_tm.tm_wday;    // Get day of the week (0 = Sunday, 1 = Monday, ..., 6 = Saturday)
    switch (wday) {
    case 0: day = "Sunday"; break;   // Set day name based on day of the week
    case 1: day = "Monday"; break;
    case 2: day = "Tuesday"; break;
    case 3: day = "Wednesday"; break;
    case 4: day = "Thursday"; break;
    case 5: day = "Friday"; break;
    case 6: day = "Saturday"; break;
    default: day = "Unknown"; break;
    }

    tableName = day;  // Set the table name based on the day of the week
}

// Load tasks saved in the database for the current day
void ToDoList::loadSavedTasks() {
    checkListBox->Clear();  // Clear existing items in the checklist box

    // Create DatabaseConnect instance and connect to the database
    DatabaseConnect dbConnect;
    DatabaseConnect::Status status = dbConnect.connectToDataBase();

    // Check if database connection was successful
    if (status == DatabaseConnect::SUCCESS)
    {
        // Load tasks and their checked status from the database
        auto [loadedTasks, loadedIsChecked] = dbConnect.LoadItemsFromdb(day.ToStdString());

        // Populate the checklist box with loaded tasks and synchronize task and isChecked vectors
        tasks.clear();
        isChecked.clear();
        for (size_t i = 0; i < loadedTasks.size(); ++i) {
            checkListBox->Append(loadedTasks[i]);    // Add task to checklist box
            tasks.push_back(loadedTasks[i]);         // Add task to tasks vector
            isChecked.push_back(loadedIsChecked[i]); // Add checked status to isChecked vector

            // Set the checked state based on the loaded data
            if (loadedIsChecked[i]) {
                checkListBox->Check(i, true);    // Check the item in UI if it was checked in the database
            }
        }

        updateGauge();  // Update progress gauge based on loaded tasks
    }
    else {
        // Display error message if database connection failed
        wxMessageBox("There was a connection error. Please try again!", "Error", wxOK | wxICON_INFORMATION);
    }
}

// Create UI controls for the ToDoList window
void ToDoList::CreateControls() {
    heading = new wxStaticText(this, wxID_ANY, day, wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER); // Heading showing current day
    progressBar = new CustomGauge(this, 100, 0, wxDefaultPosition, wxSize(200, 30));  // Custom progress bar
    progressText = new wxStaticText(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);  // Text showing progress
    checkListBox = new wxCheckListBox(this, wxID_ANY);   // Checklist box for tasks
    checkAllButton = new wxButton(this, wxID_ANY, "Check All Tasks");     // Button to check all tasks
    unCheckAllButton = new wxButton(this, wxID_ANY, "UnCheck All Tasks"); // Button to uncheck all tasks

    heading->SetFont(wxFontInfo(wxSize(0, 48)));  // Set font for heading
    checkListBox->SetFont(wxFontInfo(wxSize(0, 36)));  // Set font for checklist box items

    checkAllButton->SetBackgroundColour(*wxGREEN);   // Set background color for Check All button
    unCheckAllButton->SetBackgroundColour(*wxRED);   // Set background color for UnCheck All button

    // Bind events for UI controls
    checkListBox->Bind(wxEVT_CHECKLISTBOX, &ToDoList::SaveDataOnCheck, this);   // Save data when checkbox is checked/unchecked
    checkListBox->Bind(wxEVT_LISTBOX_DCLICK, &ToDoList::OnDoubleClick, this);  // Handle double-click on checklist item
    checkAllButton->Bind(wxEVT_BUTTON, &ToDoList::CheckAllItems, this);        // Check all tasks button event
    unCheckAllButton->Bind(wxEVT_BUTTON, &ToDoList::UnCheckAllItems, this);    // Uncheck all tasks button event
}

// Create sizers to organize UI elements in the ToDoList window
void ToDoList::CreateSizers() {
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL); // Vertical box sizer for main layout
    sizer->Add(heading, 0, wxEXPAND | wxALL, 5);    // Add heading with padding
    sizer->Add(progressBar, 0, wxEXPAND | wxALL, 10);  // Add progress bar with padding
    sizer->Add(progressText, wxSizerFlags().CenterHorizontal());  // Add progress text centered horizontally
    sizer->AddSpacer(10);   // Spacer between heading and checklist box
    sizer->Add(checkListBox, 1, wxEXPAND | wxALL, 5);   // Add checklist box with padding

    sizer->AddSpacer(25);   // Spacer between checklist box and buttons

    wxFlexGridSizer* buttonSizer = new wxFlexGridSizer(1, 2, 0, 100); // Flex grid sizer for buttons
    buttonSizer->AddGrowableCol(0, 1);  // Growable column for first button
    buttonSizer->AddGrowableCol(1, 1);  // Growable column for second button
    buttonSizer->Add(checkAllButton, 1, wxEXPAND);   // Add Check All button
    buttonSizer->Add(unCheckAllButton, 1, wxEXPAND); // Add UnCheck All button
    sizer->Add(buttonSizer, wxSizerFlags().CenterHorizontal()); // Add button sizer centered horizontally

    sizer->AddSpacer(25);   // Spacer below buttons

    this->SetSizer(sizer);  // Set main sizer for the window

    SetScrollRate(5, 5);    // Set scrolling rate
    FitInside();            // Ensure scrolled window fits its children
}

// Save data when a checkbox is checked/unchecked
void ToDoList::SaveDataOnCheck(wxCommandEvent& event) {
    DatabaseConnect dbConnect;  // Create DatabaseConnect instance
    DatabaseConnect::Status status = dbConnect.connectToDataBase();  // Connect to database

    // Check if database connection was successful
    if (status == DatabaseConnect::SUCCESS)
    {
        int index = event.GetInt(); // Get index of checked item
        if (index != wxNOT_FOUND) {
            std::string value = checkListBox->GetString(index).ToStdString();  // Get task text
            bool isChecked = checkListBox->IsChecked(index);   // Check if item is checked
            dbConnect.updateOnCheck(tableName.ToStdString(), value, isChecked);  // Update database with checked state
        }
        updateGauge();  // Update progress gauge
        event.Skip();   // Allow event to propagate
    }
    else {
        // Display error message if database connection failed
        wxMessageBox("There was a connection error. Please try again!", "Error", wxOK | wxICON_INFORMATION);
    }
}

// Check all tasks in the checklist box
void ToDoList::CheckAllItems(wxCommandEvent& event) {
    DatabaseConnect dbConnect;  // Create DatabaseConnect instance
    DatabaseConnect::Status status = dbConnect.connectToDataBase();  // Connect to database

    // Check if database connection was successful
    if (status == DatabaseConnect::SUCCESS)
    {
        for (size_t i = 0; i < checkListBox->GetCount(); i++) {
            checkListBox->Check(i, true);   // Check all items in checklist box
        }
        DatabaseConnect::SetAllItemsToTrue(tableName.ToStdString());  // Update database to set all items as checked
        updateGauge();  // Update progress gauge
        event.Skip();   // Allow event to propagate
    }
    else {
        // Display error message if database connection failed
        wxMessageBox("There was a connection error. Please try again!", "Error", wxOK | wxICON_INFORMATION);
    }
}

// Uncheck all tasks in the checklist box
void ToDoList::UnCheckAllItems(wxCommandEvent& event) {
    DatabaseConnect dbConnect;  // Create DatabaseConnect instance
    DatabaseConnect::Status status = dbConnect.connectToDataBase();  // Connect to database

    // Check if database connection was successful
    if (status == DatabaseConnect::SUCCESS)
    {
        for (size_t i = 0; i < checkListBox->GetCount(); i++) {
            checkListBox->Check(i, false);  // Uncheck all items in checklist box
        }
        DatabaseConnect::SetAllItemsToFalse(tableName.ToStdString()); // Update database to set all items as unchecked
        updateGauge();  // Update progress gauge
        event.Skip();   // Allow event to propagate
    }
    else {
        // Display error message if database connection failed
        wxMessageBox("There was a connection error. Please try again!", "Error", wxOK | wxICON_INFORMATION);
    }
}

// Handle double-click event on checklist item
void ToDoList::OnDoubleClick(wxCommandEvent& event) {
    DatabaseConnect dbConnect;  // Create DatabaseConnect instance
    DatabaseConnect::Status status = dbConnect.connectToDataBase();  // Connect to database

    // Check if database connection was successful
    if (status == DatabaseConnect::SUCCESS)
    {
        int selection = checkListBox->GetSelection(); // Get index of double-clicked item
        if (selection != wxNOT_FOUND) {
            bool isChecked = checkListBox->IsChecked(selection);   // Check current state of item
            checkListBox->Check(selection, !isChecked);   // Toggle item's checked state in UI

            // Update database with the new checked state
            std::string value = checkListBox->GetString(selection).ToStdString();  // Get task text
            dbConnect.updateOnCheck(tableName.ToStdString(), value, !isChecked);   // Update database with new state
        }

        updateGauge();  // Update progress gauge
        event.Skip();   // Allow event to propagate
    }
    else {
        // Display error message if database connection failed
        wxMessageBox("There was a connection error. Please try again!", "Error", wxOK | wxICON_INFORMATION);
    }
}

// Update progress gauge based on checked tasks
void ToDoList::updateGauge() {
    int checked = 0;
    int percent = 0;

    // Count checked items in the checklist box
    for (size_t i = 0; i < checkListBox->GetCount(); i++) {
        if (checkListBox->IsChecked(i)) {
            checked += 1;
        }
    }

    // Calculate percentage of checked items
    if (checkListBox->GetCount() > 0) {
        percent = static_cast<int>((static_cast<float>(checked) / checkListBox->GetCount()) * 100);
        progressBar->SetValue(percent);    // Set progress bar value
    }
    else {
        progressBar->SetValue(0);   // Set progress bar to zero if checklist box is empty
    }

    // Set progress text
    progressText->SetLabel(wxString::Format("You are %d%% done for today!", percent));

    // Set color of progress bar based on percentage
    if (percent == 100) {
        progressBar->SetColor(*wxGREEN);    // Green color for 100% progress
    }
    else if (percent <= 99 && percent >= 50) {
        progressBar->SetColor(*wxYELLOW);   // Yellow color for 50% to 99% progress
    }
    else {
        progressBar->SetColor(*wxRED);      // Red color for less than 50% progress
    }
}
