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

#include "OrthoCanvas.h"
#include "Camera.h"

#include "view/EditPanel.h"

#include <gl/glu.h>

using namespace d2d;

OrthoCanvas::OrthoCanvas(EditPanel* editPanel)
	: GLCanvas(editPanel)
{
}

void OrthoCanvas::onSize(int w, int h)
{
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	const float hWidth = w * m_camera->getScale() * 0.5f,
		hHeight = h * m_camera->getScale() * 0.5f;
	gluOrtho2D(
		m_camera->getCenter().x - hWidth, 
		m_camera->getCenter().x + hWidth, 
		m_camera->getCenter().y - hHeight,
		m_camera->getCenter().y + hHeight
		);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}