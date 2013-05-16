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

#include "CombinationFileAdapter.h"
#include "FileNameTools.h"

#include <fstream>

#include "common/tools.h"

using namespace d2d;

void CombinationFileAdapter::load(const char* filename)
{
	m_data.clear();

	std::ifstream fin(filename);

	wxString dir = FilenameTools::getFileDir(filename) + "\\";

	size_t size;
	fin >> size;
	m_data.reserve(size);
	for (size_t i = 0; i < size; ++i)
	{
		Entry entry;

		entry.filepath = StringTools::getLine(fin);
		entry.filepath = FilenameTools::getExistFilepath(entry.filepath, dir);

		std::string strLine = StringTools::getLine(fin);
		std::stringstream ss(strLine);
		ss >> entry.pos >> entry.angle >> entry.scale >> entry.xMirror >> entry.yMirror;

		m_data.push_back(entry);
	}

	fin.close();
}
