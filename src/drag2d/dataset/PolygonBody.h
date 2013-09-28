#pragma once

#include "IBody.h"

namespace d2d
{
	class BodyData;

	class PolygonBody : public IBody
	{
	public:
		PolygonBody(b2World* world, const BodyData& bodyData, float scale = 1.0f);
		PolygonBody(b2World* world, float hWidth, float hHeight, bool isStatic, float scale = 1.0f);
		
		virtual void getRect(Rect& rect, const Vector& position, float angle) const;

	}; // PolygonBody
}

