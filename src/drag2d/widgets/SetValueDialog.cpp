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

#include "SetValueDialog.h"

using namespace d2d;

SetValueDialog::SetValueDialog(wxWindow* parent, const wxString& title, const wxString& oldVal, const wxPoint& pos)
	: wxDialog(parent, wxID_ANY, title, pos)
{
	initLayout(oldVal);
	SetPosition(wxPoint(pos.x, pos.y - GetSize().y));
}

wxString SetValueDialog::getText() const
{
	return m_textCtrl->GetLineText(0);
}

void SetValueDialog::initLayout(const wxString& oldVal)
{
	wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);	

	m_textCtrl = new wxTextCtrl(this, wxID_ANY, oldVal);
	topSizer->Add(m_textCtrl, 0, wxALIGN_CENTRE | wxALL, 5);

	wxBoxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);
	btnSizer->Add(new wxButton(this, wxID_OK), wxALL, 5);
	btnSizer->Add(new wxButton(this, wxID_CANCEL), wxALL, 5);
	topSizer->Add(btnSizer, 0, wxALL, 5);

	SetSizer(topSizer);
	topSizer->Fit(this);
}
