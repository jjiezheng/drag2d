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

#ifndef D2D_INTERFACE_SHAPE_H
#define D2D_INTERFACE_SHAPE_H

#include "common/interface.h"
#include "common/UserDataImpl.h"
#include "common/Object.h"
#include "common/Color.h"

namespace d2d
{
	class Vector;
	class Rect;

	class IShape : public ICloneable, public UserDataImpl, public Object, public ISerializable
	{
	public:
		IShape();
		virtual ~IShape() { clearUserData(true); }

		//
		// UserDataImpl interface
		//	
		virtual void clearUserData(bool deletePtr) {}

		//
		// ICloneable interface
		//
		virtual IShape* clone() { return NULL; }

		virtual bool isContain(const Vector& pos) const = 0;
		virtual bool isIntersect(const Rect& rect) const = 0;

		virtual const Rect& getRect() const = 0;

		virtual void draw(const Colorf& color = Colorf(0, 0, 0)) const = 0;

	protected:
		static const int QUERY_ACCURACY = 5;

	public:
		std::string name;

	}; // IShape
}

#endif // D2D_INTERFACE_SHAPE_H
