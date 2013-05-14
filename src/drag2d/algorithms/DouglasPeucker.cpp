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

#include "DouglasPeucker.h"

#include "common/Math.h"

using namespace d2d;

void DouglasPeucker::implement(const std::vector<Vector>& line, float precision, std::vector<Vector>& dst)
{
	dst.clear();

	std::vector<bool> flag(line.size(), false);
	pointsReduction(line, precision, flag, 0, line.size() - 1);
	for (size_t i = 0, n = line.size(); i < n; ++i)
		if (flag[i]) dst.push_back(line[i]);
}

void DouglasPeucker::pointsReduction(const std::vector<Vector>& line, float precision, std::vector<bool>& flag,
									 size_t begin, size_t end)
{
	if ( begin > end )
		return;

	if ( begin == end )
	{
		flag[begin] = true;
		return;
	}

	if ( end - begin < 2 )
	{
		flag[begin] = true;
		flag[end] = true;
		return;
	}

	size_t savedPointIndex = 0;
	float max = - FLT_MAX;
	for (size_t i = begin + 1; i < end; ++i)
	{
		float dis = Math::getDisPointToSegment(line[i], line[begin], line[end]);
		if (dis > max)
		{
			max = dis;
			savedPointIndex = i;
		}
	}

	if (max < precision)
	{
		flag[begin] = true;
		flag[end] = true;
	}
	else
	{
		pointsReduction(line, precision, flag, begin, savedPointIndex);
		pointsReduction(line, precision, flag, savedPointIndex, end);
	}
}
