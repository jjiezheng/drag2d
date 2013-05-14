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

#ifndef D2D_INTERFACE_LIBRARY_PAGE_H
#define D2D_INTERFACE_LIBRARY_PAGE_H

#include <wx/wx.h>

#include "common/interface.h"

namespace d2d
{
	class ListItem;
	class ISymbol;
	class LibraryList;

	class ILibraryPage : public wxWindow, public ISerializable
	{
	public:
		ILibraryPage(wxWindow* parent, const wxString& name);

		//
		// ISerializable interface
		//
		virtual void loadFromTextFile(std::ifstream& fin);
		virtual void storeToTextFile(std::ofstream& fout) const;

		virtual bool isHandleSymbol(ISymbol* symbol) const = 0;

		virtual void clear();

		void traverse(IVisitor& visitor) const;

		const wxString& getName() const { return m_name; }

		ListItem* getItem(int index = -1) const;
		ISymbol* getSymbol(int index = -1) const;

		void reloadTexture() const;

		LibraryList* getList() { return m_list; }

	protected:
		virtual void initLayout(bool draggable = true);

		virtual void onAddPress(wxCommandEvent& event) = 0;
		virtual void onDelPress(wxCommandEvent& event);

	protected:
		void initButtons(wxSizer* sizer);

	protected:
		wxString m_name;

		wxButton *m_btnAdd, *m_btnDel;

		LibraryList* m_list;

	}; // ILibraryPage
}

#endif // D2D_INTERFACE_LIBRARY_PAGE_H
