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

#include "Layer.h"

#include "dataset/ISprite.h"
#include "dataset/IShape.h"

using namespace d2d;

Layer::Layer()
{
	static int count = 0;
	m_name = wxT("layer") + wxString::FromDouble(count++);
}

Layer::~Layer()
{
	clear();
}

void Layer::setName(const wxString& name)
{
	m_name = name;
}

void Layer::traverseSprites(IVisitor& visitor) const
{
	std::vector<ISprite*>::const_iterator itr = m_sprites.begin();
	for ( ; itr != m_sprites.end(); ++itr)
	{
		bool hasNext;
		visitor.visit(*itr, hasNext);
		if (!hasNext) break;
	}
}

void Layer::traverseShapes(IVisitor& visitor) const
{
	std::vector<IShape*>::const_iterator itr = m_shapes.begin();
	for ( ; itr != m_shapes.end(); ++itr)
	{
		bool hasNext;
		visitor.visit(*itr, hasNext);
		if (!hasNext) break;
	}
}

void Layer::insert(ISprite* sprite)
{
	m_sprites.push_back(sprite);
}

void Layer::remove(ISprite* sprite)
{
	std::vector<ISprite*>::iterator itr = m_sprites.begin();
	for ( ; itr != m_sprites.end(); ++itr)
	{
		if (*itr == sprite)
		{
			m_sprites.erase(itr);
			break;
		}
	}
}

void Layer::resetOrder(ISprite* sprite, bool up)
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
	{
		if (m_sprites[i] == sprite)
		{
			if (up && i != n - 1)
			{
				ISprite* tmp = m_sprites[i];
				m_sprites[i] = m_sprites[i+1];
				m_sprites[i+1] = tmp;
			}
			else if (!up && i != 0)
			{
				ISprite* tmp = m_sprites[i];
				m_sprites[i] = m_sprites[i-1];
				m_sprites[i-1] = tmp;
			}
		}
	}
}

void Layer::insert(IShape* shape)
{
	m_shapes.push_back(shape);
}

void Layer::remove(IShape* shape)
{
	std::vector<IShape*>::iterator itr = m_shapes.begin();
	for ( ; itr != m_shapes.end(); ++itr)
	{
		if (*itr == shape)
		{
			m_shapes.erase(itr);
			break;
		}
	}
}

void Layer::clear()
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		m_sprites[i]->release();
	m_sprites.clear();

	for (size_t i = 0, n = m_shapes.size(); i < n; ++i)
		m_shapes[i]->release();
	m_shapes.clear();
}
