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

#include "PerspectCanvas.h"

#include <gl/glu.h>

using namespace d2d;

GLfloat cameraPos[] = {0, -400.0f, 400.0f, 1.0f};

PerspectCanvas::PerspectCanvas(EditPanel* editPanel)
	: GLCanvas(editPanel)
{
}

void PerspectCanvas::onSize(int w, int h)
{
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

 	GLfloat fAspect = (GLfloat)w / (GLfloat)h;
 	gluPerspective(60.0f, fAspect, 1.0f, 1000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(cameraPos[0], cameraPos[1], cameraPos[2], 
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
}