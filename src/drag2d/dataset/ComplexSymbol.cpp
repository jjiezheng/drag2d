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
#include "ISprite.h"
#include "SymbolMgr.h"
#include "SpriteFactory.h"

#include "common/ComplexFileAdapter.h"
#include "dataset/Bitmap.h"
#include "dataset/AbstractBV.h"
#include "render/SpriteDraw.h"

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
}

void ComplexSymbol::draw(const ISprite* sprite/* = NULL*/) const
{
	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		SpriteDraw::drawSprite(m_sprites[i]);
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

	for (size_t i = 0, n = m_sprites.size(); i < n; ++i)
		SpriteDraw::drawSprite(m_sprites[i], memDC);

	memDC.SelectObject(wxNullBitmap);
}