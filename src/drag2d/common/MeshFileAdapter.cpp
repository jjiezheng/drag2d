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

#include "MeshFileAdapter.h"

#include <fstream>

#include "common/FileNameTools.h"
#include "common/tools.h"

using namespace d2d;

void MeshFileAdapter::load(const char* filename)
{
	std::ifstream fin(filename);

	m_imagePath = StringTools::getLine(fin);
	m_imagePath = FilenameTools::getExistFilepath(m_imagePath);

	size_t size;
	fin >> size;

	m_vertices.clear();
	m_vertices.reserve(size);
	for (size_t i = 0; i < size; ++i)
	{
		Vector p;
		fin >> p;
		m_vertices.push_back(p);
	}

	m_texCoords.clear();
	m_texCoords.reserve(size);
	for (size_t i = 0; i < size; ++i)
	{
		Vector p;
		fin >> p;
		m_texCoords.push_back(p);
	}

	fin.close();
}
