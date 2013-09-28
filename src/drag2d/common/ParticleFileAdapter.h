#pragma once

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

