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

#ifndef D2D_LAYER_H
#define D2D_LAYER_H

#include <wx/wx.h>
#include <vector>

#include "common/interface.h"

namespace d2d
{	
	class ISprite;
	class IShape;

	class Layer : public ICloneable
 	{
 	public:
 		Layer();
 		~Layer();

		//
		// ICloneable interface
		//
		virtual ICloneable* clone() { return NULL; }

		const wxString& getName() const { return m_name; }
		void setName(const wxString& name);

		// use IVisitor if has other implementation
		const std::vector<ISprite*>& getSprites() const { return m_sprites; }
		const std::vector<IShape*>& getShapes() const { return m_shapes; }

		void traverseSprites(IVisitor& visitor) const;
		void traverseShapes(IVisitor& visitor) const;

		void insert(ISprite* shape);
		void remove(ISprite* shape);
		void resetOrder(ISprite* sprite, bool up);

		void insert(IShape* sprite);
		void remove(IShape* sprite);

 		void clear();
 
 	private:
		wxString m_name;

 		std::vector<ISprite*> m_sprites;
 		std::vector<IShape*> m_shapes;
 
 	}; // Layer
}

#endif // D2D_LAYER_H
