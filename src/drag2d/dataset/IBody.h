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

#ifndef D2D_INTERFACE_BODY_H
#define D2D_INTERFACE_BODY_H

#include <wx/wx.h>

#include "common/interface.h"

class b2World;
class b2Body;
struct b2Filter;

namespace d2d
{
	class Rect;
	class Vector;

	class IBody : public ICloneable
	{
	public:
		virtual ~IBody();

		//
		// ICloneable interface
		//
		virtual IBody* clone();

		virtual void getRect(Rect& rect, const Vector& position, float angle) const = 0;

		b2Body* getBody() const;
		void setBody(b2Body* body);

		const wxString& getFilename() const;

		bool isAlive() const;
		void setAlive(bool isAlive);

		void setDensity(float density);
		void setFriction(float friction);
		void setCollisionFilter(const b2Filter& filter);

		void setTransform(const Vector& pos, float angle);

	protected:
		IBody() {}
		IBody(b2World* world, float scale = 1.0f);

	protected:
		wxString m_filename;

		bool m_alive;

		b2World* m_world;
		b2Body* m_body;

	private:
		float m_scale;

	}; // IBody

	inline b2Body* IBody::getBody() const
	{
		return m_body;
	}

	inline void IBody::setBody(b2Body* body)
	{
		m_body = body;
	}

	inline const wxString& IBody::getFilename() const
	{
		return m_filename;
	}

	inline bool IBody::isAlive() const
	{
		return m_alive;
	}

	inline void IBody::setAlive(bool isAlive)
	{
		m_alive = isAlive;
	}
}

#endif // D2D_INTERFACE_BODY_H
