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

#include "EditNodesAtomicOP.h"

#include "dataset/ChainShape.h"

using namespace d2d;
using namespace d2d::edit_nodes;

ModifyNodesAOP::ModifyNodesAOP(const std::vector<ChainShape*>& chains, 
							   const std::vector<std::vector<Vector> >& dstChains)
{
	m_chains.reserve(chains.size());
	for (size_t i = 0, n = chains.size(); i < n; ++i)
	{
		chains[i]->retain();
		m_chains.push_back(chains[i]);

		m_src.push_back(chains[i]->getVertices());
		m_dst.push_back(dstChains[i]);
	}
}

void ModifyNodesAOP::undo()
{
	for (size_t i = 0, n = m_chains.size(); i< n; ++i)
		m_chains[i]->setVertices(m_src[i]);
}

void ModifyNodesAOP::redo()
{
	for (size_t i = 0, n = m_chains.size(); i< n; ++i)
		m_chains[i]->setVertices(m_dst[i]);
}
