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

#include "CosineSmooth.h"

#include <cmath>

#include "common/Math.h"

using namespace d2d;

void CosineSmooth::implement(const std::vector<Vector>& src, float samplingWidth, 
							 std::vector<Vector>& dst)
{
	dst.clear();

	if (src.size() < 2) return;

	for (size_t i = 0, n = src.size() - 1; i < n; ++i)
	{
		const Vector& p0 = src[i],
			p1 = src[i+1];
		const size_t samplingCount = static_cast<size_t>(std::floor(std::abs(p1.x - p0.x) / samplingWidth));
		const float dx = (p1.x - p0.x) / samplingCount;
		const float da = PI / samplingCount;
		const float yMid = (p0.y + p1.y) * 0.5f;
		const float ampl = (p0.y - p1.y) * 0.5f;
		dst.push_back(p0);
		for (size_t j = 1; j < samplingCount; ++j)
		{
			Vector pt;
			pt.x = p0.x + j * dx;
			pt.y = yMid + ampl * cos(da * j);
			dst.push_back(pt);
		}
	}
	dst.push_back(src.back());
}
