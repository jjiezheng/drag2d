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

#ifndef D2D_ABSTRACT_BV_H
#define D2D_ABSTRACT_BV_H

#include <vector>

#include "common/interface.h"

namespace d2d
{
	class Rect;
	class Vector;

	class AbstractBV : public ICloneable, public ISerializable
	{
	public:
		
		//
		// ICloneable interface
		//
		virtual AbstractBV* clone() { return NULL; }

		virtual void initFromRect(const Rect& rect) = 0;

		virtual bool isValid() const = 0;
		virtual void makeInfinite() = 0;

		virtual void translate(const Vector& offset) = 0;
		virtual void rotate(float delta) = 0;

		virtual void combine(const Vector& pos) = 0;
		virtual void combine(const Rect& rect) = 0;

		virtual bool isContain(const Vector& pos) const = 0;
		virtual bool isIntersect(const Rect& rect) const = 0;

		virtual float area() const = 0;
		virtual float width() const = 0;
		virtual float height() const = 0;
		virtual Vector center() const = 0;

		virtual void getBoundPos(std::vector<Vector>& bound) const = 0;

	}; // AbstractBV
}

#endif // D2D_ABSTRACT_BV_H
