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

#include "LibraryShapePage.h"
#include "LibraryList.h"

#include "dataset/SymbolMgr.h"
#include "dataset/ISymbol.h"
#include "dataset/ShapeSymbol.h"
#include "common/FileNameParser.h"

using namespace d2d;

LibraryShapePage::LibraryShapePage(wxWindow* parent)
	: ILibraryPage(parent, wxT("图形"))
{
	initLayout();
}

bool LibraryShapePage::isHandleSymbol(ISymbol* symbol) const
{
	return dynamic_cast<ShapeSymbol*>(symbol) != NULL;
}

void LibraryShapePage::onAddPress(wxCommandEvent& event)
{
	wxString filter = "*_" + FileNameParser::getFileTag(FileNameParser::e_polygon) + ".txt";
	wxFileDialog dlg(this, wxT("选择图形文件"), wxEmptyString, 
		wxEmptyString, filter, wxFD_OPEN | wxFD_MULTIPLE);
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
