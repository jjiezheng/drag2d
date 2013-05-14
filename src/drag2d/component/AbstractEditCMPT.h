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

#ifndef D2D_ABSTRACT_EDIT_CMPT_H
#define D2D_ABSTRACT_EDIT_CMPT_H

#include <wx/wx.h>
#include <vector>

namespace d2d
{
	class EditPanel;
	class AbstractEditOP;

	class AbstractEditCMPT : public wxPanel
	{
	public:
		AbstractEditCMPT(wxWindow* parent, const wxString& name, EditPanel* editPanel,
			const wxString& childrenName = wxEmptyString);
		virtual ~AbstractEditCMPT();

		virtual void updateControlValue() {}

	protected:
		virtual wxSizer* initLayout() = 0;

		virtual void show(bool show) {}

		void addChild(AbstractEditCMPT* child) { m_children.push_back(child); }

		wxSizer* initChildrenLayout();

	private:
		void loadEditOP();
		void loadEditOP(AbstractEditCMPT* cmpt);

		void onChangeEditType(wxCommandEvent& event);

	private:
		// wxRadioBox + Spacer
		static const int CMPT_SIZER_INDEX_OFFSET = 2;

	protected:
		EditPanel* m_editPanel;

		AbstractEditOP* m_editOP;

	private:
		wxString m_name;

		wxString m_childrenName;
		std::vector<AbstractEditCMPT*> m_children;
		wxSizer* m_childrenSizer;

	}; // AbstractEditCMPT
}

#endif // D2D_ABSTRACT_EDIT_CMPT_H
