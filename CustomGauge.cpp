#include "CustomGauge.h"

// Constructor
CustomGauge::CustomGauge(wxWindow* parent, int range, int value, wxPoint pos, wxSize size)
    : wxPanel(parent, wxID_ANY, pos, size, wxNO_BORDER),
    m_range(range), m_value(value), m_color(*wxBLUE) {
    // Set background style for custom painting
    SetBackgroundStyle(wxBG_STYLE_PAINT);

    // Bind the paint event handler
    Bind(wxEVT_PAINT, &CustomGauge::OnPaint, this);
}

// Set the range of the gauge
void CustomGauge::SetRange(int range) {
    m_range = range;
    // Refresh the gauge to reflect the new range
    Refresh();
}

// Set the value of the gauge
void CustomGauge::SetValue(int value) {
    m_value = value;
    // Refresh the gauge to reflect the new value
    Refresh();
}

// Set the color of the gauge
void CustomGauge::SetColor(const wxColour& color) {
    m_color = color;
    // Refresh the gauge to reflect the new color
    Refresh();
}

// Paint event handler
void CustomGauge::OnPaint(wxPaintEvent& event) {
    // Use wxAutoBufferedPaintDC for double-buffered painting
    wxAutoBufferedPaintDC dc(this);
    dc.Clear();

    // Get the size of the gauge
    wxSize size = GetClientSize();
    int width = size.GetWidth();
    int height = size.GetHeight();

    // Calculate the filled width based on the current value and range
    if (m_range > 0) {
        int filledWidth = width * m_value / m_range;

        // Set the brush and draw the filled portion
        dc.SetBrush(wxBrush(m_color));
        dc.SetPen(*wxTRANSPARENT_PEN);
        dc.DrawRectangle(0, 0, filledWidth, height);
    }

    // Draw the outline of the gauge
    dc.SetBrush(*wxTRANSPARENT_BRUSH);
    dc.SetPen(wxPen(*wxBLACK));
    dc.DrawRectangle(0, 0, width, height);
}
