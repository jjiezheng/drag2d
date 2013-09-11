/*
* Copyright (c) 2012-2013 Guang Zhu http://runnersoft.net
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

#include "SelectDrawTypeWidget.h"

#include "view/OrthoCanvas.h"
#include "common/Settings.h"

using namespace d2d;

SelectDrawTypeWidget::SelectDrawTypeWidget(wxWindow* parent, OrthoCanvas* canvas)
	: AbstractWidget(parent)
	, m_canvas(canvas)
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	wxArrayString choices;
	choices.Add(wxT("物理+图片"));
	choices.Add(wxT("物理"));
	choices.Add(wxT("图片"));

	wxRadioBox* drawChoice = new wxRadioBox(this, wxID_ANY, wxT("显示方式"), 
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
