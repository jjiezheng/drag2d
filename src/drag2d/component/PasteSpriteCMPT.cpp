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

#include "PasteSpriteCMPT.h"

#include "operator/PasteSpriteOP.h"

using namespace d2d;

PasteSpriteCMPT::PasteSpriteCMPT(wxWindow* parent, const wxString& name, 
								 EditPanel* editPanel, SpritesPanelImpl* spritesImpl,
								 PropertySettingPanel* propertyPanel)
	: AbstractEditCMPT(parent, name, editPanel)
{
	m_editOP = new PasteSpriteOP(editPanel, spritesImpl, propertyPanel, this);
}

wxSizer* PasteSpriteCMPT::initLayout()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("工具"));
	wxBoxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);

// 	wxBoxSizer* xSizer = new wxBoxSizer(wxHORIZONTAL);
// 	xSizer->Add(new wxStaticText(this, wxID_ANY, wxT("水平偏移: ")));
//	wxCheckBox* m_xCheckBox = new wxCheckBox(this, wxID_ANY, wxT("水平镜像"));

	sizer->Add(m_xMirror = new wxCheckBox(this, wxID_ANY, wxT("水平镜像")));
	sizer->AddSpacer(10);
	sizer->Add(m_yMirror = new wxCheckBox(this, wxID_ANY, wxT("竖直镜像")));

	return sizer;
}
