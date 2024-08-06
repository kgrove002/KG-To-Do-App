#pragma once
#include <wx/wx.h>
#include <wx/listbox.h>
#include <vector>
#include <string>

class TaskPanel : public wxScrolledWindow
{
private:
    wxStaticText* headlineText;
    wxTextCtrl* inputField;
    wxButton* addButton;
    wxListBox* listBox;
    wxButton* clearButton;
    wxButton* deleteButton;
    std::vector<std::string> tasks;
    std::vector<bool> isChecked;
    std::string table;

    void CreateControls(const wxString headline);
    void CreateSizers();
    void mouseHover(wxMouseEvent& event);
    void mouseExit(wxMouseEvent& event);
    void AddNewTask();
    void DeleteTask(wxCommandEvent& event);
    void ClearAllTasks(wxCommandEvent& event);
    void AddNewTaskButton(wxCommandEvent& event);
    void AddNewTaskEnter(wxCommandEvent& event);
public:
    TaskPanel(wxWindow* parent, const wxString& headline, const std::string& table);

    void LoadTasksFromDatabase(const std::string& table);

};
