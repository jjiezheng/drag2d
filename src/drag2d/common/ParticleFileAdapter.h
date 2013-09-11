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

#ifndef D2D_PARTICLE_FILE_ADAPTER_H
#define D2D_PARTICLE_FILE_ADAPTER_H

#include "IFileAdapter.h"

#include <json/json.h>

namespace d2d
{
	class ParticleFileAdapter
	{
	public:
		virtual void load(const char* filename);

	public:
		struct Child
		{
			std::string filepath;

			std::string name;

			float start_scale, end_scale;

			float min_rotate, max_rotate;

			float start_z;
		};

	public:
		std::string name;
		std::string package;

		int count;
		int layer;
		float emission_time;

		float min_life, max_life;

		float min_hori, max_hori;
		float min_vert, max_vert;

		float min_spd, max_spd;

		float gravity;

		float inertia;

		float fadeout_time;

		bool bounce;

		bool additive_blend;

		float start_radius;

		bool orient_to_movement;

		bool orient_to_parent;

		std::vector<Child> children;

	}; // ParticleFileAdapter
}

#endif // D2D_PARTICLE_FILE_ADAPTER_H