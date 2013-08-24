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

#include "FontBlankSymbol.h"

#include "common/Vector.h"
#include "render/PrimitiveDraw.h"

#include <json/json.h>

using namespace d2d;

FontBlankSymbol::FontBlankSymbol()
{
	static int id = 0;
	m_name = wxT("fontblank") + wxVariant(id++);

	align = size = 0;
	width = height = 100;
}

FontBlankSymbol::~FontBlankSymbol()
{
}

void FontBlankSymbol::loadFromTextFile(std::ifstream& fin)
{
}

void FontBlankSymbol::storeToTextFile(std::ofstream& fout) const
{
}

void FontBlankSymbol::reloadTexture() const
{
}

void FontBlankSymbol::draw(const ISprite* sprite/* = NULL*/) const
{
	PrimitiveDraw::drawRect(Vector(0, 0), width*0.5f, height*0.5f, true, 2, Colorf(0.7f, 0.7f, 0.7f, 0.7f));
}

float FontBlankSymbol::getWidth(const ISprite* sprite/* = NULL*/) const
{
	return width;
}

float FontBlankSymbol::getHeight(const ISprite* sprite/* = NULL*/) const
{
	return height;
}

void FontBlankSymbol::refresh()
{
}

void FontBlankSymbol::loadResources()
{
	Json::Value value;
	Json::Reader reader;
	std::ifstream fin(m_filepath.fn_str());
	reader.parse(fin, value);
	fin.close();

	name = value["name"].asString();
	font = value["font"].asString();
	color = value["color"].asString();
	align = value["align"].asDouble();
	size = value["size"].asDouble();
	width = value["width"].asDouble();
	height = value["height"].asDouble();
}