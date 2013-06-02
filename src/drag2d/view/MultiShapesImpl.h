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

#ifndef D2D_MULTI_SHAPES_IMPL_H
#define D2D_MULTI_SHAPES_IMPL_H

#include <vector>
#include <wx/wx.h>

#include "EditPanelDefs.h"

#include "dataset/SelectionSet.h"

namespace d2d
{
	class MultiShapesImpl
	{
	public:
		MultiShapesImpl(wxWindow* wnd);
		virtual ~MultiShapesImpl();

		virtual void traverseShapes(IVisitor& visitor, 
			TraverseType type = e_allExisting) const = 0;
		virtual void removeShape(IShape* shape) = 0;
		virtual void insertShape(IShape* shape) = 0;
		virtual void clearShapes() = 0;

		IShape* queryShapeByPos(const Vector& pos) const;
		void queryShapesByRect(const Rect& rect, std::vector<IShape*>& result) const;		

		ShapeSelection* getShapeSelection() { return m_shapeSelection; }
		void removeShapeSelection();

	private:
		class PointQueryVisitor : public IVisitor
		{
		public:
			PointQueryVisitor(const Vector& pos, IShape** pResult);
			virtual void visit(ICloneable* object, bool& bFetchNext);

		private:
			const Vector& m_pos;
			IShape** m_pResult;

		}; // PointQueryVisitor

		class RectQueryVisitor : public IVisitor
		{
		public:
			RectQueryVisitor(const Rect& rect, std::vector<IShape*>& result);
			virtual void visit(ICloneable* object, bool& bFetchNext);

		private:
			const Rect& m_rect;
			std::vector<IShape*>& m_result;

		}; // RectQueryVisitor

		class RemoveSelectionVisitor : public IVisitor
		{
		public:
			RemoveSelectionVisitor(MultiShapesImpl* shapesImpl);
			virtual void visit(ICloneable* object, bool& bFetchNext);

		private:
			MultiShapesImpl* m_shapesImpl;

		}; // RemoveSelectionVisitor

	protected:
		wxWindow* m_wnd;

		ShapeSelection* m_shapeSelection;

	}; // MultiShapesImpl
}

#endif // D2D_MULTI_SHAPES_IMPL_H
