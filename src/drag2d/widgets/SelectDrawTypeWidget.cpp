#include "SelectDrawTypeWidget.h"

#include "view/OrthoCanvas.h"
#include "common/Settings.h"

namespace d2d
{

SelectDrawTypeWidget::SelectDrawTypeWidget(wxWindow* parent, OrthoCanvas* canvas)
	: AbstractWidget(parent)
	, m_canvas(canvas)
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	wxArrayString choices;
	choices.Add(wxT("����+ͼƬ"));
	choices.Add(wxT("����"));
	choices.Add(wxT("ͼƬ"));

	wxRadioBox* drawChoice = new wxRadioBox(this, wxID_ANY, wxT("��ʾ��ʽ"), 
		wxDefaultPosition, wxDefaultSize, choices, 1, wxRA_SPECIFY_COLS);
	Connect(drawChoice->GetId(), wxEVT_COMMAND_RADIOBOX_SELECTED, 
		wxCommandEventHandler(SelectDrawTypeWidget::onSetDrawType));
	drawChoice->SetSelection(Settings::drawType);

	sizer->Add(drawChoice);

	SetSizer(sizer);
}

void SelectDrawTypeWidget::onSetDrawType(wxCommandEvent& event)
{
	Settings::drawType = static_cast<Settings::DrawType>(event.GetSelection());
	m_canvas->Refresh();
}

} // d2d