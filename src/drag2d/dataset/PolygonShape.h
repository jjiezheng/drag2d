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

#ifndef D2D_POLYGON_SHAPE_H
#define D2D_POLYGON_SHAPE_H

#include "ChainShape.h"

#include <wx/wx.h>

namespace d2d
{
	class ImageSymbol;

	class PolygonShape : public ChainShape
	{
	public:
		enum FillingType
		{
			e_Color = 0,
			e_Texture,
		};

	public:
		PolygonShape(const PolygonShape& polygon);
		PolygonShape(const std::vector<Vector>& vertices);
		virtual ~PolygonShape();
		
		//
		// IObject interface
		//
		virtual PolygonShape* clone();

		//
		// IShape interface
		//
		virtual void draw(const Colorf& color = Colorf(0, 0, 0)) const;

		//
		// ChainShape interface
		//
		virtual void refresh();

	public:
		void buildFillingTris();
		void buildFillingTris(const std::vector<Vector>& segments);

	private:
		void getBoundingRegion(const std::vector<Vector>& bounding, float& left, float& right, float& low, float& up);
		void getTextureBoundarySegments(float left, float right, float low, float up,
			std::vector<Vector>& segments);
		void computeTextureCoords(float left, float low);

	public:
		int m_fillingType;

		Colorf m_fillingColor;
		ImageSymbol* m_fillingTexture;

		std::vector<Vector> m_fillingVertices;
		std::vector<Vector> m_fillingTexCoords;

	}; // PolygonShape
}

#endif // D2D_POLYGON_SHAPE_H
