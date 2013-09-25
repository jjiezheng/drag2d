#include "DialogStageCanvas.h"

#include "view/EditPanel.h"
#include "view/LibraryPanel.h"

using namespace d2d;

DialogStageCanvas::DialogStageCanvas(EditPanel* editPanel, LibraryPanel* library)
	: OrthoCanvas(editPanel)
	, m_library(library)
{
}

void DialogStageCanvas::initGL()
{
	d2d::OrthoCanvas::initGL();
	if (m_library)
		m_library->reloadTexture();
}

void DialogStageCanvas::onDraw()
{
	m_editPanel->drawEditTemp();
}
