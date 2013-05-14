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

#include "ImageSymbol.h"
#include "Bitmap.h"

using namespace d2d;

ImageSymbol::ImageSymbol()
	: m_image(NULL)
{
}

ImageSymbol::~ImageSymbol()
{

}

ImageSymbol* ImageSymbol::clone()
{
	return NULL;
}

void ImageSymbol::loadFromTextFile(std::ifstream& fin)
{
}

void ImageSymbol::storeToTextFile(std::ofstream& fout) const
{
}

void ImageSymbol::reloadTexture() const
{
	m_image->reload();
}

void ImageSymbol::draw() const
{
	m_image->draw();
}

float ImageSymbol::getWidth() const
{
	return m_image->width();
}

float ImageSymbol::getHeight() const
{
	return m_image->height();
}

unsigned int ImageSymbol::getTextureID() const
{
	return m_image->textureID();
}

void ImageSymbol::loadResources()
{
	const Bitmap* init = BitmapMgr::Instance()->getItem(m_filepath);
	if (m_bitmap != init) delete m_bitmap;
	m_bitmap = const_cast<Bitmap*>(init);

	m_image = ImageMgr::Instance()->getItem(m_filepath);
}
