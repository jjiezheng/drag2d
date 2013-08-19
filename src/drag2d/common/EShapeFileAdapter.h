/*
* Copyright (c) 2013 Guang Zhu http://runnersoft.net
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

#ifndef D2D_ESHAPE_FILE_ADAPTER_H
#define D2D_ESHAPE_FILE_ADAPTER_H

#include <json/json.h>

#include "IFileAdapter.h"

namespace d2d
{
	class IShape;
	class BezierShape;
	class PolygonShape;
	class ChainShape;
	class RectShape;
	class CircleShape;

	class EShapeFileAdapter : public IFileAdapter
	{
	public:
		EShapeFileAdapter(std::vector<IShape*>& shapes);

		virtual void load(const char* filename);
		virtual void store(const char* filename);

		static IShape* loadShape(const Json::Value& value);
		static Json::Value store(IShape* shape);

	private:
		static IShape* loadBezier(const Json::Value& value);
		static IShape* loadPolygon(const Json::Value& value);
		static IShape* loadChain(const Json::Value& value);
		static IShape* loadRect(const Json::Value& value);
		static IShape* loadCircle(const Json::Value& value);

		static Json::Value store(const BezierShape* bezier);
		static Json::Value store(const PolygonShape* poly);
		static Json::Value store(const ChainShape* chain);
		static Json::Value store(const RectShape* rect);
		static Json::Value store(const CircleShape* circle);

	public:
		std::vector<IShape*>& m_shapes;

	}; // EShapeFileAdapter
}

#endif // D2D_ESHAPE_FILE_ADAPTER_H