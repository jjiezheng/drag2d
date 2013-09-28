#include "DrawRectangleOP.h"

#include "render/PrimitiveDraw.h"

namespace d2d
{

DrawRectangleOP::DrawRectangleOP(EditPanel* editPanel,
								 const Colorf& color /*= Colorf(0, 0, 0)*/)
	: ZoomViewOP(editPanel, true)
	, m_color(color)
{
	m_firstPos.setInvalid();
	m_currPos.setInvalid();
}

bool DrawRectangleOP::onMouseLeftDown(int x, int y)
{
	if (ZoomViewOP::onMouseLeftDown(x, y)) return true;

	m_firstPos = m_editPanel->transPosScreenToProject(x, y);
	m_editPanel->Refresh();

	return false;
}

bool DrawRectangleOP::onMouseLeftUp(int x, int y)
{
	if (ZoomViewOP::onMouseLeftUp(x, y)) return true;

	m_firstPos.setInvalid();
	m_currPos.setInvalid();
	m_editPanel->Refresh();

	return false;
}

bool DrawRectangleOP::onMouseDrag(int x, int y)
{
	if (ZoomViewOP::onMouseDrag(x, y)) return true;

	if (m_firstPos.isValid())
	{
		m_currPos = m_editPanel->transPosScreenToProject(x, y);
		m_editPanel->Refresh();
	}

	return false;
}

bool DrawRectangleOP::onDraw() const
{
	if (ZoomViewOP::onDraw()) return true;

	if (m_firstPos.isValid() && m_currPos.isValid())
		PrimitiveDraw::drawRect(m_firstPos, m_currPos, false, 2, m_color);

	return false;
}

bool DrawRectangleOP::clear()
{
	if (ZoomViewOP::clear()) return true;

	m_firstPos.setInvalid();
	m_currPos.setInvalid();

	return false;
}

} // d2d