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

#ifndef D2D_SELECTION_SET_H
#define D2D_SELECTION_SET_H

#include <set>

#include "dataset/ISprite.h"
#include "dataset/IShape.h"

namespace d2d
{
	template<class T>
	class SelectionSet : public Object
	{
	public:
		virtual ~SelectionSet() {}

		size_t size() const;
		bool empty() const;

		void clear();

		bool isExist(T* item) const;
		void insert(T* item);
		void erase(T* item);

		void traverse(IVisitor& visitor) const;

	private:
		std::set<T*> m_items;

	}; // SelectionSet
	
	template<class T>
	inline size_t SelectionSet<T>::size() const
	{
		return m_items.size();
	}

	template<class T>
	inline bool SelectionSet<T>::empty() const
	{
		return m_items.empty();
	}

	template<class T>
	inline void SelectionSet<T>::clear()
	{
		m_items.clear();
	}

	template<class T>
	inline bool SelectionSet<T>::isExist(T* sprite) const
	{
		return m_items.find(sprite) != m_items.end();
	}

	template<class T>
	inline void SelectionSet<T>::insert(T* sprite)
	{
		m_items.insert(sprite);
	}

	template<class T>
	inline void SelectionSet<T>::erase(T* sprite)
	{
		m_items.erase(sprite);
	}

	template<class T>
	inline void SelectionSet<T>::traverse(IVisitor& visitor) const
	{
		std::set<T*>::const_iterator itr = m_items.begin();
		for ( ; itr != m_items.end(); ++itr)
		{
			bool hasNext;
			visitor.visit(*itr, hasNext);
			if (!hasNext) break;
		}
	}

	typedef SelectionSet<ISprite> SpriteSelection;
	typedef SelectionSet<IShape> ShapeSelection;
	typedef SelectionSet<Vector> NodeSelection;
}

#endif // D2D_SELECTION_SET_H
