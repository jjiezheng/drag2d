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

#ifndef D2D_CHAIN_SHAPE_H
#define D2D_CHAIN_SHAPE_H

#include <vector>

#include "IShape.h"

#include "common/Rect.h"

namespace d2d
{
	class ChainShape : public IShape
	{
	public:
		ChainShape();
		ChainShape(const ChainShape& chain);
		ChainShape(const std::vector<Vector>& vertices, bool isLoop);

		//
		// IObject interface
		//
		virtual ChainShape* clone();

		//
		// ISerializable interface
		//
		virtual void loadFromTextFile(std::ifstream& fin);
		virtual void storeToTextFile(std::ofstream& fout) const;

		//
		// f2Shape interface
		//
		virtual bool isContain(const Vector& pos);
		virtual bool isIntersect(const Rect& rect);

		//
		// IShape interface
		//
		virtual const Rect& getRect() const { return m_rect; }
		virtual void draw(const Colorf& color = Colorf(0, 0, 0)) const;

		virtual void insertVertices(size_t index, const Vector& pos);
		// todo: too slow! 
		// cache index or pointer? how to avoid iterator's invalid
		// maybe use list instead of array, but others' effective? 
 		virtual void removeVertices(const Vector& pos);
 		virtual void changeVertices(const Vector& from, const Vector& to);
		virtual void setVertices(const std::vector<Vector>& vertices);
		virtual void refresh() {}

		void setClosed(bool isClose) { m_isLoop = isClose; }
		bool isClosed() const { return m_isLoop; }

		const std::vector<Vector>& getVertices() const { return m_vertices; }

	private:
		void initBounding();

	protected:
		std::vector<Vector> m_vertices;
		bool m_isLoop;

		Rect m_rect;

	}; // ChainShape
}

#endif // D2D_CHAIN_SHAPE_H
