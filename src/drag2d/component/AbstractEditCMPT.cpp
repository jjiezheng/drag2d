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

#include "AbstractEditCMPT.h"

#include "common/tools.h"
#include "operator/AbstractEditOP.h"
#include "view/EditPanel.h"

using namespace d2d;

AbstractEditCMPT::AbstractEditCMPT(wxWindow* parent, const wxString& name, EditPanel* editPanel,
								   const wxString& childrenName/* = wxEmptyString*/)
	: wxPanel(parent)
	, m_editPanel(editPanel)
	, m_editOP(NULL)
	, m_name(name)
	, m_childrenName(childrenName)
	, m_childrenSizer(NULL)
{
}

AbstractEditCMPT::~AbstractEditCMPT()
{
	if (m_editOP)
		m_editOP->release();
 
 	for_each(m_children.begin(), m_children.end(), DeletePointerFunctor<AbstractEditCMPT>());
}

wxSizer* AbstractEditCMPT::initChildrenLayout()
{
	m_childrenSizer = new wxBoxSizer(wxVERTICAL);

	if (m_children.size() > 1)
	{
		wxArrayString choices;
		for (size_t i = 0, n = m_children.size(); i < n; ++i)
			choices.Add(m_children[i]->m_name);

		wxRadioBox* editChoice = new wxRadioBox(this, wxID_ANY, m_childrenName == wxEmptyString ? wxT("²Ù×÷Ñ¡Ôñ") : m_childrenName, 
			wxDefaultPosition, wxDefaultSize, choices, 1, wxRA_SPECIFY_COLS);
		Connect(editChoice->GetId(), wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEventHandler(AbstractEditCMPT::onChangeEditType));
		m_childrenSizer->Add(editChoice, 0);

		m_childrenSizer->AddSpacer(20);

		for (size_t i = 0, n = m_children.size(); i < n; ++i)
		{
			AbstractEditCMPT* child = m_children[i];
			child->SetSizer(child->initLayout());
			m_childrenSizer->Add(child);

			m_childrenSizer->Show(i + CMPT_SIZER_INDEX_OFFSET, i == 0);
		}
	}
	else if (m_children.size() == 1)
	{
		AbstractEditCMPT* child = m_children[0];
		child->SetSizer(child->initLayout());
		m_childrenSizer->Add(child);

		m_childrenSizer->Show((size_t)0, true);
	}

	if (!m_children.empty()) 
		m_children[0]->loadEditOP(this);

	return m_childrenSizer;
}

void AbstractEditCMPT::loadEditOP()
{
	m_editPanel->setEditOP(m_editOP);
	m_editPanel->Refresh();
}

void AbstractEditCMPT::loadEditOP(AbstractEditCMPT* cmpt)
{
	if (cmpt->m_editOP)
	{
		cmpt->m_editOP->clear();
		cmpt->m_editOP->release();
	}
	m_editOP->retain();
	cmpt->m_editOP = m_editOP;

	m_editPanel->setEditOP(m_editOP);
	m_editPanel->Refresh();
}

void AbstractEditCMPT::onChangeEditType(wxCommandEvent& event)
{
	int index = event.GetInt();

	for (size_t i = 0, n = m_children.size(); i < n; ++i)
	{
		m_childrenSizer->Show(i + CMPT_SIZER_INDEX_OFFSET, i == index);
		m_children[i]->show(i == index);

		m_children[i]->Layout();
	}
	Layout();

	if (index < m_children.size())
		m_children[index]->loadEditOP(this);
}
