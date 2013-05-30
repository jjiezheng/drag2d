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

#include "SymbolFactory.h"
#include "ImageSymbol.h"
#include "ShapeSymbol.h"
#include "EShapeSymbol.h"
#include "MeshSymbol.h"
#include "CombinationSymbol.h"
#include "FontSymbol.h"

#include "common/FileNameTools.h"
#include "common/FileNameParser.h"

using namespace d2d;

ISymbol* SymbolFactory::create(const wxString& filepath)
{
	ISymbol* symbol = NULL;

	wxString ext = FilenameTools::getExtension(filepath).Lower();

	if (ext == "png" || ext == "jpg" || ext == "bmp")
	{
		symbol = new ImageSymbol;
	}
	else if (ext == "ttf")
	{
		symbol = new FontSymbol;
	}
	else if (ext == "txt")
	{
		if (FileNameParser::isType(filepath, FileNameParser::e_polygon))
			symbol = new ShapeSymbol;
		else if (FileNameParser::isType(filepath, FileNameParser::e_mesh))
			symbol = new MeshSymbol;
		else if (FileNameParser::isType(filepath, FileNameParser::e_combination))
			symbol = new CombinationSymbol;
	}
	else if (ext == "json")
	{
		if (FileNameParser::isType(filepath, FileNameParser::e_shape))
			symbol = new EShapeSymbol;
	}

	return symbol;
}
