/*
* Copyright (c) 2012-2013 XZRUNNER http://runnersoft.net
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

#include "MeshSprite.h"
#include "IBody.h"
#include "BodyFactory.h"

#include <Box2D/Box2D.h>

#include "common/Math.h"
#include "dataset/BVFactory.h"
#include "dataset/AbstractBV.h"

using namespace d2d;

MeshSprite::MeshSprite()
{
}

MeshSprite::MeshSprite(const MeshSprite& sprite)
	: ISprite(sprite)
	, m_symbol(sprite.m_symbol)
{
}

MeshSprite::MeshSprite(MeshSymbol* symbol)
	: m_symbol(symbol)
{
	buildBounding();
}

MeshSprite::~MeshSprite()
{
}

MeshSprite* MeshSprite::clone()
{
	return new MeshSprite(*this);
}

const MeshSymbol& MeshSprite::getSymbol() const
{
	return *m_symbol;
}

void MeshSprite::setSymbol(ISymbol* symbol)
{
	MeshSymbol* mesh = dynamic_cast<MeshSymbol*>(symbol);
	if (mesh) m_symbol = mesh;
}

void MeshSprite::loadBodyFromFile()
{
	if (!m_symbol) return;

	if (m_body) delete m_body;
	m_body = BodyFactory::createBody(m_symbol->getVertices(), m_scale);

	m_body->getBody()->SetTransform(b2Vec2(m_pos.x / BOX2D_SCALE_FACTOR, m_pos.y / BOX2D_SCALE_FACTOR), m_angle);
}

void MeshSprite::buildBounding()
{
	delete m_bounding;
	m_bounding = BVFactory::createBV(e_obb);

	if (m_symbol)
	{
		Rect rect;
		const std::vector<Vector>& vertices = m_symbol->getVertices();
		for (size_t i = 0, n = vertices.size(); i < n; ++i)
			rect.combine(vertices[i]);

		rect *= m_scale;
		rect.translate(m_pos);
		m_bounding->initFromRect(rect);
		m_bounding->rotate(m_angle);
	}
}
