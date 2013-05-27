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

#include "EditNodesOP.h"

#include "algorithms/DouglasPeucker.h"
#include "algorithms/CosineSmooth.h"
#include "dataset/ChainShape.h"
#include "common/Settings.h"
#include "history/EditNodesAtomicOP.h"
#include "render/PrimitiveDraw.h"

using namespace d2d;

EditNodesOP::EditNodesOP(EditPanel* editPanel, MultiShapesImpl* shapesImpl)
	: SelectNodesOP(editPanel, shapesImpl, NULL)
{
	m_lastPos.setInvalid();
}

bool EditNodesOP::onKeyDown(int keyCode)
{
	if (keyCode == WXK_DELETE)
	{
		for (size_t i = 0, n = m_nodeSelection.size(); i < n; ++i)
		{
			SelectNodesOP::ChainSelectedNodes* selected = m_nodeSelection[i];
			for (size_t j = 0, m = selected->selectedNodes.size(); j < m; ++j)
				selected->chain->removeVertices(selected->selectedNodes[j]);
		}
	}

	SelectNodesOP::onKeyDown(keyCode);

	return false;
}

bool EditNodesOP::onMouseLeftDown(int x, int y)
{
	if (SelectNodesOP::onMouseLeftDown(x, y)) return true;

	if (wxGetKeyState(WXK_CONTROL)) return false;

	if (m_nodeSelection.empty())
		m_lastPos.setInvalid();
	else
		m_lastPos = m_editPanel->transPosScreenToProject(x, y);

	return false;
}

bool EditNodesOP::onMouseDrag(int x, int y)
{
	if (SelectNodesOP::onMouseDrag(x, y)) return true;

	if (wxGetKeyState(WXK_CONTROL)) return false;

	if (m_lastPos.isValid())
	{
		Vector currPos = m_editPanel->transPosScreenToProject(x, y);
		Vector offset = currPos - m_lastPos;
		for (size_t i = 0, n = m_nodeSelection.size(); i < n; ++i)
		{
			SelectNodesOP::ChainSelectedNodes* selected = m_nodeSelection[i];
			for (size_t j = 0, m = selected->selectedNodes.size(); j < m; ++j)
			{
				const Vector& from = selected->selectedNodes[j];
				Vector to = from + offset;
				selected->chain->changeVertices(from, to);
				selected->selectedNodes[j] = to;
			}
			selected->chain->refresh();
		}
		m_lastPos = currPos;

		m_editPanel->Refresh();
	}

	return false;
}

bool EditNodesOP::onDraw() const
{
	if (SelectNodesOP::onDraw()) return true;

	const float radius = Settings::ctlPosSize == 0 ? 3 : Settings::ctlPosSize;
	for (size_t i = 0, n = m_buffer.size(); i < n; ++i)
	{
		PrimitiveDraw::drawPolyline(m_buffer[i].dst, Colorf(0.8f, 0.2f, 0.2f), false);
		PrimitiveDraw::drawCircles(m_buffer[i].dst, radius, true, 2, Colorf(0.2f, 0.2f, 0.8f));
	}

	return false;
}

bool EditNodesOP::clear()
{
	if (SelectNodesOP::clear()) return true;

	m_lastPos.setInvalid();
	m_buffer.clear();

	return false;
}

void EditNodesOP::simplify(float threshold)
{
	m_buffer.clear();
	for (size_t i = 0, n = m_nodeSelection.size(); i < n; ++i)
	{
		Modified modified;
		modified.src = m_nodeSelection[i];
		DouglasPeucker::implement(modified.src->selectedNodes, threshold, modified.dst);
		m_buffer.push_back(modified);
	}
}

void EditNodesOP::smooth(float samplingWidth)
{
	m_buffer.clear();
	for (size_t i = 0, n = m_nodeSelection.size(); i < n; ++i)
	{
		Modified modified;
		modified.src = m_nodeSelection[i];
		CosineSmooth::implement(modified.src->selectedNodes, samplingWidth, modified.dst);
		m_buffer.push_back(modified);
	}
}

void EditNodesOP::updateModified()
{
	std::vector<ChainShape*> chains;
	std::vector<std::vector<Vector> > chainsDst;

	for (size_t i = 0, n = m_buffer.size(); i < n; ++i)
	{
		const std::vector<Vector>& chain = m_buffer[i].src->chain->getVertices();
		const std::vector<Vector>& src = m_buffer[i].src->selectedNodes;
		const std::vector<Vector>& dst = m_buffer[i].dst;

		size_t begin = 0, end = chain.size() - 1;
		for (size_t j = 0, m = chain.size(); j < m; ++j)
		{
			if (src.front() == chain[j])
			{
				begin = j;
				break;
			}
		}
		for (int j = chain.size() - 1; j >= 0; --j)
		{
			if (src.back() == chain[j])
			{
				end = j;
				break;
			}
		}

		assert(begin <= end);
		std::vector<Vector> result;
		copy(chain.begin(), chain.begin() + begin, back_inserter(result));
		copy(dst.begin(), dst.end(), back_inserter(result));
		copy(chain.begin() + end + 1, chain.end(), back_inserter(result));

		m_buffer[i].src->chain->setVertices(result);

		chains.push_back(m_buffer[i].src->chain);
		chainsDst.push_back(result);
	}

	m_editPanel->addHistoryOP(new edit_nodes::ModifyNodesAOP(chains, chainsDst));

	m_buffer.clear();
	clearSelectedNodes();
}
