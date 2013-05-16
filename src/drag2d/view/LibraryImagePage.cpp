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

#include "LibraryImagePage.h"
#include "LibraryList.h"

#include "dataset/SymbolMgr.h"
#include "dataset/ISymbol.h"
#include "dataset/ImageSymbol.h"

using namespace d2d;

LibraryImagePage::LibraryImagePage(wxWindow* parent, bool supportBmp/* = true*/)
	: ILibraryPage(parent, wxT("Image"))
	, m_supportBmp(supportBmp)
{
	initLayout();
}

bool LibraryImagePage::isHandleSymbol(ISymbol* symbol) const
{
	return dynamic_cast<ImageSymbol*>(symbol) != NULL;
}

void LibraryImagePage::onAddPress(wxCommandEvent& event)
{
	wxString formatFilter = m_supportBmp ? wxT("*.png;*.jpg;*.bmp") : wxT("*.png;*.jpg");
	wxFileDialog dlg(this, wxT("Choose images"), wxEmptyString, 
		wxEmptyString, formatFilter, wxFD_OPEN | wxFD_MULTIPLE);
	if (dlg.ShowModal() == wxID_OK)
	{
		wxArrayString filenames;
		dlg.GetPaths(filenames);
		for (size_t i = 0, n = filenames.size(); i < n; ++i)
		{
			ISymbol* symbol = SymbolMgr::Instance()->getSymbol(filenames[i]);
			m_list->insert(symbol);
		}
	}
}
