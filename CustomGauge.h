#pragma once
#include <wx/wx.h>
#include <wx/dcbuffer.h>

class CustomGauge : public wxPanel {
public:
    CustomGauge(wxWindow* parent, int range = 100, int value = 0, wxPoint pos = wxDefaultPosition, wxSize size = wxDefaultSize);

    void SetRange(int range);
    void SetValue(int value);
    void SetColor(const wxColour& color);

private:
    void OnPaint(wxPaintEvent& event);

    int m_range;
    int m_value;
    wxColour m_color;
};