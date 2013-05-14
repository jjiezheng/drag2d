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

#ifndef D2D_EDIT_POLYLINE_OP_H
#define D2D_EDIT_POLYLINE_OP_H

#include "view/MultiShapesImpl.h"
#include "component/DrawPolylineCMPT.h"
#include "dataset/ChainShape.h"

namespace d2d
{
	class EditPanel;
	class MultiShapesImpl;
	class DrawPolylineCMPT;
	class ChainShape;
	class SelectShapesOP;

	template <typename TBase, typename TSelected, typename TCMPT>
	class EditPolylineOP : public TBase
	{
	public:
		EditPolylineOP(EditPanel* editPanel, 
			MultiShapesImpl* shapesImpl, TCMPT* cmpt);
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
		struct NodeAddr
		{
			ChainShape* chain;
			Vector pos;

			NodeAddr() : chain(NULL) {}
		};

		class NodeCapture
		{
		public:
			NodeCapture(MultiShapesImpl* shapesImpl, int tol);

			void captureEditable(const Vector& pos, NodeAddr& result);
			void captureSelectable(const Vector& pos, NodeAddr& result);

		private:
			class QueryChainVisitor : public IVisitor
			{
			public:
				QueryChainVisitor(const Vector& pos, float tolerance, NodeAddr& result);

				virtual void visit(ICloneable* object, bool& bFetchNext);

			private:
				const Vector& m_pos;
				float m_tolerance;

				Rect m_rect;
				NodeAddr& m_result;

			}; // SetTexCoordsVisitor

		private:
			MultiShapesImpl* m_shapesImpl;
			int m_tol;

		}; // NodeCapture

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

	private:
		static const int DRAG_SELECT_TOL = 5;

	private:
		MultiShapesImpl* m_shapesImpl;

		TCMPT* m_cmpt;
//		NodeAddr m_captured;
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
