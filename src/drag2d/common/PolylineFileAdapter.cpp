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

#include "PolylineFileAdapter.h"

#include <fstream>

#include "dataset/ChainShape.h"

using namespace d2d;

PolylineFileAdapter::PolylineFileAdapter(std::vector<ChainShape*>& chains)
	: m_chains(chains)
{
}

void PolylineFileAdapter::load(const char* filename)
{
	std::ifstream fin(filename);

	std::string line;
	getline(fin, line);
	m_imgPath = line;

	size_t size;
	fin >> size;
	for (size_t i = 0; i < size; ++i)
	{
		ChainShape* chain = new ChainShape();
		chain->loadFromTextFile(fin);
		m_chains.push_back(chain);
	}

	fin.close();
}

void PolylineFileAdapter::store(const char* filename)
{
	std::ofstream fout(filename);

	fout << m_imgPath << '\n';
	fout << m_chains.size() << '\n';
	for (size_t i = 0, n = m_chains.size(); i < n; ++i)
		m_chains[i]->storeToTextFile(fout);

	fout.close();
}
