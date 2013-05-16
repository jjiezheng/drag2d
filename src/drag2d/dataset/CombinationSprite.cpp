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

#include "CombinationSprite.h"
#include "IBody.h"
#include "BodyFactory.h"

#include "common/FileNameParser.h"
#include "common/FilenameTools.h"
#include "common/Math.h"
#include "dataset/BVFactory.h"
#include "dataset/AbstractBV.h"

#include <Box2D/Box2D.h>

using namespace d2d;

CombinationSprite::CombinationSprite()
{
}

CombinationSprite::CombinationSprite(const CombinationSprite& sprite)
	: ISprite(sprite)
	, m_symbol(sprite.m_symbol)
{
}

CombinationSprite::CombinationSprite(CombinationSymbol* symbol)
	: m_symbol(symbol)
{
	buildBounding();
}

CombinationSprite::~CombinationSprite()
{
}

CombinationSprite* CombinationSprite::clone()
{
	return new CombinationSprite(*this);
}

const CombinationSymbol& CombinationSprite::getSymbol() const
{
	return *m_symbol;
}

void CombinationSprite::setSymbol(ISymbol* symbol)
{
	CombinationSymbol* combination = dynamic_cast<CombinationSymbol*>(symbol);
	if (combination) m_symbol = combination;
}

void CombinationSprite::loadBodyFromFile()
{
	if (!m_symbol) return;

	std::vector<d2d::FixtureDataInfo> fixtures;
	const std::vector<ISprite*>& sprites = m_symbol->getChildren();
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
	{
		ISprite* sprite = sprites[i];

		d2d::FixtureDataInfo fixture;

		if (FileNameParser::isType(sprite->getSymbol().getFilepath(), FileNameParser::e_mesh))
		{
			fixture.filepath = sprite->getSymbol().getFilepath();
		}
		else
		{
			wxString path = FilenameTools::getFilePathExceptExtension(sprite->getSymbol().getFilepath());
			wxString polygonPath = path + "_" + FileNameParser::getFileTag(FileNameParser::e_polyline) + ".txt";
			wxString circlePath = path + "_" + FileNameParser::getFileTag(FileNameParser::e_circle) + ".txt";
			if (FilenameTools::isExist(polygonPath))
				fixture.filepath = polygonPath;
			else if (FilenameTools::isExist(circlePath))
				fixture.filepath = circlePath;
		}

		fixture.pos = sprite->getPosition();
		fixture.angle = sprite->getAngle();
		fixture.scale = sprite->getScale();

		fixtures.push_back(fixture);
	}

	if (!fixtures.empty())
	{
		if (m_body) delete m_body;
		m_body = BodyFactory::createBody(fixtures, m_scale);
		m_body->getBody()->SetTransform(b2Vec2(m_pos.x / BOX2D_SCALE_FACTOR, m_pos.y / BOX2D_SCALE_FACTOR), m_angle);
	}
}

void CombinationSprite::buildBounding()
{
	delete m_bounding;
	m_bounding = BVFactory::createBV(e_obb);

	if (m_symbol)
	{
		Rect rect = m_symbol->getRect();

		rect *= m_scale;
		rect.translate(m_pos);
		m_bounding->initFromRect(rect);
		m_bounding->rotate(m_angle);
	}
}
