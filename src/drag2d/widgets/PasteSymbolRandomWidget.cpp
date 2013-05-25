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

#include "PasteSymbolRandomWidget.h"

#include "common/Random.h"
#include "common/Math.h"
#include "dataset/ISymbol.h"
#include "view/LibraryPanel.h"

#include <wx/spinctrl.h>

using namespace d2d;

PasteSymbolRandomWidget::PasteSymbolRandomWidget(wxWindow* parent, LibraryPanel* libraryPanel)
	: wxPanel(parent)
	, m_libraryPanel(libraryPanel)
{
	initLayout();
}

void PasteSymbolRandomWidget::initLayout()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("随机参数"));
	wxBoxSizer* topSizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
	topSizer->AddSpacer(5);
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("元件过滤"));
		wxBoxSizer* sizer = new wxStaticBoxSizer(bounding, wxHORIZONTAL);
		sizer->Add(m_symbolFilterCtrl = new wxTextCtrl(this, wxID_ANY, wxEmptyString));
		topSizer->Add(sizer);
	}
	topSizer->AddSpacer(5);
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Scale"));
		wxBoxSizer* sizer = new wxStaticBoxSizer(bounding, wxHORIZONTAL);
		m_scaleMinCtrl = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(50, -1), wxSP_ARROW_KEYS | wxALIGN_RIGHT, 10, 100, 75);
		sizer->Add(m_scaleMinCtrl);
		sizer->Add(new wxStaticText(this, wxID_ANY, wxT(" - ")));
		m_scaleMaxCtrl = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(50, -1), wxSP_ARROW_KEYS | wxALIGN_RIGHT, 10, 100, 100);
		sizer->Add(m_scaleMaxCtrl);
		topSizer->Add(sizer);
	}
	topSizer->AddSpacer(5);
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Angle"));
		wxBoxSizer* sizer = new wxStaticBoxSizer(bounding, wxHORIZONTAL);
		m_angleMinCtrl = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(55, -1), wxSP_ARROW_KEYS | wxALIGN_RIGHT, -180, 180, -45);
		sizer->Add(m_angleMinCtrl);
		sizer->Add(new wxStaticText(this, wxID_ANY, wxT(" - ")));
		m_angleMaxCtrl = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(55, -1), wxSP_ARROW_KEYS | wxALIGN_RIGHT, -180, 180, 45);
		sizer->Add(m_angleMaxCtrl);
		topSizer->Add(sizer);
	}
	SetSizer(topSizer);
}

void PasteSymbolRandomWidget::getRandomValue(RandomValue& val) const
{
	std::vector<ISymbol*> symbols;
	m_libraryPanel->traverse(FilterSymbolVisitor(m_symbolFilterCtrl->GetValue(), symbols));
	if (symbols.empty())
		val.symbol = NULL;
	else
		val.symbol = symbols[static_cast<int>(symbols.size() * Random::getNum0To1())];

	const float scaleMin = m_scaleMinCtrl->GetValue() * 0.01f,
		scaleMax = m_scaleMaxCtrl->GetValue() * 0.01f;
	val.scale = scaleMin + (scaleMax - scaleMin) * Random::getNum0To1();

	const float angleMin = m_angleMinCtrl->GetValue() * TRANS_DEG_TO_RAD,
		angleMax = m_angleMaxCtrl->GetValue() * TRANS_DEG_TO_RAD;
	val.angle = angleMin + (angleMax - angleMin) * Random::getNum0To1();
}

//////////////////////////////////////////////////////////////////////////
// class PasteSymbolRandomWidget::FilterSymbolVisitor
//////////////////////////////////////////////////////////////////////////

PasteSymbolRandomWidget::FilterSymbolVisitor::
FilterSymbolVisitor(const wxString& filter, std::vector<ISymbol*>& result)
	: m_filter(filter)
	, m_result(result)
{
}

void PasteSymbolRandomWidget::FilterSymbolVisitor::
visit(ICloneable* object, bool& bFetchNext)
{
	ISymbol* symbol = static_cast<ISymbol*>(object);
	if (!m_filter.empty() && symbol->getFilepath().find(m_filter) != wxNOT_FOUND)
		m_result.push_back(symbol);
	bFetchNext = true;
}
