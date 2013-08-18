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

#include "SymbolMgr.h"
#include "SymbolFactory.h"
#include "ISymbol.h"

using namespace d2d;

SymbolMgr* SymbolMgr::m_instance = NULL;

SymbolMgr::SymbolMgr()
{
}

SymbolMgr::~SymbolMgr()
{
	clear();
}

SymbolMgr* SymbolMgr::Instance()
{
	if (!m_instance)
	{
		m_instance = new SymbolMgr();
	}
	return m_instance;
}

ISymbol* SymbolMgr::getSymbol(const wxString& filepath)
{
	std::map<wxString, ISymbol*>::iterator itr = m_symbols.find(filepath);
	if (itr == m_symbols.end())
	{
		ISymbol* symbol = SymbolFactory::create(filepath);
		bool isLoaded = symbol->loadFromFile(filepath);
		if (isLoaded)
		{
			symbol->refresh();
			m_symbols.insert(std::make_pair(filepath, symbol));
			return symbol;
		}
		else
		{
			delete symbol;
			return NULL;
		}
	}
	else
	{
		return itr->second;
	}
}

void SymbolMgr::clear()
{
	std::map<wxString, ISymbol*>::iterator itr = m_symbols.begin();
	for ( ; itr != m_symbols.end(); ++itr)
		delete itr->second;
	m_symbols.clear();
}

void SymbolMgr::traverse(IVisitor& visitor) const
{
	std::map<wxString, ISymbol*>::const_iterator itr = m_symbols.begin();
	for ( ; itr != m_symbols.end(); ++itr)
	{
		bool hasNext;
		visitor.visit(itr->second, hasNext);
		if (!hasNext) break;
	}
}

