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

#ifndef D2D_NODE_CAPTURE_OP_H
#define D2D_NODE_CAPTURE_OP_H

#include "common/Vector.h"
#include "common/interface.h"
#include "common/Rect.h"

namespace d2d
{
	class IShape;
	class BezierShape;
	class ChainShape;
	class CircleShape;
	class RectShape;
	class MultiShapesImpl;

	struct NodeAddr
	{
		IShape* shape;
		Vector pos;

		NodeAddr() : shape(NULL) {}

		void clear() {
			shape = NULL;
			pos.setInvalid();
		}
	};

	class NodeCapture
	{
	public:
		NodeCapture(MultiShapesImpl* shapesImpl, int tol);

		void captureEditable(const Vector& pos, NodeAddr& result);
		void captureSelectable(const Vector& pos, NodeAddr& result);

	private:
		class RectQueryVisitor : public IVisitor
		{
		public:
			RectQueryVisitor(const Vector& pos, float tolerance, NodeAddr& result);

			virtual void visit(ICloneable* object, bool& bFetchNext);

		private:
			bool visit(BezierShape* bezier);
			bool visit(ChainShape* chain);
			bool visit(CircleShape* circle);
			bool visit(RectShape* rect);

		private:
			const Vector& m_pos;
			float m_tolerance;

			Rect m_rect;
			NodeAddr& m_result;

		}; // RectQueryVisitor

	private:
		MultiShapesImpl* m_shapesImpl;
		int m_tol;

	}; // NodeCapture
}

#endif // D2D_NODE_CAPTURE_OP_H