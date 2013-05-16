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

#include "Image.h"

#include "render/GL10.h"

#include <SOIL/SOIL.h>

using namespace d2d;

Image::Image()
{
	m_textureID = 0;
	m_width = m_height = 0;
}

bool Image::loadFromFile(const wxString& filepath)
{
	m_filepath = filepath;

	reload();

	//  	LibJpeg::ImageData data;
	//  	LibJpeg::read_JPEG_file(m_filepath.c_str(), data);
	//  	m_textureID = SOIL_internal_create_OGL_texture(
	//  		data.pixels, data.width, data.height, data.channels,
	//  		m_textureID, SOIL_FLAG_INVERT_Y,
	//  		GL_TEXTURE_2D, GL_TEXTURE_2D,
	//  		GL_MAX_TEXTURE_SIZE );
	//  	delete[] data.pixels;

	if (m_textureID == 0)
	{
		m_width = m_height = 0;
		return false;
	}
	else
	{
		GL10::BindTexture(GL10::GL_TEXTURE_2D, m_textureID);
		GL10::GetTexLevelParameteriv(GL10::GL_TEXTURE_2D, 0, GL10::GL_TEXTURE_WIDTH, &m_width);
		GL10::GetTexLevelParameteriv(GL10::GL_TEXTURE_2D, 0, GL10::GL_TEXTURE_HEIGHT, &m_height);
		GL10::BindTexture(GL10::GL_TEXTURE_2D, NULL);
		return true;
	}
}

void Image::reload() const
{
	m_textureID = SOIL_load_OGL_texture
		(
		m_filepath.c_str(),
		SOIL_LOAD_AUTO,
		m_textureID,
		SOIL_FLAG_INVERT_Y
		);
}

void Image::draw() const
{
	GL10::Enable(GL10::GL_BLEND);
	GL10::BlendFunc(GL10::GL_SRC_ALPHA, GL10::GL_ONE_MINUS_SRC_ALPHA);

	GL10::Color4f(1.0f, 1.0f, 1.0f, 1.0f);

	const float hWidth = m_width * 0.5f,
		hHeight = m_height * 0.5f;

	GL10::BindTexture(GL10::GL_TEXTURE_2D, m_textureID);
	GL10::Begin(GL10::GL_QUADS);
		GL10::TexCoord2f(0, 0); GL10::Vertex3f(-hWidth, -hHeight, -1.0f);
		GL10::TexCoord2f(1, 0); GL10::Vertex3f( hWidth, -hHeight, -1.0f);
		GL10::TexCoord2f(1, 1); GL10::Vertex3f( hWidth,  hHeight, -1.0f);
		GL10::TexCoord2f(0, 1); GL10::Vertex3f(-hWidth,  hHeight, -1.0f);
	GL10::End();
	GL10::BindTexture(GL10::GL_TEXTURE_2D, NULL);

	GL10::Disable(GL10::GL_BLEND);
}
