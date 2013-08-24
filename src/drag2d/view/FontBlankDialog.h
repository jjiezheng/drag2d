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

#ifndef D2D_FONT_BLANK_DIALOG_H
#define D2D_FONT_BLANK_DIALOG_H

#include <wx/wx.h>

namespace d2d
{
	class FontBlankSymbol;

	class FontBlankDialog : public wxDialog
	{
	public:
		FontBlankDialog(wxWindow* parent, FontBlankSymbol* symbol);

		~FontBlankDialog();

	private:
		void initLayout();

		void storeSymbol();

	private:
		FontBlankSymbol* m_symbol;

		wxTextCtrl* m_name;

		wxTextCtrl* m_font;

		wxTextCtrl* m_color;

		wxTextCtrl* m_align;
		wxTextCtrl* m_size;

		wxTextCtrl *m_width, *m_height;

	}; // FontBlankDialog
}

#endif // D2D_FONT_BLANK_DIALOG_H