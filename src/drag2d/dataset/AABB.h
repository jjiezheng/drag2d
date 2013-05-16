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

#ifndef D2D_AABB_H
#define D2D_AABB_H

#include "AbstractBV.h"

#include "common/Rect.h"

namespace d2d
{
	class AABB : public AbstractBV
	{
	public:
		AABB();
		AABB(const AABB& aabb);

		//
		// IObject interface
		//
		virtual AABB* clone();

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

	private:
		Rect m_rect;

	}; // AABB

	inline void AABB::initFromRect(const Rect& rect)
	{
		m_rect = rect;
	}

	inline bool AABB::isValid() const
	{
		return m_rect.isValid();
	}

	inline void AABB::makeInfinite()
	{
		m_rect.makeInfinite();
	}

	inline void AABB::translate(const Vector& offset)
	{
		m_rect.translate(offset);
	}

	inline void AABB::rotate(float delta)
	{
		m_rect.makeInfinite();
	}

	inline void AABB::combine(const Vector& pos)
	{
		m_rect.combine(pos);
	}

	inline void AABB::combine(const Rect& rect)
	{
		m_rect.combine(rect);
	}

	inline const Rect& AABB::getRect() const
	{
		return m_rect;
	}
}

#endif // D2D_AABB_H
