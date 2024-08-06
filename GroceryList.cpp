#include "GroceryList.h"
#include "MainFrame.h"
#include "DatabaseConnect.h"

// Constructor implementation for GroceryList
GroceryList::GroceryList(wxWindow* parent) : wxScrolledWindow(parent, wxID_ANY) {
    table = "Grocery_List"; // Set the table name

    CreateControls();  // Create UI controls
    CreateSizers();    // Create sizers to organize UI elements

    updateGauge();     // Update the progress gauge
}

// Method to load saved tasks from the database
void GroceryList::loadSavedTasks()
{
    checkListBox->Clear();  // Clear existing items in the checklist box

    // Create DatabaseConnect instance
    DatabaseConnect dbConnect;

    // Load items from the database
    auto [loadedTasks, loadedIsChecked] = dbConnect.LoadItemsFromdb(table);

    // Populate the list box with loaded tasks and synchronize tasks and isChecked vectors
    tasks.clear();
    isChecked.clear();
    for (size_t i = 0; i < loadedTasks.size(); ++i) {
        checkListBox->Append(loadedTasks[i]);
        tasks.push_back(loadedTasks[i]);
        isChecked.push_back(loadedIsChecked[i]);

        // Set the checked state based on the loaded data
        if (loadedIsChecked[i]) {
            checkListBox->Check(i, true);
        }
    }

    updateGauge(); // Update the progress gauge
}

// Method to create UI controls
void GroceryList::CreateControls()
{
    heading = new wxStaticText(this, wxID_ANY, "Grocery List", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
    progressText = new wxStaticText(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
    checkListBox = new wxCheckListBox(this, wxID_ANY);
    progressBar = new CustomGauge(this, 100, 0, wxDefaultPosition, wxSize(200, 30));
    inputField = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    addButton = new wxButton(this, wxID_ANY, "Add");
    clearButton = new wxButton(this, wxID_ANY, "Delete All Items");
    deleteButton = new wxButton(this, wxID_ANY, "Delete Selected Item");

    heading->SetFont(wxFontInfo(wxSize(0, 48))); // Set font size for heading
    checkListBox->SetFont(wxFontInfo(wxSize(0, 36))); // Set font size for checklist box

    // Bind events to corresponding methods
    checkListBox->Bind(wxEVT_CHECKLISTBOX, &GroceryList::SaveDataOnCheck, this);
    addButton->Bind(wxEVT_BUTTON, &GroceryList::AddNewTaskButton, this);
    deleteButton->Bind(wxEVT_BUTTON, &GroceryList::DeleteTask, this);
    clearButton->Bind(wxEVT_BUTTON, &GroceryList::ClearAllTasks, this);
    inputField->Bind(wxEVT_TEXT_ENTER, &GroceryList::AddNewTaskEnter, this);

    // Set background colors for buttons
    addButton->SetBackgroundColour(*wxGREEN);
    clearButton->SetBackgroundColour(*wxRED);
    deleteButton->SetBackgroundColour(*wxYELLOW);
}

// Method to create sizers for organizing UI elements
void GroceryList::CreateSizers()
{
    // Create the main vertical box sizer
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(heading, 0, wxALIGN_CENTER | wxALL, 5);  // Center the heading with 5 pixels of border

    // Create flex grid sizer for input field and Add button
    wxFlexGridSizer* inputSizer = new wxFlexGridSizer(1, 2, 0, 25);
    inputSizer->AddGrowableCol(0, 3);  // Growable column 0 with 3 parts
    inputSizer->AddGrowableCol(1, 1);  // Growable column 1 with 1 part
    inputSizer->Add(inputField, 1, wxEXPAND);
    inputSizer->Add(addButton, 1, wxEXPAND);
    sizer->Add(inputSizer, wxSizerFlags().Expand().Border(wxALL, 5)); // Expand inputSizer with 5 pixels of border

    sizer->Add(progressBar, 0, wxEXPAND | wxALL, 10); // Add progress bar with 10 pixels of border
    sizer->Add(progressText, wxSizerFlags().CenterHorizontal()); // Center the progress text

    sizer->Add(checkListBox, 1, wxEXPAND | wxALL, 5);  // Expand the checklist box with 5 pixels of border

    // Create flex grid sizer for Delete Selected and Delete All buttons
    wxFlexGridSizer* buttonSizer = new wxFlexGridSizer(1, 2, 0, 100);
    buttonSizer->AddGrowableCol(0, 1);  // Growable column 0 with 1 part
    buttonSizer->AddGrowableCol(1, 1);  // Growable column 1 with 1 part
    buttonSizer->Add(deleteButton, 1, wxEXPAND);
    buttonSizer->Add(clearButton, 1, wxEXPAND);
    sizer->Add(buttonSizer, wxSizerFlags().CenterHorizontal().Border(wxALL, 5)); // Center the buttonSizer with 5 pixels of border

    // Set up scrolling for the window
    SetScrollRate(5, 5);
    FitInside();    // Ensure scrolled window fits its children

    this->SetSizer(sizer); // Set main sizer for the panel
}

// Method to handle Add button click event
void GroceryList::AddNewTaskButton(wxCommandEvent& event)
{
    AddNewTask(); // Call method to add new task
    event.Skip(); // Allow event to propagate
}

// Method to handle Enter key press event in input field
void GroceryList::AddNewTaskEnter(wxCommandEvent& event)
{
    AddNewTask(); // Call method to add new task
    event.Skip(); // Allow event to propagate
}

// Method to add a new task to the list box and database
void GroceryList::AddNewTask()
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
            checkListBox->Append(task);
            tasks.push_back(task.ToStdString());  // Push the task to the tasks vector
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

// Method to handle Delete Selected Item button click event
void GroceryList::DeleteTask(wxCommandEvent& event)
{
    DatabaseConnect dbConnect;  // Create DatabaseConnect instance
    DatabaseConnect::Status status = dbConnect.connectToDataBase();  // Connect to database

    // Check if database connection was successful
    if (status == DatabaseConnect::SUCCESS)
    {
        int selectedIndex = checkListBox->GetSelection(); // Get index of selected item

        if (selectedIndex == wxNOT_FOUND) {
            return; // Return if no item is selected
        }

        // Get the task description to identify the task in the database
        std::string taskDescription = tasks[selectedIndex];

        // Delete the selected task from the list box
        checkListBox->Delete(selectedIndex);
        tasks.erase(tasks.begin() + selectedIndex);  // Remove the task from the tasks vector
        isChecked.erase(isChecked.begin() + selectedIndex);  // Remove the checked state

        // Delete the task from the database
        DatabaseConnect::DeleteSelectedItemFromDB(taskDescription, table);

        updateGauge(); // Update the progress gauge
        event.Skip();  // Allow event to propagate
    }
    else {
        // Display error message if database connection failed
        wxMessageBox("There was a connection error. Please try again!", "Error", wxOK | wxICON_INFORMATION);
    }
}

// Method to handle Delete All Items button click event
void GroceryList::ClearAllTasks(wxCommandEvent& event)
{
    DatabaseConnect dbConnect;  // Create DatabaseConnect instance
    DatabaseConnect::Status status = dbConnect.connectToDataBase();  // Connect to database

    // Check if database connection was successful
    if (status == DatabaseConnect::SUCCESS)
    {
        if (checkListBox->IsEmpty()) {
            return; // Return if list box is already empty
        }

        // Show confirmation dialog
        wxMessageDialog dialog(this, "Are you sure you want to clear all tasks?", "Clear", wxYES_NO | wxCANCEL);
        int result = dialog.ShowModal();

        if (result == wxID_YES) {
            checkListBox->Clear(); // Clear all items from list box
            tasks.clear();  // Clear all tasks
            isChecked.clear();  // Clear all checked states
        }

        // Delete all items from the database table
        DatabaseConnect::DeleteAllItemsFromTable(table);

        updateGauge(); // Update the progress gauge
        event.Skip();  // Allow event to propagate
    }
    else {
        // Display error message if database connection failed
        wxMessageBox("There was a connection error. Please try again!", "Error", wxOK | wxICON_INFORMATION);
    }
}

// Method to handle item check/uncheck event in the checklist box
void GroceryList::SaveDataOnCheck(wxCommandEvent& event)
{
    DatabaseConnect dbConnect;  // Create DatabaseConnect instance
    DatabaseConnect::Status status = dbConnect.connectToDataBase();  // Connect to database

    // Check if database connection was successful
    if (status == DatabaseConnect::SUCCESS)
    {
        int index = event.GetInt(); // Get index of checked/unchecked item
        if (index != wxNOT_FOUND) {
            std::string value = checkListBox->GetString(index).ToStdString();
            bool isChecked = checkListBox->IsChecked(index);
            DatabaseConnect dbConnect;
            dbConnect.updateOnCheck(table, value, isChecked); // Update database with check state
        }
        updateGauge(); // Update the progress gauge
        event.Skip();  // Allow event to propagate
    }
    else {
        // Display error message if database connection failed
        wxMessageBox("There was a connection error. Please try again!", "Error", wxOK | wxICON_INFORMATION);
    }
}

// Method to update the progress gauge and text
void GroceryList::updateGauge() {
    int checked = 0;
    int percent = 0;

    // Count the number of checked items
    for (size_t i = 0; i < checkListBox->GetCount(); i++) {
        if (checkListBox->IsChecked(i)) {
            checked += 1;
        }
    }

    // Calculate percentage of checked items
    if (checkListBox->GetCount() > 0) {
        percent = static_cast<int>((static_cast<float>(checked) / checkListBox->GetCount()) * 100);
        progressBar->SetValue(percent);
    }
    else {
        progressBar->SetValue(0);
    }

    // Update progress text
    progressText->SetLabel(wxString::Format("You are %d%% done with your shopping!", percent));

    // Change progress bar color based on percentage
    if (percent == 100) {
        progressBar->SetColor(*wxGREEN);
    }
    else if (percent <= 99 && percent >= 50) {
        progressBar->SetColor(*wxYELLOW);
    }
    else {
        progressBar->SetColor(*wxRED);
    }
}
