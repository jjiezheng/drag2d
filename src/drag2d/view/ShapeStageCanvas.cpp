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

#include "ShapeStageCanvas.h"

#include "dataset/ChainShape.h"
#include "dataset/CurveShape.h"
#include "dataset/PolygonShape.h"
#include "view/EditPanel.h"
#include "view/MultiShapesImpl.h"
#include "render/DrawShapesVisitor.h"

using namespace d2d;

ShapeStageCanvas::ShapeStageCanvas(EditPanel* editPanel, MultiShapesImpl* shapesImpl, 
								   const Colorf& color/* = Colorf(0.0f, 0.0f, 0.0f)*/)
	: GLCanvas(editPanel)
	, m_color(color)
	, m_shapesImpl(shapesImpl)
{
}

void ShapeStageCanvas::onDraw()
{
	m_shapesImpl->traverseShapes(DrawShapesVisitor(m_color), e_visible);
	m_editPanel->drawEditTemp();
}
