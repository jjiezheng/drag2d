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

#ifndef D2D_VISITORS_H
#define D2D_VISITORS_H

#include <wx/wx.h>
#include <vector>

#include "common/interface.h"

namespace d2d
{
	template <typename T>
	class FetchAllVisitor : public IVisitor
	{
	public:
		FetchAllVisitor(std::vector<T*>& result) 
			: m_result(result) {}

		virtual void visit(ICloneable* object, bool& bFetchNext)
		{
			T* item = dynamic_cast<T*>(object);
			if (item)
				m_result.push_back(item);
			bFetchNext = true;
		}

	private:
		std::vector<T*>& m_result;

	}; // FetchAllVisitor

	//	template <typename T>
	class CountVerifyVisitor : public IVisitor
	{
	public:
		CountVerifyVisitor(bool& valid, int num)
			: m_valid(valid), m_num(num), m_count(0) {}

		virtual ~CountVerifyVisitor() {
			m_valid = m_count == m_num;
		}

		void visit(ICloneable* object, bool& bFetchNext) {
			++m_count;
			bFetchNext = m_count <= m_num;
		}

	private:
		bool& m_valid;
		const int m_num;

		int m_count;

	}; // CountVerifyVisitor

	class EmptyVerifyVisitor : public IVisitor
	{
	public:
		EmptyVerifyVisitor(bool& valid) 
			: m_valid(valid) { m_valid = true; }

		void visit(ICloneable* object, bool& bFetchNext) {
			m_valid = false;
			bFetchNext = false;
		}

	private:
		bool& m_valid;

	}; // EmptyVerifyVisitor

	template <typename T>
	class GetNameVisitor : public IVisitor
	{
	public:
		GetNameVisitor(std::vector<wxString>& names)
			: m_names(names) {}

		virtual void visit(ICloneable* object, bool& bFetchNext)
		{
			T* item = dynamic_cast<T*>(object);
			if (item)
				m_names.push_back(item->getName());
			bFetchNext = true;
		}

	private:
		std::vector<wxString>& m_names;

	}; // GetNameVisitor
}

#endif // D2D_VISITORS_H
