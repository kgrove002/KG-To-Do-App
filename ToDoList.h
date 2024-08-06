#pragma once
#include <wx/wx.h>
#include <wx/checklst.h>
#include "CustomGauge.h"


class ToDoList : public wxScrolledWindow {
public:
    ToDoList(wxWindow* parent);

    void setDayOfWeek();
    void loadSavedTasks();
    void updateGauge();
private:
    wxString day;
    wxString tableName;
    wxCheckListBox* checkListBox;
    wxButton* checkAllButton;
    wxButton* unCheckAllButton;
    CustomGauge* progressBar;
    wxStaticText* heading;
    wxStaticText* progressText;

    std::vector<std::string> tasks;
    std::vector<bool> isChecked;

    void CreateControls();
    void CreateSizers();
    void SaveDataOnCheck(wxCommandEvent& event);
    void CheckAllItems(wxCommandEvent& event);
    void UnCheckAllItems(wxCommandEvent& event);
    void OnDoubleClick(wxCommandEvent& event);

};
