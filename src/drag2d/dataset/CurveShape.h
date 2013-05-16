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

#ifndef D2D_CURVE_SHAPE_H
#define D2D_CURVE_SHAPE_H

#include "ChainShape.h"

namespace d2d
{
	class CurveShape : public ChainShape
	{
	public:
		CurveShape(const CurveShape& curve);
		CurveShape(const std::vector<Vector>& vertices);
		virtual ~CurveShape();

		//
		// IObject interface
		//
		virtual CurveShape* clone();

		//
		// IShape interface
		//
		virtual void draw(const Colorf& color = Colorf(0, 0, 0)) const;

		//
		// ChainShape interface
		//
		virtual void insertVertices(size_t index, const Vector& pos);
		virtual void removeVertices(const Vector& pos);
		virtual void changeVertices(const Vector& from, const Vector& to);
		virtual void refresh();

		const std::vector<Vector>& getMidPoints() const { return m_midPoints; }
		void setMidPoints();

	private:
		static const float SAMPLING_WIDTH;

	private:
		std::vector<Vector> m_midPoints;

	}; // CurveShape
}

#endif // D2D_CURVE_SHAPE_H
