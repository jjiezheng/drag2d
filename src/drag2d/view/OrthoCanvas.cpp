#include "OrthoCanvas.h"
#include "Camera.h"

#include "view/EditPanel.h"

#include <gl/glu.h>

namespace d2d
{

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
} // d2d