#pragma once
#include <wx/wx.h>
#include "TaskPanel.h"
#include "CustomGauge.h"

class GroceryList : public wxScrolledWindow
{
public:
    GroceryList(wxWindow* parent);
    void loadSavedTasks();

private:
    wxTextCtrl* inputField;
    wxStaticText* heading;
    wxButton* addButton;
    wxString day;
    std::string table;
    wxCheckListBox* checkListBox;
    wxButton* deleteButton;
    wxButton* clearButton;
    CustomGauge* progressBar;
    wxStaticText* progressText;

    std::vector<std::string> tasks;
    std::vector<bool> isChecked;

    void CreateControls();
    void CreateSizers();

    void AddNewTaskButton(wxCommandEvent& event);
    void AddNewTaskEnter(wxCommandEvent& event);
    void AddNewTask();
    void DeleteTask(wxCommandEvent& event);
    void ClearAllTasks(wxCommandEvent& event);
    void SaveDataOnCheck(wxCommandEvent& event);
    void updateGauge();
};

