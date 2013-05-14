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

#include "EditPolylinesOP.h"

#include "algorithms/DouglasPeucker.h"
#include "component/EditPolylinesCMPT.h"
#include "dataset/ChainShape.h"
#include "common/Settings.h"
#include "render/PrimitiveDraw.h"

using namespace d2d;

EditPolylinesOP::EditPolylinesOP(EditPanel* editPanel, MultiShapesImpl* shapesImpl,
								 EditPolylinesCMPT* cmpt)
	: SelectShapesOP(editPanel, shapesImpl, cmpt)
	, m_cmpt(cmpt)
	, m_bDirty(false)
{
	m_lastPos.setInvalid();

	clearBuffer();
}

bool EditPolylinesOP::onMouseLeftDown(int x, int y)
{
	if (SelectShapesOP::onMouseLeftDown(x, y)) return true;

	if (!m_firstPos.isValid())
		m_lastPos = m_editPanel->transPosScreenToProject(x, y);
	else
		m_lastPos.setInvalid();

	return false;
}

bool EditPolylinesOP::onMouseLeftUp(int x, int y)
{
	if (SelectShapesOP::onMouseLeftUp(x, y)) return true;

	if (m_bDirty)
		m_selection->traverse(UpdateChainVisitor());

	clearBuffer();
	m_selection->traverse(UpdateBufferVisitor(m_simplifyBuffer));

	return false;
}

bool EditPolylinesOP::onMouseDrag(int x, int y)
{
	if (SelectShapesOP::onMouseDrag(x, y)) return true;

	if (m_lastPos.isValid())
	{
		Vector currPos = m_editPanel->transPosScreenToProject(x, y);
		Vector offset = currPos - m_lastPos;
		m_selection->traverse(OffsetVisitor(offset));
		m_lastPos = currPos;

		m_bDirty = true;
		m_editPanel->Refresh();
	}

	return false;
}

bool EditPolylinesOP::onDraw() const
{
	if (SelectShapesOP::onDraw()) return true;

	std::map<ChainShape*, ChainShape*>::const_iterator itr = m_simplifyBuffer.begin();
	for ( ; itr != m_simplifyBuffer.end(); ++itr)
	{
		itr->second->draw(Colorf(0.8f, 0.8f, 0.2f));
		PrimitiveDraw::drawCircles(itr->second->getVertices(), Settings::ctlPosSize, Colorf(0.2f, 0.2f, 0.8f));
	}

	return false;
}

bool EditPolylinesOP::clear()
{
	if (SelectShapesOP::clear()) return true;

	clearBuffer();

	return false;
}

void EditPolylinesOP::simplify()
{
	std::map<ChainShape*, ChainShape*>::iterator itr = m_simplifyBuffer.begin();
	for ( ; itr != m_simplifyBuffer.end(); ++itr)
	{
		std::vector<Vector> simplified;
		DouglasPeucker::implement(itr->first->getVertices(), m_cmpt->getSimplifyThreshold(), simplified);
		itr->second->setVertices(simplified);
	}

	m_editPanel->Refresh();
}

void EditPolylinesOP::updateFromSimplified()
{
	std::map<ChainShape*, ChainShape*>::iterator itr = m_simplifyBuffer.begin();
	for ( ; itr != m_simplifyBuffer.end(); ++itr)
		itr->first->setVertices(itr->second->getVertices());

	m_editPanel->Refresh();
}

void EditPolylinesOP::clearBuffer()
{
	std::map<ChainShape*, ChainShape*>::iterator itr = m_simplifyBuffer.begin();
	for ( ; itr != m_simplifyBuffer.end(); ++itr)
		delete itr->second;
	m_simplifyBuffer.clear();

	m_bDirty = false;
}

//////////////////////////////////////////////////////////////////////////
// class EditPolylinesOP::UpdateChainVisitor
//////////////////////////////////////////////////////////////////////////

void EditPolylinesOP::UpdateChainVisitor::
visit(ICloneable* object, bool& bFetchNext)
{
	ChainShape* chain = static_cast<ChainShape*>(object);

	chain->refresh();

	bFetchNext = true;
}

//////////////////////////////////////////////////////////////////////////
// class EditPolylinesOP::UpdateBufferVisitor
//////////////////////////////////////////////////////////////////////////

EditPolylinesOP::UpdateBufferVisitor::
UpdateBufferVisitor(std::map<ChainShape*, ChainShape*>& simplifyBuffer)
	: m_simplifyBuffer(simplifyBuffer)
{
}

void EditPolylinesOP::UpdateBufferVisitor::
visit(ICloneable* object, bool& bFetchNext)
{
	ChainShape* chain = static_cast<ChainShape*>(object);

	m_simplifyBuffer.insert(std::make_pair(chain, chain->clone()));

	bFetchNext = true;
}

//////////////////////////////////////////////////////////////////////////
// class EditPolylinesOP::OffsetVisitor
//////////////////////////////////////////////////////////////////////////

EditPolylinesOP::OffsetVisitor::
OffsetVisitor(const Vector& offset)
	: m_offset(offset)
{
}

void EditPolylinesOP::OffsetVisitor::
visit(ICloneable* object, bool& bFetchNext)
{
	ChainShape* chain = static_cast<ChainShape*>(object);

	std::vector<Vector> to(chain->getVertices());
	for (size_t i = 0, n = to.size(); i < n; ++i)
		to[i] += m_offset;
	chain->setVertices(to);

	bFetchNext = true;
}
