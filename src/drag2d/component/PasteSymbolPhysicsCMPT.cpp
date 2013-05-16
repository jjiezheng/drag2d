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

#include "PasteSymbolPhysicsCMPT.h"

#include "dataset/BodyFactory.h"
#include "operator/PasteSymbolPhysicsOP.h"
#include "operator/PasteSymbolPhysicsRandomOP.h"

using namespace d2d;
 
PasteSymbolPhysicsCMPT::PasteSymbolPhysicsCMPT(wxWindow* parent, const wxString& name,
											   EditPanel* editPanel, MultiSpritesImpl* spritesImpl, LibraryPanel* libraryPanel,
											   PhysicsPanelImpl* physicsImpl, b2World* world, b2World* worldReverse,
											   float* pScale /*= NULL*/, PasteSymbolRandomWidget* randomWidget/* = NULL*/)
	: AbstractEditCMPT(parent, name, editPanel)
	, m_world(world)
	, m_worldReverse(worldReverse)
{
	if (!randomWidget)
		m_editOP = new PasteSymbolPhysicsOP(editPanel, spritesImpl, libraryPanel, physicsImpl, pScale);
	else
		m_editOP = new PasteSymbolPhysicsRandomOP(editPanel, spritesImpl, libraryPanel, physicsImpl, randomWidget);
}

wxSizer* PasteSymbolPhysicsCMPT::initLayout()
{
	wxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
	{
		wxArrayString choices;
		choices.Add(wxT("向下"));
		choices.Add(wxT("向上"));
		m_gravityChoice = new wxRadioBox(this, wxID_ANY, wxT("重力方向"), wxDefaultPosition, wxDefaultSize, choices, 1, wxRA_SPECIFY_COLS);
		Connect(m_gravityChoice->GetId(), wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEventHandler(PasteSymbolPhysicsCMPT::onChangeGravity));
		topSizer->Add(m_gravityChoice);
	}
	topSizer->AddSpacer(10);
	{
		wxCheckBox* staticCtrl = new wxCheckBox(this, wxID_ANY, wxT("固定"));
		staticCtrl->SetValue(false);
		Connect(staticCtrl->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(PasteSymbolPhysicsCMPT::onChangeStaticType));
		topSizer->Add(staticCtrl);
	}
	return topSizer;
}

void PasteSymbolPhysicsCMPT::onChangeGravity(wxCommandEvent& event)
{
	if (m_gravityChoice->GetSelection() == 0)
		BodyFactory::setWorld(m_world);
	else
		BodyFactory::setWorld(m_worldReverse);
}

void PasteSymbolPhysicsCMPT::onChangeStaticType(wxCommandEvent& event)
{
	PasteSymbolPhysicsOP* editOP = static_cast<PasteSymbolPhysicsOP*>(m_editOP);
	editOP->setStatic(event.IsChecked());
}
