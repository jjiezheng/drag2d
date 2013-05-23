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

#include "MeshSymbol.h"

#include "render/PrimitiveDraw.h"
#include "common/MeshFileAdapter.h"
#include "common/FileNameTools.h"

using namespace d2d;

MeshSymbol::MeshSymbol()
	: m_image(NULL)
{
}

MeshSymbol::~MeshSymbol()
{

}

MeshSymbol* MeshSymbol::clone() 
{ 
	return NULL; 
}

void MeshSymbol::loadFromTextFile(std::ifstream& fin)
{
}

void MeshSymbol::storeToTextFile(std::ofstream& fout) const
{
}

void MeshSymbol::reloadTexture() const
{
	m_image->reload();
}

void MeshSymbol::draw(const ISprite* sprite/* = NULL*/) const
{
//	PrimitiveDraw::drawTriangles(m_image->textureID(), m_vertices, m_texCoords);

	PrimitiveDraw::drawTrianglesSlow(m_image->textureID(), m_vertices, m_texCoords);
}

float MeshSymbol::getWidth(const ISprite* sprite/* = NULL*/) const
{
	return 0;
}

float MeshSymbol::getHeight(const ISprite* sprite/* = NULL*/) const
{
	return 0;
}

const std::vector<Vector>& MeshSymbol::getVertices() const
{
	return m_vertices;
}

void MeshSymbol::loadResources()
{
	d2d::MeshFileAdapter adapter;
	adapter.load(m_filepath.c_str());

	wxString filepath = FilenameTools::getExistFilepath(adapter.m_imagePath);

	m_image = ImageMgr::Instance()->getItem(filepath);

	m_vertices = adapter.m_vertices;
	m_texCoords = adapter.m_texCoords;
}
