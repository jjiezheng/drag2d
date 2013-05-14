/*
* Copyright (c) 2012-2013 XZRUNNER http://runnersoft.net
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

#include "Camera.h"

#include <common/interface.h>

using namespace d2d;

Camera::Camera(float scale/* = 1.0f*/)
{
	m_scale = scale;
}

const Vector& Camera::getCenter() const
{
	return m_center;
}

void Camera::setCenter(const Vector& center)
{
	m_center = center;
	notifyObservers();
}

float Camera::getScale() const
{
	return m_scale;
}

void Camera::setScale(float scale)
{
	m_scale = scale;
	notifyObservers();
}

void Camera::setScale(float scale, int x, int y, int width, int height)
{
	m_center.x = (x - (width >> 1)) * m_scale + m_center.x - (x - (width >> 1)) * scale;
	m_center.y = (y - (height >> 1)) * m_scale + m_center.y - (y - (height >> 1)) * scale;
	m_scale = scale;
	notifyObservers();
}

Vector Camera::transPosScreenToProject(int x, int y, int width, int height) const
{
	Vector proj;
	const int xView = x, yView = height - y;
	proj.x = (xView - (width >> 1)) * m_scale + m_center.x;
	proj.y = (yView - (height >> 1)) * m_scale + m_center.y;
	return proj;
}

// Vector Camera::transPosProjectToScreen(const Vector& proj, int width, int height) const
// {
// 	Vector scr;
// 	int xView = (proj.x - m_center.x) / m_scale + (width >> 1),
// 		yView = (proj.y - m_center.y) / m_scale + (height >> 1);
// 	scr.x = xView;
// 	scr.y = height - yView;
// 	return scr;
// }

void Camera::addObserver(ICameraObserver* observer)
{
	m_observers.push_back(observer);
}

void Camera::notifyObservers()
{
	for (size_t i = 0, n = m_observers.size(); i < n; ++i)
		m_observers[i]->onCameraChanged();
}
