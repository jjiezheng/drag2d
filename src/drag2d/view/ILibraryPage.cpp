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

#include "ILibraryPage.h"
#include "LibraryList.h"

using namespace d2d;

ILibraryPage::ILibraryPage(wxWindow* parent, const wxString& name)
	: wxWindow(parent, wxID_ANY)
	, m_name(name)
	, m_list(NULL)
{
	m_btnAdd = m_btnDel = NULL;
}

void ILibraryPage::loadFromTextFile(std::ifstream& fin)
{
	m_list->loadFromTextFile(fin);
}

void ILibraryPage::storeToTextFile(std::ofstream& fout) const
{
	m_list->storeToTextFile(fout);
}

void ILibraryPage::clear()
{
	m_list->clear();
}

void ILibraryPage::traverse(IVisitor& visitor) const
{
	m_list->traverse(visitor);
}

ListItem* ILibraryPage::getItem(int index/* = -1*/) const
{
	return m_list->getItem(index);
}

ISymbol* ILibraryPage::getSymbol(int index/* = -1*/) const
{
	return m_list->getSymbol(index);
}

void ILibraryPage::reloadTexture() const
{
	m_list->reloadTexture();
}

void ILibraryPage::initLayout(bool draggable/* = true*/)
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	initButtons(sizer);

	if (!m_list)
		m_list = new LibraryList(this);
	sizer->Add(m_list, 1, wxEXPAND);

	SetSizer(sizer);
}

void ILibraryPage::onDelPress(wxCommandEvent& event)
{
	m_list->remove();
}

void ILibraryPage::initButtons(wxSizer* sizer)
{
	wxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);

	m_btnAdd = new wxButton(this, wxID_ANY, "+", wxDefaultPosition, wxSize(20, 20));
	Connect(m_btnAdd->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ILibraryPage::onAddPress));
	btnSizer->Add(m_btnAdd, 0, wxLEFT | wxRIGHT, 5);

	m_btnDel = new wxButton(this, wxID_ANY, "-", wxDefaultPosition, wxSize(20, 20));
	Connect(m_btnDel->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ILibraryPage::onDelPress));
	btnSizer->Add(m_btnDel, 0, wxLEFT | wxRIGHT, 5);

	sizer->Add(btnSizer, 0, wxALIGN_RIGHT);
}
