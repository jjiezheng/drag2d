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

#ifndef D2D_EDIT_POLYLINE_OP_H
#define D2D_EDIT_POLYLINE_OP_H

#include "view/MultiShapesImpl.h"
#include "component/NodeCaptureCMPT.h"
#include "dataset/ChainShape.h"
#include "NodeCapture.h"

namespace d2d
{
	class EditPanel;
	class MultiShapesImpl;
	class ChainShape;
	class SelectShapesOP;
	class PropertySettingPanel;

	template <typename TBase, typename TSelected>
	class EditPolylineOP : public TBase
	{
	public:
		EditPolylineOP(EditPanel* editPanel, 
			MultiShapesImpl* shapesImpl, 
			PropertySettingPanel* propertyPanel,
			NodeCaptureCMPT<EditPolylineOP>* cmpt);
		virtual ~EditPolylineOP();

		virtual bool onKeyDown(int keyCode);
		virtual bool onMouseLeftDown(int x, int y);
		virtual bool onMouseLeftUp(int x, int y);
		virtual bool onMouseRightDown(int x, int y);
		virtual bool onMouseMove(int x, int y);
		virtual bool onMouseDrag(int x, int y);

		virtual bool onDraw() const;
		virtual bool clear();

	private:
		void drawCaptured(const NodeAddr& captured) const;

	private:
		class InterruptChainVisitor : public IVisitor
		{
		public:
			InterruptChainVisitor(const Vector& pos, int tol);

			virtual void visit(ICloneable* object, bool& bFetchNext);

			ChainShape* getInterruptedChain() { return m_chain; }

		private:
			const Vector& m_pos;
			int m_tol;
			ChainShape* m_chain;

		}; // InterruptChainVisitor

		class NearestNodeVisitor : public IVisitor
		{
		public:
			NearestNodeVisitor(const Vector& pos, int tol);

			virtual void visit(ICloneable* object, bool& bFetchNext);

			const Vector& getNearestNode() const {
				return m_nearest;
			}

		private:
			Vector m_pos;
			int m_tol;

			float m_dis;
			Vector m_nearest;

		}; // NearestNodeVisitor

	private:
		static const int DRAG_SELECT_TOL = 5;

	private:
		MultiShapesImpl* m_shapesImpl;

		PropertySettingPanel* m_propertyPanel;

		NodeCaptureCMPT<EditPolylineOP>* m_cmpt;
//		NodeAddr m_captured;

	protected:
		NodeAddr m_capturedEditable, m_captureSelectable;

	protected:
		TSelected* m_selectOP;
	private:
		Vector m_lastLeftDownPos;
		bool m_bSelectOpen;

	}; // EditPolylineOP
}

#include "EditPolylineOP.cpp"

#endif // D2D_EDIT_POLYLINE_OP_H
