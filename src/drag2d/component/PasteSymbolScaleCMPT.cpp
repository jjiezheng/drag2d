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

#include "PasteSymbolScaleCMPT.h"

#include "operator/PasteSymbolOP.h"
#include "view/MultiSpritesImpl.h"
#include "view/LibraryPanel.h"

#include <wx/spinctrl.h>

using namespace d2d;

PasteSymbolScaleCMPT::PasteSymbolScaleCMPT(wxWindow* parent, const wxString& name, 
										   EditPanel* editPanel, MultiSpritesImpl* spritesImpl, 
										   LibraryPanel* libraryPanel)
	: AbstractEditCMPT(parent, name, editPanel)
	, m_scaleVal(1.0f)
{
	m_editOP = new PasteSymbolOP(editPanel, spritesImpl, libraryPanel, &m_scaleVal);
}

wxSizer* PasteSymbolScaleCMPT::initLayout()
{
	wxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, wxT("Ëõ·Å: ")));

		wxSpinCtrl* scaleCtrl = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(50, -1), 
			wxSP_ARROW_KEYS, 10, 100, 100);
		Connect(scaleCtrl->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(PasteSymbolScaleCMPT::onChangeScale));
		sizer->Add(scaleCtrl);

		sizer->Add(new wxStaticText(this, wxID_ANY, wxT("%")));
		topSizer->Add(sizer);
	}
	return topSizer;
}

void PasteSymbolScaleCMPT::onChangeScale(wxSpinEvent& event)
{
	m_scaleVal = event.GetValue() * 0.01f;
}
