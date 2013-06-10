/*
* Copyright (c) 2013 Guang Zhu http://runnersoft.net
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

#include "TexPackerAdapter.h"

#include "FileNameTools.h"

#include <fstream>

using namespace d2d;

void TexPackerAdapter::load(const char* filename)
{
	Json::Value value;
	Json::Reader reader;
	std::ifstream fin(filename);
	reader.parse(fin, value);
	fin.close();

	width = value["width"].asInt();
	height = value["height"].asInt();

	std::string dlg = FilenameTools::getFileDir(filename);

	int i = 0;
	Json::Value texValue = value["image"][i++];
	while (!texValue.isNull()) {
		load(texValue, dlg);
		texValue = value["image"][i++];
	}
}

void TexPackerAdapter::load(const Json::Value& value, const std::string& dlg)
{
	Texture texture;

	texture.filepath = value["filepath"].asString();
	if (!d2d::FilenameTools::isExist(texture.filepath))
		texture.filepath = d2d::FilenameTools::getAbsolutePath(dlg, texture.filepath);

	texture.region.left = value["left"].asInt();
	texture.region.low = value["low"].asInt();
	texture.region.width = value["width"].asInt();
	texture.region.height = value["height"].asInt();

	texture.bRotate = value["rotate"].asBool();

	textures.push_back(texture);
}