/*
* Copyright (c) 2012-2013 XZRUNNER http://runnersoft.net
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

#ifndef D2D_MESH_SPRITE_H
#define D2D_MESH_SPRITE_H

#include "ISprite.h"

#include "MeshSymbol.h"

namespace d2d
{
	class MeshSprite;

	class MeshSprite : public ISprite
	{
	public:
		MeshSprite();
		MeshSprite(const MeshSprite& sprite);
		MeshSprite(MeshSymbol* symbol);
		virtual ~MeshSprite();

		//
		// IObject interface
		//
		virtual MeshSprite* clone();

		//
		// Sprite interface
		//
		virtual const MeshSymbol& getSymbol() const;
		virtual void setSymbol(ISymbol* symbol);
		virtual void loadBodyFromFile();

	protected:
		virtual void buildBounding();

	private:
		MeshSymbol* m_symbol;

	}; // MeshSprite
}

#endif // D2D_MESH_SPRITE_H
