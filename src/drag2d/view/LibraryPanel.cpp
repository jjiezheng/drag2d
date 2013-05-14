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

#include "LibraryPanel.h"
#include "ILibraryPage.h"
#include "LibraryList.h"
#include "LibraryImagePage.h"

#include "common/visitors.h"
#include "dataset/SymbolMgr.h"
#include "dataset/ImageSymbol.h"

using namespace d2d;

LibraryPanel::LibraryPanel(wxWindow* parent)
	: wxPanel(parent, wxID_ANY)
	, m_selected(NULL)
{
	initLayout();
}

void LibraryPanel::loadFromTextFile(std::ifstream& fin)
{

}

void LibraryPanel::storeToTextFile(std::ofstream& fout) const
{

}

void LibraryPanel::onPageChanged(wxBookCtrlEvent& event)
{
	m_selected = m_pages[event.GetSelection()];
}

void LibraryPanel::clear()
{
	for (size_t i = 0, n = m_pages.size(); i < n; ++i)
		m_pages[i]->clear();
}

void LibraryPanel::reloadTexture() const
{
	if (m_selected)
		m_selected->reloadTexture();
}

void LibraryPanel::addPage(ILibraryPage* page)
{
	if (m_notebook->GetPageCount() == 0)
		m_selected = page;

	m_notebook->AddPage(page, page->getName());
	m_pages.push_back(page);
}

ISymbol* LibraryPanel::getSymbol(int index/* = -1*/) const
{
	if (m_selected)
		return m_selected->getSymbol(index);
	else
		return NULL;
}

void LibraryPanel::loadFromSymbolMgr(const SymbolMgr& mgr)
{
	std::vector<ISymbol*> symbols;
	mgr.traverse(FetchAllVisitor<ISymbol>(symbols));
	for (size_t i = 0, n = symbols.size(); i < n; ++i)
	{
		ISymbol* symbol = symbols[i];
		for (size_t j = 0, m = m_pages.size(); j < m; ++j)
		{
			ILibraryPage* page = m_pages[j];
			if (page->isHandleSymbol(symbol))
			{
				page->getList()->insert(symbol);
				break;
			}
		}
	}
}

void LibraryPanel::traverse(IVisitor& visitor) const
{
	if (m_selected)
		m_selected->traverse(visitor);
}

void LibraryPanel::initLayout()
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	m_notebook = new wxNotebook(this, wxID_ANY);
	Connect(m_notebook->GetId(), wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxBookCtrlEventHandler(LibraryPanel::onPageChanged));

	sizer->Add(m_notebook, 1, wxEXPAND);

	SetSizer(sizer);
}
