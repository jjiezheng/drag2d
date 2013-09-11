#ifndef D2D_DIALOG_STAGE_CANVAS_H
#define D2D_DIALOG_STAGE_CANVAS_H

#include "OrthoCanvas.h"

namespace d2d
{
	class EditPanel;
	class LibraryPanel;

	class DialogStageCanvas : public OrthoCanvas
	{
	public:
		DialogStageCanvas(EditPanel* editPanel, LibraryPanel* library);

	protected:
		virtual void initGL();
		virtual void onDraw();

	private:
		LibraryPanel* m_library;

	}; // DialogStageCanvas
}

#endif // D2D_DIALOG_STAGE_CANVAS_H
