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

#include "DrawRectCMPT.h"

#include "operator/EditRectOP.h"

using namespace d2d;

DrawRectCMPT::DrawRectCMPT(wxWindow* parent, const wxString& name, 
						   EditPanel* editPanel, MultiShapesImpl* shapesImpl)
	: AbstractEditCMPT(parent, name, editPanel)
{
	m_editOP = new EditRectOP(editPanel, shapesImpl, this);
}

int DrawRectCMPT::getNodeCaptureDistance() const
{
	return m_tolSlider->GetValue();
}

wxSizer* DrawRectCMPT::initLayout()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("结点捕捉精度"));
	wxBoxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
	m_tolSlider = new wxSlider(this, wxID_ANY, 5, 0, 15, wxDefaultPosition, wxDefaultSize, wxSL_LABELS);
	sizer->Add(m_tolSlider);
	return sizer;
}