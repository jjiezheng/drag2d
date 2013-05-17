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

#include "NodeCapture.h"

#include "view/MultiShapesImpl.h"
#include "dataset/ChainShape.h"
#include "common/Math.h"

using namespace d2d;

NodeCapture::NodeCapture(MultiShapesImpl* shapesImpl, int tol)
	: m_shapesImpl(shapesImpl)
	, m_tol(tol)
{
}

void NodeCapture::captureEditable(const Vector& pos, NodeAddr& result)
{
	m_shapesImpl->traverseShapes(QueryChainVisitor(pos, m_tol, result), e_editable);
}

void NodeCapture::captureSelectable(const Vector& pos, NodeAddr& result)
{
	m_shapesImpl->traverseShapes(QueryChainVisitor(pos, m_tol, result), e_selectable);
}

NodeCapture::QueryChainVisitor::
QueryChainVisitor(const Vector& pos, float tolerance, NodeAddr& result)
	: m_pos(pos)
	, m_tolerance(tolerance)
	, m_rect(pos, tolerance, tolerance)
	, m_result(result)
{
	result.chain = NULL;
}

void NodeCapture::QueryChainVisitor::
visit(ICloneable* object, bool& bFetchNext)
{
	ChainShape* chain = static_cast<ChainShape*>(object);

	if (!Math::isAABBIntersectAABB(m_rect, chain->getRect()))
	{
		bFetchNext = true;
		return;
	}

	if (!chain->isIntersect(m_rect)) 
	{
		bFetchNext = true;
		return;
	}

	const std::vector<Vector>& vertices = chain->getVertices();
	for (size_t i = 0, n = vertices.size(); i < n; ++i)
	{
		if (Math::getDistance(vertices[i], m_pos) < m_tolerance)
		{
			m_result.chain = chain;
			m_result.pos = vertices[i];
			bFetchNext = false;
			return;
		}
	}

	bFetchNext = true;
}
