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

#ifndef D2D_PRIMITIVE_DRAW_H
#define D2D_PRIMITIVE_DRAW_H

#include <vector>

#include "common/Color.h"

namespace d2d
{
	class Vector;
	struct Colorf;

	class PrimitiveDraw
	{
	public:
		static void resetColorAndTexture();

		static void drawRect(const Vector& center, float radius, 
			bool isFill = false, float size = 2, const Colorf& color = Colorf(0, 0, 0));
		static void drawRect(const Vector& center, float hWidth, float hHeight, 
			bool isFill = false, float size = 2, const Colorf& color = Colorf(0, 0, 0));
		static void drawRect(const Vector& p0, const Vector& p1,
			bool isFill = false, float size = 2, const Colorf& color = Colorf(0, 0, 0));

		static void drawCircle(const Vector& center, float radius, bool isFill = false, 
			float size = 2, const Colorf& color = Colorf(0, 0, 0), size_t kSegments = 16);
		static void drawCircles(const std::vector<Vector>& circles, float radius, bool isFill = false, 
			float size = 2, const Colorf& color = Colorf(0, 0, 0), size_t kSegments = 16);

		static void drawPoints(const std::vector<Vector>& vertices, const Colorf& color, float size = 2);
		static void drawLine(const Vector& p0, const Vector& p1, const Colorf& color, float size = 2);
		static void drawDotLine(const Vector& p0, const Vector& p1, const Colorf& color, float size = 2);
		static void drawDashLine(const Vector& p0, const Vector& p1, const Colorf& color, float size = 2);
		static void drawDotDashLine(const Vector& p0, const Vector& p1, const Colorf& color, float size = 2);
		static void drawLines(const std::vector<Vector>& vertices, const Colorf& color, float size = 2);
		static void drawPolyline(const std::vector<Vector>& vertices, const Colorf& color, bool isClose, float size = 2);
		static void drawPolygon(const std::vector<Vector>& vertices, const Colorf& color);
		static void drawTriangles(const std::vector<Vector>& triangles, const Colorf& color);
		// 		static void drawTriangles(const std::vector<Vector>& triangles, const wxString& filepath);
		// 		static void drawTriangles(const std::vector<Vector>& triangles, const wxString& filepath, 
		// 			const Vector& leftLow, const std::vector<unsigned int>& mesh);
		static void drawTriangles(unsigned int texID, const std::vector<Vector>& triangles, 
			const std::vector<Vector>& texCoords);
		static void drawTrianglesSlow(unsigned int texID, const std::vector<Vector>& triangles, 
			const std::vector<Vector>& texCoords);

	}; // PrimitiveDraw
}

#endif // D2D_PRIMITIVE_DRAW_H
