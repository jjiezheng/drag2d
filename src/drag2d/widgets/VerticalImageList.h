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

#ifndef D2D_VERTICAL_IMAGE_LIST_H
#define D2D_VERTICAL_IMAGE_LIST_H

#include <wx/wx.h>
#include <wx/vlbox.h>
#include <vector>

namespace d2d
{
	class ListItem;
	class IVisitor;

	class VerticalImageList : public wxVListBox
	{
	public:
		VerticalImageList(wxWindow* parent, const wxString& name = wxEmptyString,
			bool draggable = true);
		virtual ~VerticalImageList();

		// VerticalImageList
		virtual void onListSelected(wxCommandEvent& event) {}
		virtual void onListDoubleClicked(wxCommandEvent& event) {}

		void traverse(IVisitor& visitor) const;

		void clear();
		void insert(ListItem* item);
		void remove();

	protected:
		// wxVListBox
		virtual void OnDrawItem(wxDC& dc, const wxRect& rect, size_t n) const;
//		virtual void OnDrawBackground(wxDC& dc, const wxRect& rect, size_t n) const;
		virtual void OnDrawSeparator(wxDC& dc, wxRect& rect, size_t n) const;
		virtual wxCoord OnMeasureItem(size_t n) const;

	private:
		void onDragInit(wxMouseEvent& event);

	private:
		static const int SPACE_UP = 5;
		static const int SPACE_DOWN = 25;

	protected:
		std::vector<ListItem*> m_items;

	private:
		wxString m_name;

	}; // VerticalImageList
}

#endif // D2D_VERTICAL_IMAGE_LIST_H
