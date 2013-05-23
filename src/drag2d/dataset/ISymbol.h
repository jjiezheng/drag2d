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

#ifndef D2D_INTERFACE_SYMBOL_H
#define D2D_INTERFACE_SYMBOL_H

#include "widgets/ListItem.h"
#include "common/ResourcesMgr.h"

namespace d2d
{
	class ISprite;

	class ISymbol : public ListItem, public ISerializable
	{
	public:
		virtual ~ISymbol() {}

		//
		// IObject interface
		//	
		virtual ISymbol* clone() { return NULL; }

		virtual void reloadTexture() const = 0;
		virtual void draw(const ISprite* sprite = NULL) const = 0;
		virtual float getWidth(const ISprite* sprite = NULL) const = 0;
		virtual float getHeight(const ISprite* sprite = NULL) const = 0;

		bool loadFromFile(const wxString& filepath);

		const wxString& getFilepath() const { 
			return m_filepath; 
		}

	protected:
		virtual void loadResources() = 0;

	protected:
		wxString m_filepath;

	}; // ISymbol
}

#endif // D2D_INTERFACE_SYMBOL_H
