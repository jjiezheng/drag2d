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

#ifndef D2D_SGI_H
#define D2D_SGI_H

#include <vector>

#include "common/Vector.h"

namespace d2d
{
	class SGI
	{
	public:

		static void implement(const std::vector<Vector>& src,
			std::vector<std::vector<Vector> >& dst);

	private:
		struct Edge;

		struct Triangle
		{
			bool bUsed;

			Vector nodes[3];

			// [0] v0-v1, [1] v1-v2, [2] v2-v0
			Edge* edges[3];

			Triangle() {
				bUsed = false;
				edges[0] = edges[1] = edges[2] = NULL;
			}
		};

		struct Edge
		{
			Vector start, end;

			Triangle *left, *right;

			Edge() {
				left = right = NULL;
			}
		};

	private:
		static void insertEdge(std::vector<Edge*>& edges, Triangle* tri,
			int index);

		static void traversal(Triangle* tri, std::vector<Vector>& strip, int level = 0);

		static int getDegree(Triangle* tri, int level);

		static Triangle* getMinDegreeTri(const std::vector<Triangle*>& tris);

	}; // SGI
}

#endif // D2D_SGI_H
