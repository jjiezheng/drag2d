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

#ifndef D2D_PASTE_SYMBOL_RANDOM_WIDGET_H
#define D2D_PASTE_SYMBOL_RANDOM_WIDGET_H

#include <vector>
#include <wx/wx.h>

#include "common/interface.h"

namespace d2d
{
	class ISymbol;
	class LibraryPanel;

	class PasteSymbolRandomWidget : public wxPanel
	{
	public:
		struct RandomValue
		{
			ISymbol* symbol;
			float scale;
			float angle;	// in radian
		};

	public:
		PasteSymbolRandomWidget(wxWindow* parent, LibraryPanel* libraryPanel);

		void getRandomValue(RandomValue& val) const;

	private:
		void initLayout();

	private:
		class FilterSymbolVisitor : public IVisitor
		{
		public:
			FilterSymbolVisitor(const wxString& filter, std::vector<ISymbol*>& result);

			virtual void visit(ICloneable* object, bool& bFetchNext);

		private:
			wxString m_filter;

			std::vector<ISymbol*>& m_result;
		};

	private:
		LibraryPanel* m_libraryPanel;

		wxTextCtrl* m_symbolFilterCtrl;
		wxSpinCtrl *m_scaleMinCtrl, *m_scaleMaxCtrl;
		wxSpinCtrl *m_angleMinCtrl, *m_angleMaxCtrl;

	}; // PasteSymbolRandomWidget
}

#endif // D2D_PASTE_SYMBOL_RANDOM_WIDGET_H
