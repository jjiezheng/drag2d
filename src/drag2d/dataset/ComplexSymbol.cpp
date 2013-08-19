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

#include "ComplexSymbol.h"
#include "ComplexSprite.h"
#include "ISprite.h"
#include "SymbolMgr.h"
#include "SpriteFactory.h"

#include "common/ComplexFileAdapter.h"
#include "dataset/Bitmap.h"
#include "dataset/AbstractBV.h"
#include "render/SpriteDraw.h"

#include <queue>
#include <set>

using namespace d2d;

ComplexSymbol::ComplexSymbol()
{
	static int id = 0;
	m_name = wxT("complex") + wxVariant(id++);

	const float SCALE = 0.15f;
	const float WIDTH = 800, HEIGHT = 480;
	m_bitmap = new Bitmap(
		new wxBitmap(WIDTH * SCALE, HEIGHT * SCALE)
		);
}

ComplexSymbol::~ComplexSymbol()
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		m_sprites[i]->release();
	m_sprites.clear();
}

void ComplexSymbol::loadFromTextFile(std::ifstream& fin)
{
}

void ComplexSymbol::storeToTextFile(std::ofstream& fout) const
{
}

void ComplexSymbol::reloadTexture() const
{
	std::vector<std::pair<const ISprite*, d2d::Vector> > children;
	getAllChildren(children);
	
	std::set<const ISymbol*> symbols;
	for (size_t i = 0, n = children.size(); i < n; ++i)
		symbols.insert(&children[i].first->getSymbol());

	std::set<const ISymbol*>::iterator itr = symbols.begin();
	for ( ; itr != symbols.end(); ++itr)
		(*itr)->reloadTexture();
}

void ComplexSymbol::draw(const ISprite* sprite/* = NULL*/) const
{
// 	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
// 		SpriteDraw::drawSprite(m_sprites[i]);

	//////////////////////////////////////////////////////////////////////////

// 	std::vector<const ISprite*> children;
// 	getAllChildren(children);
// 	for (size_t i = 0, n = children.size(); i < n; ++i)
// 		SpriteDraw::drawSprite(children[i]);

	//////////////////////////////////////////////////////////////////////////

	std::vector<std::pair<const ISprite*, d2d::Vector> > children;
	getAllChildren(children);
	for (size_t i = 0, n = children.size(); i < n; ++i)
	{
		const ISprite* child = children[i].first;

		SpriteDraw::drawSprite(&child->getSymbol(), child->getPosition() + children[i].second, 
			child->getAngle(), child->getScale());
	}
}

float ComplexSymbol::getWidth(const ISprite* sprite/* = NULL*/) const
{
	return 0;
}

float ComplexSymbol::getHeight(const ISprite* sprite/* = NULL*/) const
{
	return 0;
}

void ComplexSymbol::refresh()
{
	ISymbol::refresh();
	refreshThumbnail();
	initBounding();
}

bool ComplexSymbol::isOneLayer() const
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		if (dynamic_cast<d2d::ComplexSprite*>(m_sprites[i]))
			return false;
	return true;
}

void ComplexSymbol::loadResources()
{
	ComplexFileAdapter adapter;
	adapter.load(m_filepath.c_str());

	for (size_t i = 0, n = adapter.m_data.size(); i < n; ++i)
	{
		ISprite* sprite = NULL;

		const ComplexFileAdapter::Entry& entry = adapter.m_data[i];
		ISymbol* symbol = SymbolMgr::Instance()->getSymbol(entry.filepath);
		sprite = SpriteFactory::create(symbol);

		sprite->setTransform(entry.pos, entry.angle);
		sprite->setScale(entry.scale);
		sprite->setMirror(entry.xMirror, entry.yMirror);

		m_sprites.push_back(sprite);
	}

	initBounding();
}

void ComplexSymbol::initBounding()
{
	m_rect.makeInfinite();
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
	{
		std::vector<Vector> vertices;
		m_sprites[i]->getBounding()->getBoundPos(vertices);
		for (size_t j = 0, m = vertices.size(); j < m; ++j)
			m_rect.combine(vertices[j]);
	}
}

void ComplexSymbol::refreshThumbnail()
{
	wxMemoryDC memDC;
	memDC.SelectObject(const_cast<wxBitmap&>(*m_bitmap->getBitmap()));

	memDC.SetBackground(wxBrush(*wxBLACK));
	//	memDC.SetBackground(wxBrush(m_scene->m_color/**wxRED*/));
	memDC.Clear();

	std::vector<std::pair<const ISprite*, d2d::Vector> > children;
	getAllChildren(children);
	for (size_t i = 0, n = children.size(); i < n; ++i)
		SpriteDraw::drawSprite(children[i].first, children[i].second, memDC);

	memDC.SelectObject(wxNullBitmap);
}

void ComplexSymbol::getAllChildren(std::vector<std::pair<const ISprite*, d2d::Vector> >& children) const
{
	std::queue<std::pair<const ISprite*, d2d::Vector> > buffer;
	std::set<const ISprite*> unique;	// avoid complex symbol contain itself

	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		buffer.push(std::make_pair(m_sprites[i], d2d::Vector(0, 0)));
	while (!buffer.empty())
	{
		std::pair<const ISprite*, d2d::Vector> pairSprite = buffer.front(); buffer.pop();
		const ComplexSprite* complex = dynamic_cast<const ComplexSprite*>(pairSprite.first);
		if (complex)
		{
			if (unique.find(pairSprite.first) == unique.end())
			{
				unique.insert(pairSprite.first);
				for (size_t i = 0, n = complex->getSymbol().m_sprites.size(); i < n; ++i)
				{
					ISprite* child = complex->getSymbol().m_sprites[i];
					buffer.push(std::make_pair(child, pairSprite.second + complex->getPosition()));
				}
			}
		}
		else
		{
			children.push_back(pairSprite);
		}
	}
}