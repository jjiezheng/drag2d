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

#ifndef D2D_CAMERA_H
#define D2D_CAMERA_H

#include <vector>

#include "common/Vector.h"

namespace d2d
{
	class ICameraObserver;

	class Camera
	{
	public:
		Camera(float scale = 1.0f);

		const Vector& getCenter() const;
		void setCenter(const Vector& center);

		float getScale() const;
		void setScale(float scale);
		void setScale(float scale, int x, int y, int width, int height);

		Vector transPosScreenToProject(int x, int y, int width, int height) const;
//		Vector transPosProjectToScreen(const Vector& proj, int width, int height) const;

		// For reset projection transform (gluOrtho2D)
		void addObserver(ICameraObserver* observer);

	private:
		void notifyObservers();

	private:
		Vector m_center;
		float m_scale;

		std::vector<ICameraObserver*> m_observers;

	}; // Camera
}

#endif // D2D_CAMERA_H
