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

#ifndef D2D_COLOR_H
#define D2D_COLOR_H

#include <iostream>

namespace d2d
{
	struct Colorf
	{
		float r, g, b, a;

		Colorf() 
			: r(0), g(0), b(0), a(1) {}
		Colorf(float _r, float _g, float _b) 
			: r(_r), g(_g), b(_b), a(1) {}
		Colorf(float _r, float _g, float _b, float _a) 
			: r(_r), g(_g), b(_b), a(_a) {}

		const Colorf& operator = (const Colorf& color)
		{
			r = color.r;
			g = color.g;
			b = color.b;
			a = color.a;
			return *this;
		}

		void set(float pr, float pg, float pb, float pa) {
			r = pr;
			g = pg;
			b = pb;
			a = pa;
		}

		void set(float pr, float pg, float pb) {
			r = pr;
			g = pg;
			b = pb;
		}

		unsigned int pack() const {
			return ((int)(r * 255 + 0.5f) << 24) | ((int)(g * 255 + 0.5f) << 16) | ((int)(b * 255 + 0.5f) << 8) | ((int)(r * 255 + 0.5f));
		}
		void unpack(unsigned int color) {
			r = ((color & 0xff000000) >> 24) / 255.0f;
			g = ((color & 0x00ff0000) >> 16) / 255.0f;
			b = ((color & 0x0000ff00) >> 8) / 255.0f;
			a = ((color & 0x000000ff)) / 255.0f;
		}

	}; // Colorf

	inline std::istream& operator >> (std::istream& is, Colorf& color)
	{
		is >> color.r >> color.g >> color.b >> color.a;
		return is;
	}
	inline std::ostream& operator << (std::ostream& os, const Colorf& color)
	{
		os << color.r << " " << color.g << " " << color.b << " " << color.a;
		return os;
	}

	struct Colori
	{
		int r, g, b, a;

		void set(int pr, int pg, int pb, int pa) {
			r = pr;
			g = pg;
			b = pb;
			a = pa;
		}

		void set(const Colori& c) { set(c.r, c.g, c.b, c.a); }

	}; // Colori
}

#endif // D2D_COLOR_H
