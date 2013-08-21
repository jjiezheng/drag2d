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

#include "Patch9Symbol.h"
#include "ISprite.h"
#include "SymbolMgr.h"
#include "SpriteFactory.h"

#include "common/Patch9FileAdapter.h"
#include "common/FileNameTools.h"
#include "dataset/Bitmap.h"
#include "render/SpriteDraw.h"

using namespace d2d;

Patch9Symbol::Patch9Symbol()
{
	static int id = 0;
	m_name = wxT("patch") + wxVariant(id++);

	memset(m_sprites, 0, sizeof(int) * 9);

	m_width = m_height = 0;

	const float SCALE = 0.15f;
	const float WIDTH = 800, HEIGHT = 480;
	m_bitmap = new Bitmap(
		new wxBitmap(WIDTH * SCALE, HEIGHT * SCALE)
		);
}

Patch9Symbol::~Patch9Symbol()
{
	for (size_t i = 0; i < 3; ++i)
		for (size_t j = 0; j < 3; ++j)
		{
			if (!m_sprites[i][j]) continue;
			m_sprites[i][j]->release();
		}
	memset(m_sprites, 0, sizeof(int) * 9);
}

void Patch9Symbol::loadFromTextFile(std::ifstream& fin)
{
}

void Patch9Symbol::storeToTextFile(std::ofstream& fout) const
{
}

void Patch9Symbol::reloadTexture() const
{
	for (size_t i = 0; i < 3; ++i)
		for (size_t j = 0; j < 3; ++j)
		{
			if (!m_sprites[i][j]) continue;
			m_sprites[i][j]->getSymbol().reloadTexture();
		}
}

void Patch9Symbol::draw(const ISprite* sprite/* = NULL*/) const
{
	for (size_t i = 0; i < 3; ++i)
		for (size_t j = 0; j < 3; ++j)
		{
			if (!m_sprites[i][j]) continue;
			SpriteDraw::drawSprite(m_sprites[i][j]);
		}
}

float Patch9Symbol::getWidth(const ISprite* sprite/* = NULL*/) const
{
	return m_width;
}

float Patch9Symbol::getHeight(const ISprite* sprite/* = NULL*/) const
{
	return m_height;
}

void Patch9Symbol::refresh()
{
	ISymbol::refresh();
	refreshThumbnail();
	initBounding();
}

void Patch9Symbol::composeFromSprites(ISprite* sprites[3][3],
									  float width, float height)
{
	for (size_t i = 0; i < 3; ++i)
		for (size_t j = 0; j < 3; ++j)
			if (!sprites[i][j]) return;
	for (size_t i = 0; i < 3; ++i)
		for (size_t j = 0; j < 3; ++j)
		{
			if (m_sprites[i][j]) m_sprites[i][j]->release();
			m_sprites[i][j] = sprites[i][j]->clone();
		}
	
	m_width = width;
	m_height = height;

	composeFromSprites();
}

void Patch9Symbol::resize(float width, float height)
{
	m_width = width;
	m_height = height;

	composeFromSprites();
}

void Patch9Symbol::loadResources()
{
	Patch9FileAdapter adapter;
	adapter.load(m_filepath.c_str());

	std::string dlg = FilenameTools::getFileDir(m_filepath);

	m_width = adapter.width;
	m_height = adapter.height;
	for (size_t i = 0; i < 3; ++i)
	{
		for (size_t j = 0; j < 3; ++j)
		{
			const Patch9FileAdapter::Entry& entry = adapter.m_data[i*3+j];

			std::string filepath = entry.filepath;
			if (!FilenameTools::isExist(filepath))
				filepath = FilenameTools::getAbsolutePath(dlg, filepath);

			ISymbol* symbol = SymbolMgr::Instance()->getSymbol(filepath);
			ISprite* sprite = SpriteFactory::create(symbol);

			sprite->setTransform(entry.pos, entry.angle);
			sprite->setScale(entry.scale);
			sprite->setMirror(entry.xMirror, entry.yMirror);

			m_sprites[i][j] = sprite;
		}
	}
	composeFromSprites();
}

void Patch9Symbol::initBounding()
{
	
}

void Patch9Symbol::refreshThumbnail()
{
	wxMemoryDC memDC;
	memDC.SelectObject(const_cast<wxBitmap&>(*m_bitmap->getBitmap()));

	memDC.SetBackground(wxBrush(*wxWHITE));
	memDC.Clear();

	for (size_t i = 0; i < 3; ++i)
		for (size_t j = 0; j < 3; ++j)
			SpriteDraw::drawSprite(m_sprites[i][j], memDC);

	memDC.SelectObject(wxNullBitmap);
}

void Patch9Symbol::composeFromSprites()
{
	const float w0 = m_sprites[0][0]->getSymbol().getWidth(),
		w2 = m_sprites[0][2]->getSymbol().getWidth(),
		w1 = m_width - w0 - w2;
	const float h0 = m_sprites[0][0]->getSymbol().getHeight(),
		h2 = m_sprites[0][2]->getSymbol().getHeight(),
		h1 = m_height - h0 - h2;

	stretch(m_sprites[0][0], d2d::Vector(-w0*0.5f-w1*0.5f, -h0*0.5f-h1*0.5f), w0, h0);
	stretch(m_sprites[0][1], d2d::Vector(0.0f, -h0*0.5f-h1*0.5f), w1, h0);
	stretch(m_sprites[0][2], d2d::Vector(w1*0.5f+w2*0.5f, -h0*0.5f-h1*0.5f), w2, h0);

	stretch(m_sprites[1][0], d2d::Vector(-w0*0.5f-w1*0.5f, 0.0f), w0, h1);
	stretch(m_sprites[1][1], d2d::Vector(0.0f, 0.0f), w1, h1);
	stretch(m_sprites[1][2], d2d::Vector(w1*0.5f+w2*0.5f, 0.0f), w2, h1);

	stretch(m_sprites[2][0], d2d::Vector(-w0*0.5f-w1*0.5f, h1*0.5f+h2*0.5f), w0, h2);
	stretch(m_sprites[2][1], d2d::Vector(0.0f, h1*0.5f+h2*0.5f), w1, h2);
	stretch(m_sprites[2][2], d2d::Vector(w1*0.5f+w2*0.5f, h1*0.5f+h2*0.5f), w2, h2);
}

void Patch9Symbol::stretch(ISprite* sprite, const d2d::Vector& center, 
						  float width, float height)
{
	sprite->setTransform(center, sprite->getAngle());
	const float sw = sprite->getSymbol().getWidth(),
		sh = sprite->getSymbol().getHeight();
	sprite->setScale(width / sw, height / sh);
}