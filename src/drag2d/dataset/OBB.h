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

#ifndef D2D_OBB_H
#define D2D_OBB_H

#include "AbstractBV.h"

#include "common/Rect.h"

namespace d2d
{
	class OBB : public AbstractBV
	{
	public:
		OBB();
		OBB(const OBB& obb);

		//
		// IObject interface
		//
		virtual OBB* clone();

		//
		// ISerializable interface
		//
		virtual void loadFromTextFile(std::ifstream& fin);
		virtual void storeToTextFile(std::ofstream& fout) const;

		virtual void initFromRect(const Rect& rect);

		virtual bool isValid() const;
		virtual void makeInfinite();

		virtual void translate(const Vector& offset);
		virtual void rotate(float delta);

		virtual void combine(const Vector& pos);
		virtual void combine(const Rect& rect);

		virtual bool isContain(const Vector& pos) const;
		virtual bool isIntersect(const Rect& rect) const;

		virtual float area() const;
		virtual float width() const;
		virtual float height() const;
		virtual Vector center() const;

		virtual void getBoundPos(std::vector<Vector>& bound) const;

		const Rect& getRect() const;
		float getAngle() const;

	private:
		Rect transToRect() const;

	private:
		Rect m_rect;
		float m_angle;

	}; // OBB

	inline void OBB::initFromRect(const Rect& rect)
	{
		m_rect = rect;
		m_angle = 0;
	}

	inline bool OBB::isValid() const
	{
		return m_rect.isValid();
	}

	inline void OBB::makeInfinite()
	{
		m_rect.makeInfinite();
	}

	inline void OBB::combine(const Vector& pos)
	{
		if (m_rect.isValid())
		{
			m_rect = transToRect();
			m_angle = 0;
		}
		m_rect.combine(pos);
	}

	inline void OBB::combine(const Rect& rect)
	{
		if (m_rect.isValid())
		{
			m_rect = transToRect();
			m_angle = 0;
		}
		m_rect.combine(rect);
	}

	inline const Rect& OBB::getRect() const
	{
		return m_rect;
	}

	inline float OBB::getAngle() const
	{
		return m_angle;
	}
}

#endif // D2D_OBB_H
