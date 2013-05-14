/*
* Copyright (c) 2012-2013 XZRUNNER http://runnersoft.net
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

#include "MousePositionCMPT.h"

#include "operator/MousePositionOP.h"

using namespace d2d;

MousePositionCMPT::MousePositionCMPT(wxWindow* parent, const wxString& name, EditPanel* editPanel)
	: AbstractEditCMPT(parent, name, editPanel)
{
	m_editOP = new MousePositionOP(editPanel, this);
}

void MousePositionCMPT::updatePosition(const Vector& pos)
{
	m_xText->ChangeValue(wxString::FromDouble(pos.x, 1));
	m_yText->ChangeValue(wxString::FromDouble(pos.y, 1));
}

wxSizer* MousePositionCMPT::initLayout()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Êó±êÎ»ÖÃ"));
	wxBoxSizer* topSizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, wxT("x: ")));
		sizer->Add(m_xText = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY));
		topSizer->Add(sizer);
	}
	topSizer->AddSpacer(10);
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, wxT("y: ")));
		sizer->Add(m_yText = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY));
		topSizer->Add(sizer);
	}
	return topSizer;
}
