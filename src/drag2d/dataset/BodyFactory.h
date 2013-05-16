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

#ifndef D2D_BODY_FACTORY_H
#define D2D_BODY_FACTORY_H

#include "FixtureData.h"

#include <wx/wx.h>
#include <vector>

class b2World;

namespace d2d
{
	class IBody;

	class BodyFactory
	{
	public:
		static IBody* createBody(const wxString& filename, float scale = 1.0f);
		static IBody* createBody(const std::vector<Vector>& triangles, float scale = 1.0f);
		static IBody* createBody(const std::vector<FixtureDataInfo>& fixtures, float scale = 1.0f);

		static void setWorld(b2World* world);

	private:
		static b2World* m_world;

	}; // BodyFactory

	inline void BodyFactory::setWorld(b2World* world)
	{
		m_world = world;
	}
}

#endif // D2D_BODY_FACTORY_H
