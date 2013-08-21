#include "DialogStageCanvas.h"

#include "view/EditPanel.h"
#include "view/LibraryPanel.h"

using namespace d2d;

DialogStageCanvas::DialogStageCanvas(EditPanel* editPanel, LibraryPanel* library)
	: GLCanvas(editPanel)
	, m_library(library)
{
}

void DialogStageCanvas::initGL()
{
	d2d::GLCanvas::initGL();
	m_library->reloadTexture();
}

void DialogStageCanvas::onDraw()
{
	m_editPanel->drawEditTemp();
}
