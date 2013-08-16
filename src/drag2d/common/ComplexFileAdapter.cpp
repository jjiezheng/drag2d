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

#include "ComplexFileAdapter.h"
#include "FileNameTools.h"
#include "Context.h"

#include <fstream>

namespace d2d
{
	void ComplexFileAdapter::load(const char* filename)
	{
		Json::Value value;
		Json::Reader reader;
		std::ifstream fin(filename);
		reader.parse(fin, value);
		fin.close();

		int i = 0;
		Json::Value spriteValue = value["sprite"][i++];
		while (!spriteValue.isNull()) {
			m_data.push_back(load(spriteValue));
			spriteValue = value["sprite"][i++];
		}
	}

	ComplexFileAdapter::Entry ComplexFileAdapter::load(const Json::Value& value)
	{
		Entry entry;

		entry.filepath = d2d::FilenameTools::getAbsolutePath(
			Context::Instance()->getDir(), value["filepath"].asString());

		entry.pos.x = value["position"]["x"].asDouble();
		entry.pos.y = value["position"]["y"].asDouble();

		entry.angle = value["angle"].asDouble();

		entry.scale = value["scale"].asDouble();

		entry.xMirror = value["x mirror"].asDouble();
		entry.yMirror = value["y mirror"].asDouble();

		return entry;
	}
}