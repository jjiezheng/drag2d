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

#include "TexturePackerFileAdapter.h"

#include <fstream>

#include <drag2d.h>

using namespace d2d;

void TexturePackerFileAdapter::load(const char* filename)
{
	Json::Value value;
	Json::Reader reader;
	std::ifstream fin(filename);
	reader.parse(fin, value);
	fin.close();

	width = value["meta"]["size"]["w"].asInt();
	height = value["meta"]["size"]["h"].asInt();

	int i = 0;
	Json::Value frameValue = value["frames"][i++];
	while (!frameValue.isNull()) {
		Entry entry;
		load(frameValue, entry);
		frames.push_back(entry);
		frameValue = value["frames"][i++];
	}
}

void TexturePackerFileAdapter::load(const Json::Value& value, Entry& entry)
{
	entry.filename = value["filename"].asString();
	StringTools::toLower(entry.filename);
	load(value["frame"], entry.frame);
	entry.rotated = value["rotated"].asBool();
	entry.trimmed = value["trimmed"].asBool();
	load(value["spriteSourceSize"], entry.spriteSourceSize);
	entry.srcWidth = value["sourceSize"]["w"].asInt();
	entry.srcHeight = value["sourceSize"]["h"].asInt();
}

void TexturePackerFileAdapter::load(const Json::Value& value, Region& region)
{
	region.x = value["x"].asInt();
	region.y = value["y"].asInt();
	region.w = value["w"].asInt();
	region.h = value["h"].asInt();
}