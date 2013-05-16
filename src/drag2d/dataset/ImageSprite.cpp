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

#include "ImageSprite.h"
#include "IBody.h"
#include "BodyFactory.h"

#include "common/FileNameParser.h"
#include "common/FilenameTools.h"
#include "common/Math.h"
#include "dataset/BVFactory.h"
#include "dataset/AbstractBV.h"

#include <Box2D/Box2D.h>

using namespace d2d;

ImageSprite::ImageSprite()
	: m_symbol(NULL)
{
}

ImageSprite::ImageSprite(const ImageSprite& sprite)
	: ISprite(sprite)
	, m_symbol(sprite.m_symbol)
{
}

ImageSprite::ImageSprite(ImageSymbol* symbol)
	: m_symbol(symbol)
{
	buildBounding();
}

ImageSprite* ImageSprite::clone()
{
	return new ImageSprite(*this);
}

const ImageSymbol& ImageSprite::getSymbol() const
{
	return *m_symbol;
}

void ImageSprite::setSymbol(ISymbol* symbol)
{
	ImageSymbol* image = dynamic_cast<ImageSymbol*>(symbol);
	if (image) m_symbol = image;
}

void ImageSprite::loadBodyFromFile()
{
	wxString path = FilenameTools::getFilePathExceptExtension(m_symbol->getFilepath());
	wxString polygonPath = path + "_" + FileNameParser::getFileTag(FileNameParser::e_polyline) + ".txt";
	wxString circlePath = path + "_" + FileNameParser::getFileTag(FileNameParser::e_circle) + ".txt";
	if (FilenameTools::isExist(polygonPath))
	{
		if (m_body) delete m_body;
		m_body = BodyFactory::createBody(polygonPath, m_scale);
	}
	else if (FilenameTools::isExist(circlePath))
	{
		if (m_body) delete m_body;
		m_body = BodyFactory::createBody(circlePath, m_scale);
	}
	m_body->getBody()->SetTransform(b2Vec2(m_pos.x / BOX2D_SCALE_FACTOR, m_pos.y / BOX2D_SCALE_FACTOR), m_angle);
}

void ImageSprite::buildBounding(float* texCoords)
{
	if (!m_symbol) return;

	const int width = m_symbol->getWidth(),
		height = m_symbol->getHeight();

	delete m_bounding;
	m_bounding = BVFactory::createBV(e_obb);

	Rect rect;
	rect.xMin = width * (texCoords[0] - 0.5f);
	rect.xMax = width * (texCoords[2] - 0.5f);
	rect.yMin = height * (texCoords[1] - 0.5f);
	rect.yMax = height * (texCoords[3] - 0.5f);

	Vector center(rect.xCenter(), rect.yCenter());
	float hWidth = (rect.xMax - rect.xMin) * 0.5f * m_scale,
		hHeight = (rect.yMax - rect.yMin) * 0.5f * m_scale;
	rect.xMin = -hWidth;
	rect.xMax = hWidth;
	rect.yMin = -hHeight;
	rect.yMax = hHeight;

	Vector offset = Math::rotateVector(center, m_angle);
	rect.translate(m_pos + offset);
	m_bounding->initFromRect(rect);
	m_bounding->rotate(m_angle);
}

void ImageSprite::buildBounding()
{
	if (!m_symbol) return;

	const float hWidth = m_symbol->getWidth() * 0.5f * m_scale,
		hHeight = m_symbol->getHeight() * 0.5f * m_scale;

	delete m_bounding;
	m_bounding = BVFactory::createBV(e_obb);
	Rect rect;
	rect.xMin = -hWidth;
	rect.xMax = hWidth;
	rect.yMin = -hHeight;
	rect.yMax = hHeight;
	rect.translate(m_pos);
	m_bounding->initFromRect(rect);
	m_bounding->rotate(m_angle);
}
