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

#include "LibraryList.h"

#include "dataset/ISymbol.h"

using namespace d2d;

LibraryList::LibraryList(wxWindow* parent,
						 const wxString& filter/* = wxT("symbol")*/,
						 bool draggable /*= true*/)
	: VerticalImageList(parent, filter, draggable)
{
}

void LibraryList::loadFromTextFile(std::ifstream& fin)
{

}

void LibraryList::storeToTextFile(std::ofstream& fout) const
{

}

ListItem* LibraryList::getItem(int index/* = -1*/) const
{
	if (index == -1)
		index = GetSelection();

	if (index < 0 || index >= (int)m_items.size()) 
		return NULL;
	else 
		return m_items[index];
}

ISymbol* LibraryList::getSymbol(int index/* = -1*/) const
{
	ListItem* item = getItem(index);
	if (item)
		return static_cast<ISymbol*>(item);
	else
		return NULL;
}

void LibraryList::reloadTexture() const
{
	for (size_t i = 0, n = m_items.size(); i < n; ++i)
		static_cast<ISymbol*>(m_items[i])->reloadTexture();
}
