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

#ifndef D2D_BODY_DATA_H
#define D2D_BODY_DATA_H

#include "common/ResourcesMgr.h"

#include <vector>
#include <Box2D/Box2D.h>

#include "common/Vector.h"

namespace d2d
{
	class BodyData;
	typedef ResourcesMgr<BodyData> BodyDataMgr;

	class BodyData
	{
	public:
		enum Type
		{
			e_chain		= 0,
			e_polygon	= 1,
			e_circle	= 2,
			e_mesh		= 3,
		};

		struct FixtureData
		{
			std::vector<Vector> vertices;
			float density, friction, restitution;

			FixtureData() {
				density = 1;
				friction = 0.5f;
				restitution = 0.7f;
			}

			void setFixtureDef(b2FixtureDef& fixtureDef) const {
				fixtureDef.density = density;
				fixtureDef.friction = friction;
				fixtureDef.restitution = restitution;
			}

		}; // FixtureData

	public:
		BodyData();
		~BodyData();

		Type type() const;

		const std::vector<FixtureData*>& getAllFixturesData() const;

		const wxString& getFilename() const;

		void insertFixture(FixtureData* fixtureData) { 
			m_fixtures.push_back(fixtureData); 
		}

	private:
		void clear();

		bool loadFromFile(const wxString& filename);

		void loadFromMeshFile(const wxString& filename);
		void loadFromPolygonFile(const wxString& filename);
		void loadFromCircleFile(const wxString& filename);

	private:
		wxString m_filename;

		Type m_type;

		std::vector<FixtureData*> m_fixtures;

		friend class ResourcesMgr<BodyData>;

	}; // BodyData

	inline BodyData::Type BodyData::type() const
	{	
		return m_type;
	}

	inline const std::vector<BodyData::FixtureData*>& BodyData::getAllFixturesData() const
	{
		return m_fixtures;
	}

	inline const wxString& BodyData::getFilename() const
	{
		return m_filename;
	}
}

#endif // D2D_BODY_DATA_H
