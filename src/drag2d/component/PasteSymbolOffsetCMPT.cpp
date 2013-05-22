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

#ifndef D2D_PASTE_SYMBOL_OFFSET_CMPT_CPP
#define D2D_PASTE_SYMBOL_OFFSET_CMPT_CPP

#include "PasteSymbolOffsetCMPT.h"

#include <wx/spinctrl.h>

//using namespace d2d;

template <typename TOperation>
d2d::PasteSymbolOffsetCMPT<TOperation>::
PasteSymbolOffsetCMPT(wxWindow* parent, const wxString& name,
					  EditPanel* editPanel, MultiSpritesImpl* spritesImpl, 
					  LibraryPanel* libraryPanel)
	: AbstractEditCMPT(parent, name, editPanel)
{
	m_editOP = new TOperation(editPanel, spritesImpl, libraryPanel, this);
}

template <typename TOperation>
d2d::Vector d2d::PasteSymbolOffsetCMPT<TOperation>::getOffset() const
{
	return Vector(m_xSpin->GetValue(), m_ySpin->GetValue());
}

template <typename TOperation>
wxSizer* d2d::PasteSymbolOffsetCMPT<TOperation>::initLayout()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("自动偏移"));
	wxBoxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);

	wxBoxSizer* xSizer = new wxBoxSizer(wxHORIZONTAL);
	xSizer->Add(new wxStaticText(this, wxID_ANY, wxT("水平偏移: ")));
	m_xSpin = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS | wxALIGN_RIGHT, -200, 200, 0);
	xSizer->Add(m_xSpin);
	sizer->Add(xSizer);

	wxBoxSizer* ySizer = new wxBoxSizer(wxHORIZONTAL);
	ySizer->Add(new wxStaticText(this, wxID_ANY, wxT("竖直偏移: ")));
	m_ySpin = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS | wxALIGN_RIGHT, -200, 200, 0);
	ySizer->Add(m_ySpin);
	sizer->Add(ySizer);

	return sizer;
}

#endif // D2D_PASTE_SYMBOL_OFFSET_CMPT_CPP