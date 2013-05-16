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

#ifndef D2D_SELECT_PIXELS_OP_H
#define D2D_SELECT_PIXELS_OP_H

#include <vector>

#include "ZoomViewOP.h"
#include "dataset/RawPixels.h"

namespace d2d
{
	class SelectPixelsOP : public ZoomViewOP
	{
	public:
		enum e_Mode
		{
			RectSelect = 0,
			PointSelect,
			PolylineSelect
		};

	public:
		SelectPixelsOP(EditPanel* editPanel, RawPixels* pixels, e_Mode selectType, 
			RawPixels::PixelBuf& result, bool isSelect, int magicMarkerDis = 0);
		virtual ~SelectPixelsOP();

		virtual bool onMouseLeftDown(int x, int y);
		virtual bool onMouseLeftUp(int x, int y);
		virtual bool onMouseRightDown(int x, int y);
		virtual bool onMouseMove(int x, int y);
		virtual bool onMouseLeftDClick(int x, int y);
		virtual bool onMouseDrag(int x, int y);

		virtual bool onDraw() const;
		virtual bool clear();

		void setEditType(bool isSelect);
		void setMagicMarkerDis(int dis);

	private:
		void rectSelect();
		void pointSelect();
		void polylineSelect();

		void selectByPos(int x, int y);

		void selectByMagicMarker(int x, int y);

	private:
		RawPixels* m_pixels;

		bool m_isSelect;

		e_Mode m_mode;
		AbstractEditOP* m_drawOP;
		int m_magicMarkerDis;

		RawPixels::PixelBuf& m_result;
		
		Vector m_firstPos, m_currPos;
		std::vector<Vector> m_area;

	}; // SelectPixelsOP
}

#endif // D2D_SELECT_PIXELS_OP_H
