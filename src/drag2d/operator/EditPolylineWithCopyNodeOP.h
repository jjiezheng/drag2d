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

#ifndef D2D_EDIT_POLYLINE_WITH_COPY_NODE_OP_H
#define D2D_EDIT_POLYLINE_WITH_COPY_NODE_OP_H

#include "EditPolylineOP.h"
#include "SelectNodesOP.h"

namespace d2d
{
	template <typename T>
	class EditPolylineWithCopyNodeOP : public EditPolylineOP<T, SelectNodesOP, DrawPolygonCMPT>
	{
 	public:
 		EditPolylineWithCopyNodeOP(EditPanel* editPanel, MultiShapesImpl* shapesImpl)
 			: EditPolylineOP<T, SelectNodesOP, DrawPolygonCMPT>(editPanel, shapesImpl, NULL)
 		{}
 
 		virtual bool onKeyDown(int keyCode)
 		{
 			if (EditPolylineOP<T, SelectNodesOP, DrawPolygonCMPT>::onKeyDown(keyCode)) 
				return true;
 
 			if (wxGetKeyState(WXK_CONTROL_V))
 				m_selectOP->fetchSelectedNode(m_polyline);
 
 			return false;
 		}

	}; // EditPolylineWithCopyNodeOP
}

#include "EditPolylineWithCopyNodeOP.h"

#endif // D2D_EDIT_POLYLINE_WITH_COPY_NODE_OP_H
