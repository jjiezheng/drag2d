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

#ifndef D2D_SPRITE_BATCH_H
#define D2D_SPRITE_BATCH_H

#include "VertexBuffer.h"

namespace d2d
{
	class ImageSprite;
	class Image;

	class SpriteBatch
	{
	public:
		enum UsageHint
		{
			USAGE_DYNAMIC = 1,
			USAGE_STATIC,
			USAGE_STREAM,
			USAGE_MAX_ENUM
		};

	public:
		SpriteBatch(int size, int usage);
		virtual ~SpriteBatch();

		int add(const ImageSprite* sprite, int index = -1);

		void* lock();
		void unlock();

		void onDraw() const;

		int getAndResetDrawTimes() {
			int times = m_drawTimes;
			m_drawTimes = 0;
			return times;
		}

		void clear() {
			m_next = 0;
			m_drawTimes = 0;
		}

	private:
		void createBuffer(int size, int usage);
		void fillIndicesBuffer(int size);

		void add(const Vertex* v, int index);

	private:
		static const int X_MIRROR = 0x0001;
		static const int Y_MIRROR = 0x0002;

	private:
		unsigned int m_textureID;

		int m_size;
		int m_next;

		mutable int m_drawTimes;

		float m_node[6];
		Vertex m_sprite[4];

		VertexBuffer* m_vertices;
		VertexBuffer* m_indices;

//		static void draw(const std::vector<ISprite*>& sprites) const;

	}; // SpriteBatch
}

#endif // D2D_SPRITE_BATCH_DRAW_H
