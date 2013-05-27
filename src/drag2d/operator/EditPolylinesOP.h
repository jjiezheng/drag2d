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

#ifndef D2D_EDIT_POLYLINES_OP_H
#define D2D_EDIT_POLYLINES_OP_H

#include <map>

#include "SelectShapesOP.h"

namespace d2d
{
	class ChainShape;
	class EditPolylinesCMPT;

	class EditPolylinesOP : public SelectShapesOP
	{
	public:
		EditPolylinesOP(EditPanel* editPanel, MultiShapesImpl* shapesImpl,
			PropertySettingPanel* propertyPanel, EditPolylinesCMPT* cmpt);

		virtual bool onMouseLeftDown(int x, int y);
		virtual bool onMouseLeftUp(int x, int y);
		virtual bool onMouseDrag(int x, int y);

		virtual bool onDraw() const;
		virtual bool clear();

		void simplify();
		void updateFromSimplified();

	private:
		void clearBuffer();

	private:
		class UpdateChainVisitor : public IVisitor
		{
		public:
			virtual void visit(ICloneable* object, bool& bFetchNext);
		}; // UpdateChainVisitor

		class UpdateBufferVisitor : public IVisitor
		{
		public:
			UpdateBufferVisitor(std::map<ChainShape*, ChainShape*>& simplifyBuffer);

			virtual void visit(ICloneable* object, bool& bFetchNext);

		private:
			std::map<ChainShape*, ChainShape*>& m_simplifyBuffer;

		}; // UpdateBufferVisitor

		class OffsetVisitor : public IVisitor
		{
		public:
			OffsetVisitor(const Vector& offset);

			virtual void visit(ICloneable* object, bool& bFetchNext);

		private:
			const Vector& m_offset;

		}; // OffsetVisitor

	private:
		EditPolylinesCMPT* m_cmpt;

		std::map<ChainShape*, ChainShape*> m_simplifyBuffer;

		Vector m_lastPos;

		bool m_bDirty;

	}; // EditPolylinesOP
}

#endif // D2D_EDIT_POLYLINES_OP_H
