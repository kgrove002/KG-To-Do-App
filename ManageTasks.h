#pragma once

#include <wx/wx.h>
#include "TaskPanel.h"
#include <vector>

class ManageTasks : public wxPanel
{
public:
    ManageTasks(wxWindow* parent);

    static void LoadTasksForAllDays();

private:
    void SwitchDays(int dayIndex);

    wxButton* SundayButton;
    wxButton* MondayButton;
    wxButton* TuesdayButton;
    wxButton* WednesdayButton;
    wxButton* ThursdayButton;
    wxButton* FridayButton;
    wxButton* SaturdayButton;

    static TaskPanel* SundayPanel;
    static TaskPanel* MondayPanel;
    static TaskPanel* TuesdayPanel;
    static TaskPanel* WednesdayPanel;
    static TaskPanel* ThursdayPanel;
    static TaskPanel* FridayPanel;
    static TaskPanel* SaturdayPanel;

    std::vector<TaskPanel*> panels;
    std::vector<wxButton*> buttons;

    void CreateControls();
    void CreateSizers();
    void mouseHover(wxMouseEvent& event);
    void mouseExit(wxMouseEvent& event);

    wxPanel* containerPanel; // Container for task panels
};
