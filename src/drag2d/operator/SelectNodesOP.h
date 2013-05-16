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

#ifndef D2D_SELECT_NODES_OP_H
#define D2D_SELECT_NODES_OP_H

#include <vector>

#include "DrawRectangleOP.h"

#include "common/Rect.h"
#include "dataset/SelectionSet.h"

namespace d2d
{
	class MultiShapesImpl;
	class AbstractEditCMPT;
	class ChainShape;

	class SelectNodesOP : public DrawRectangleOP
	{
	public:
		SelectNodesOP(EditPanel* editPanel, MultiShapesImpl* shapesImpl, AbstractEditCMPT* callback = NULL);
		virtual ~SelectNodesOP();

		virtual bool onKeyDown(int keyCode);
		virtual bool onMouseLeftDown(int x, int y);
		virtual bool onMouseLeftUp(int x, int y);

		virtual bool onDraw() const;
		virtual bool clear();

		void fetchSelectedNode(std::vector<Vector>& nodes) const;

		static int getThreshold();

	protected:
		void clearSelectedNodes();

	protected:
		struct ChainSelectedNodes
		{
		public:
			ChainShape* chain;
			std::vector<Vector> selectedNodes;
		}; // ChainSelectedNodes

	private:
		class PosQueryVisitor : public IVisitor
		{
		public:
			PosQueryVisitor(const Vector& pos, ChainSelectedNodes** result);
			virtual void visit(ICloneable* object, bool& bFetchNext);

		private:
			const Vector& m_pos;
			Rect m_rect;
			ChainSelectedNodes** m_result;

		}; // PosQueryVisitor

		class RectQueryVisitor : public IVisitor
		{
		public:
			RectQueryVisitor(const Rect& rect, std::vector<ChainSelectedNodes*>& result);
			virtual void visit(ICloneable* object, bool& bFetchNext);

		private:
			const Rect& m_rect;
			std::vector<ChainSelectedNodes*>& m_result;

		}; // RectQueryVisitor

	protected:
		std::vector<ChainSelectedNodes*> m_nodeSelection;

	private:
		MultiShapesImpl* m_shapeImpl;

		Vector m_firstPos;

	}; // SelectNodesOP
}

#endif // D2D_SELECT_NODES_OP_H
