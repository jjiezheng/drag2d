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

#ifndef D2D_TOOLS_H
#define D2D_TOOLS_H

#ifndef _CRT_SECURE_NO_DEPRECATE
    #define _CRT_SECURE_NO_DEPRECATE
#endif
#ifndef _SCL_SECURE_NO_DEPRECATE
    #define _SCL_SECURE_NO_DEPRECATE
#endif

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cctype>

typedef __int8 int8_t;
typedef __int16 int16_t;
typedef __int32 int32_t;
typedef __int64 int64_t;
typedef unsigned __int8 uint8_t;
typedef unsigned __int16 uint16_t;
typedef unsigned __int32 uint32_t;
typedef unsigned __int64 uint64_t;

typedef uint8_t byte;

#define NOMINMAX

class StringTools
{
public:
	static std::string intToString(int i)
	{
		std::stringstream ss;
		ss << i;
		return ss.str();
	}

	static int stringToInt(const std::string& str)
	{
		std::stringstream ss(str);
		int ret;
		ss >> ret;
		return ret;
	}

	static void toLower(std::string& str)
	{
		std::transform(str.begin(), str.end(), str.begin(), std::tolower);
	}

	static void toUpper(std::string& str)
	{
		std::transform(str.begin(), str.end(), str.begin(), std::toupper);
	}

	static std::string getLine(std::ifstream& fin)
	{
		std::string line;
		while ((line.empty() || line == " ") && getline(fin, line))
			;
		return line;
	}

}; // StringTools

class STLTools
{
public:
	template<class T>
	static int getArrayIndex(const std::vector<T*>& array, T* item)
	{
		int index = -1;
		std::vector<T*>::const_iterator itr = array.begin();
		for (int i = 0; itr != array.end(); ++itr, ++i)
		{
			if (*itr == item)
			{
				index = i;
				break;
			}
		}
		return index;
	}

}; // STLTools

template<class T>
struct DeletePointerFunctor
{
	void operator()(const T* pT) const
	{
		delete pT, pT = NULL;
	}
};

////
//// Exceptions
////
//class Exception
//{
//public:
//	virtual std::string what() = 0;
//	virtual ~Exception() {}
//}; // Exception
//
//class IndexOutOfBoundsException : public Exception
//{
//public:
//	IndexOutOfBoundsException(size_t i) { m_error = StringTools::intToString(i); }
//	virtual ~IndexOutOfBoundsException() {}
//	virtual std::string what() { return "IndexOutOfBoundsException: " + m_error; }
//
//private:
//	std::string m_error;
//}; // IndexOutOfBoundsException
//
//class IllegalArgumentException : public Exception
//{
//public:
//	IllegalArgumentException(std::string s) : m_error(s) {}
//	virtual ~IllegalArgumentException() {}
//	virtual std::string what() { return "IllegalArgumentException: " + m_error; }
//
//private:
//	std::string m_error;
//}; // IllegalArgumentException
//
//class IllegalStateException : public Exception
//{
//public:
//	IllegalStateException(std::string s) : m_error(s) {}
//	virtual ~IllegalStateException() {}
//	virtual std::string what() { return "IllegalStateException: " + m_error + "\nPlease contact "/* + PACKAGE_BUGREPORT*/; }
//
//private:
//	std::string m_error;
//}; // IllegalStateException
//
//class NotSupportedException : public Exception
//{
//public:
//	NotSupportedException(std::string s) : m_error(s) {}
//	virtual ~NotSupportedException() {}
//	virtual std::string what() { return "NotSupportedException: " + m_error; }
//
//private:
//	std::string m_error;
//}; // NotSupportedException

#endif // D2D_TOOLS_H
