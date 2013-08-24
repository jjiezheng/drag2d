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

#include "LibraryFontBlankPage.h"
#include "LibraryFontBlankList.h"
#include "FontBlankDialog.h"

#include "dataset/SymbolMgr.h"
#include "dataset/FontBlankSymbol.h"
#include "common/FileNameParser.h"

using namespace d2d;

LibraryFontBlankPage::LibraryFontBlankPage(wxWindow* parent)
	: ILibraryPage(parent, wxT("FontBlank"))
{
	m_list = new LibraryFontBlankList(this);
	initLayout();
}

bool LibraryFontBlankPage::isHandleSymbol(ISymbol* symbol) const
{
	return dynamic_cast<FontBlankSymbol*>(symbol) != NULL;
}

void LibraryFontBlankPage::initLayout(bool draggable /*= true*/)
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	{
		wxBoxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);

		wxButton* btnNew = new wxButton(this, wxID_ANY, "new", wxDefaultPosition, wxSize(40, 20));
		Connect(btnNew->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(LibraryFontBlankPage::onNewBtnPress));
		btnSizer->Add(btnNew, 0, wxRIGHT, 10);

		initButtons(btnSizer);

		sizer->Add(btnSizer, 0, wxALIGN_RIGHT);
	}
	{
		sizer->Add(m_list, 1, wxEXPAND);
	}
	SetSizer(sizer);
}

void LibraryFontBlankPage::onAddPress(wxCommandEvent& event)
{
	wxString filter = FileNameParser::getFileTag(FileNameParser::e_fontblank);
	filter = wxT("*_") + filter + wxT(".json");
	wxFileDialog dlg(this, wxT("导入font blank文件"), wxEmptyString, 
		wxEmptyString, filter, wxFD_OPEN | wxFD_MULTIPLE);
	if (dlg.ShowModal() == wxID_OK)
	{
		ISymbol* symbol = SymbolMgr::Instance()->getSymbol(dlg.GetPath());
		m_list->insert(symbol);
	}
}

void LibraryFontBlankPage::onNewBtnPress(wxCommandEvent& event)
{
	FontBlankSymbol* item = new FontBlankSymbol();
	m_list->insert(item);

	FontBlankDialog dlg(this, item);
	dlg.ShowModal();

// 	Context::Instance()->stage->resetCanvas();
}