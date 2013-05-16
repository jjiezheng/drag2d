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

#ifndef D2D_EDIT_NODES_ATOMIC_OP_H
#define D2D_EDIT_NODES_ATOMIC_OP_H

#include <vector>

#include "common/Vector.h"

#include "AbstractAtomicOP.h"

namespace d2d
{
	class ChainShape;

	namespace edit_nodes
	{
		class ModifyNodesAOP : public AbstractAtomicOP
		{
		public:
			ModifyNodesAOP(const std::vector<ChainShape*>& chains, 
				const std::vector<std::vector<Vector> >& dstChains);

			virtual void undo();
			virtual void redo();

		private:
			std::vector<ChainShape*> m_chains;
			std::vector<std::vector<Vector> > m_src, m_dst;

		}; // ModifyNodesAOP
	}
}

#endif // D2D_EDIT_NODES_ATOMIC_OP_H
