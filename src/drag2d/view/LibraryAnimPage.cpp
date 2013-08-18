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

#include "LibraryAnimPage.h"
#include "LibraryList.h"

#include "dataset/AnimSymbol.h"
#include "common/FileNameParser.h"

using namespace d2d;

LibraryAnimPage::LibraryAnimPage(wxWindow* parent)
	: ILibraryPage(parent, wxT("Anim"))
{
	initLayout();
	m_list->setFileter("anim");
}

bool LibraryAnimPage::isHandleSymbol(d2d::ISymbol* symbol) const
{
	return dynamic_cast<AnimSymbol*>(symbol) != NULL;
}

void LibraryAnimPage::onAddPress(wxCommandEvent& event)
{
	wxString filter = FileNameParser::getFileTag(FileNameParser::e_anim);
	filter = wxT("*_") + filter + wxT(".json");
	wxFileDialog dlg(this, wxT("导入anim文件"), wxEmptyString, 
		wxEmptyString, filter, wxFD_OPEN | wxFD_MULTIPLE);
	if (dlg.ShowModal() == wxID_OK)
	{
		wxArrayString filenames;
		dlg.GetPaths(filenames);
		for (size_t i = 0, n = filenames.size(); i < n; ++i)
		{
			AnimSymbol* item = new AnimSymbol;
			item->loadFromFile(filenames[i]);
			item->refresh();
			m_list->insert(item);
		}
	}
}
