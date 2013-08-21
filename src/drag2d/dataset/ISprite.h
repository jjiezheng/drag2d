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

#ifndef D2D_INTERFACE_SPRITE_H
#define D2D_INTERFACE_SPRITE_H

#include "common/interface.h"
#include "common/UserDataImpl.h"
#include "common/Object.h"
#include "common/Vector.h"

namespace d2d
{
	class Vector;
	class Rect;
	class ISymbol;
	class IBody;
	class AbstractBV;

	class ISprite : public ICloneable, public UserDataImpl, public Object
	{
	public:
		ISprite();
		ISprite(const ISprite& sprite);
		virtual ~ISprite();

		//
		// ICloneable interface
		//
		virtual ISprite* clone();

		//
		// UserDataImpl interface
		//	
		virtual void clearUserData(bool deletePtr);

		virtual const ISymbol& getSymbol() const = 0;
		virtual void setSymbol(ISymbol* symbol) = 0;
		virtual void loadBodyFromFile() = 0;

		virtual void setTransform(const Vector& position, float angle);
		void setScale(float xScale, float yScale);
		void setScale(float scale);

		bool isContain(const Vector& pos) const;
		bool isIntersect(const Rect& rect) const;

		void translate(const Vector& offset);
		void rotate(float delta);

		const Vector& getPosition() const;
		float getAngle() const;
		float getScaleX() const { return m_xScale; }
		float getScaleY() const { return m_yScale; }

		void setMirror(bool xMirror, bool yMirror) { m_xMirror = xMirror; m_yMirror = yMirror; }
		void getMirror(bool& xMirror, bool& yMirror) const { xMirror = m_xMirror; yMirror = m_yMirror; }

		AbstractBV* getBounding() const;

		void updateEachFrame();
		IBody* getBody() const;

		virtual void buildBounding() = 0;

	private:
		void afterScaleChanged();

	protected:
		Vector m_pos;
		float m_angle;
		float m_xScale, m_yScale;

		bool m_xMirror, m_yMirror;

		AbstractBV* m_bounding;

		IBody* m_body;

	}; // ISprite

	class SpriteCmp
	{
	public:
		enum Type
		{
			e_file,
			e_x,
			e_y
		};

	public:
		SpriteCmp(Type type = e_file);

		bool operator() (const ISprite* s0, const ISprite* s1) const;

	private:
		Type m_type;

	}; // SpriteCmp
}

#endif // D2D_INTERFACE_SPRITE_H
