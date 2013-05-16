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

#include "ShapesPanelImpl.h"

#include "common/tools.h"

using namespace d2d;

ShapesPanelImpl::ShapesPanelImpl(wxWindow* wnd)
	: MultiShapesImpl(wnd)
{
}

ShapesPanelImpl::~ShapesPanelImpl()
{
	clear();
}

void ShapesPanelImpl::traverseShapes(IVisitor& visitor, 
									 TraverseType type/* = e_allExisting*/) const
{
	std::vector<IShape*>::const_iterator itr = m_shapes.begin();
	for ( ; itr != m_shapes.end(); ++itr)
	{
		bool hasNext;
		visitor.visit(*itr, hasNext);
		if (!hasNext) break;
	}
}

void ShapesPanelImpl::removeShape(IShape* shape)
{
	std::vector<IShape*>::iterator itr = m_shapes.begin();
	for ( ; itr != m_shapes.end(); ++itr)
	{
		if (*itr == shape)
		{
			delete *itr;
			m_shapes.erase(itr);
			break;
		}
	}
}

void ShapesPanelImpl::insertShape(IShape* shape)
{
	m_shapes.push_back(shape);
	m_wnd->Refresh();
}

void ShapesPanelImpl::clear()
{
	for_each(m_shapes.begin(), m_shapes.end(), DeletePointerFunctor<IShape>());
	m_shapes.clear();
}
