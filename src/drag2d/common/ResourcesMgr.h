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

#ifndef D2D_RESOURCES_MGR_H
#define D2D_RESOURCES_MGR_H

#include <wx/wx.h>
#include <map>

namespace d2d
{
	class IVisitor;

	template<class T>
	class ResourcesMgr
	{
	public:
		static ResourcesMgr* Instance();

		T* getItem(const wxString& filename);

		void clear();

		void traverse(IVisitor& visitor) const;

	protected:
		ResourcesMgr();
		~ResourcesMgr();

	private:
		static ResourcesMgr<T>* m_instance;

		std::map<wxString, T*> m_items;

	}; // ResourcesMgr

	template<class T>
	ResourcesMgr<T>* ResourcesMgr<T>::m_instance = NULL;

	template<class T>
	inline ResourcesMgr<T>* ResourcesMgr<T>::Instance()
	{
		if (!m_instance)
		{
			m_instance = new ResourcesMgr<T>;
		}
		return m_instance;
	}

	template<class T>
	inline T* ResourcesMgr<T>::getItem(const wxString& filename)
	{
		std::map<wxString, T*>::iterator itr = m_items.find(filename);
		if (itr == m_items.end())
		{
			T* item = new T;
			bool loaded = item->loadFromFile(filename);
			if (loaded)
			{
				m_items.insert(std::make_pair(filename, item));
				return item;
			}
			else
			{
				delete item;
				return NULL;
			}
		}
		else
		{
			return itr->second;
		}
	}

	template<class T>
	inline void ResourcesMgr<T>::clear()
	{
		std::map<wxString, T*>::iterator itr = m_items.begin();
		for ( ; itr != m_items.end(); ++itr)
			delete itr->second;
		m_items.clear();
	}

	template<class T>
	inline void ResourcesMgr<T>::traverse(IVisitor& visitor) const
	{
		std::map<wxString, T*>::const_iterator itr = m_items.begin();
		for ( ; itr != m_items.end(); ++itr)
		{
			bool hasNext;
			visitor.visit(itr->second, hasNext);
			if (!hasNext) break;
		}
	}

	template<class T>
	ResourcesMgr<T>::ResourcesMgr()
	{
	}

	template<class T>
	ResourcesMgr<T>::~ResourcesMgr()
	{
		clear();
	}
}

#endif // D2D_RESOURCES_MGR_H
