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

#ifndef D2D_RANDOM_H
#define D2D_RANDOM_H

#include <time.h>

namespace d2d
{
	class Random
	{
	public:
		template<class T>
			static void RandomPermutation(std::vector<T>& src);

		static float getNum(float min, float max);
		static float getNum0To1();
		static float getRadian();

	}; // Random

	template<class T>
	void Random::RandomPermutation(std::vector<T>& src)
	{
		srand((unsigned)time(NULL));
		size_t range = src.size() - 1;

		for(size_t i = src.size() - 1; i > 1; --i) {
			size_t rndIndex = int(__int64(range) * rand() / (RAND_MAX + 1.0 ));
			std::swap(src.at(i), src.at(rndIndex));
		}
	}

	inline float Random::getNum(float min, float max)
	{
		return (rand() / (float(RAND_MAX)+1) * (max - min)) + min;
	}

	inline float Random::getNum0To1()
	{
		return rand() / static_cast<float>(RAND_MAX);
	}

	inline float Random::getRadian()
	{
		return 3.14f * 2 * getNum0To1();
	}
}

#endif // D2D_RANDOM_H
