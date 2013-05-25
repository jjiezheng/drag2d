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

#include "DrawPolygonCMPT.h"
#include "UniversalCMPT.h"

#include "common/visitors.h"
#include "common/Math.h"
#include "dataset/SymbolMgr.h"
#include "dataset/ImageSymbol.h"
#include "dataset/PolygonShape.h"
#include "operator/EditPolylineWithCopyNodeOP.h"
#include "operator/SelectNodesOP.h"
#include "view/MultiShapesImpl.h"

#include <wx/colordlg.h>

using namespace d2d;

DrawPolygonCMPT::DrawPolygonCMPT(wxWindow* parent, const wxString& name,
								 EditPanel* editPanel, MultiShapesImpl* shapesImpl,
								 wxColourData& colorData)
	: NodeCaptureCMPT<EditPolylineOP<DrawPolygonEdgeOP, SelectShapesOP> >(parent, name, editPanel, shapesImpl)
	, m_shapesImpl(shapesImpl)
	, m_color(*wxBLACK)
	, m_colorData(colorData)
{
	m_editOP = NULL;

	addChild(new UniversalCMPT(this, wxT("直接画线"), editPanel, 
		new EditPolylineOP<DrawPolygonEdgeOP, SelectShapesOP>(editPanel, shapesImpl, this)));
	addChild(new UniversalCMPT(this, wxT("选节点画线"), editPanel, 
		new EditPolylineWithCopyNodeOP<DrawPolygonEdgeOP>(editPanel, shapesImpl)));
}

void DrawPolygonCMPT::updateControlValue()
{
	bool empty;
	m_shapesImpl->getShapeSelection()->traverse(EmptyVerifyVisitor(empty));
	m_btnTrigger->Enable(!empty);
}

wxSizer* DrawPolygonCMPT::initLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	sizer->Add(NodeCaptureCMPT<EditPolylineOP<DrawPolygonEdgeOP, SelectShapesOP> >::initLayout());

	sizer->AddSpacer(20);

	sizer->Add(initChildrenLayout());

	sizer->AddSpacer(20);

	sizer->Add(initPreviewButtonPanel());

	sizer->AddSpacer(10);

	m_btnTrigger = new wxButton(this, wxID_ANY, wxT("设置"));
	Connect(m_btnTrigger->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DrawPolygonCMPT::onTriggerFillingColor));
	sizer->Add(m_btnTrigger);
	m_btnTrigger->Enable(false);

	return sizer;
}

wxSizer* DrawPolygonCMPT::initPreviewButtonPanel()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	wxArrayString choices;
	choices.Add(wxT("颜色"));
	choices.Add(wxT("纹理"));
	m_fillingTypeChoice = new wxRadioBox(this, wxID_ANY, wxT("填充方式"), wxDefaultPosition, wxDefaultSize, choices, 1, wxRA_SPECIFY_COLS);
	Connect(m_fillingTypeChoice->GetId(), wxEVT_COMMAND_RADIOBOX_SELECTED, wxCommandEventHandler(DrawPolygonCMPT::onChangeFillingType));
	sizer->Add(m_fillingTypeChoice);

	sizer->AddSpacer(10);

	m_btnReview = new wxButton(this, wxID_ANY);	
	Connect(m_btnReview->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(DrawPolygonCMPT::onSetColor));
	sizer->Add(m_btnReview);
	fillingButton();

	return sizer;
}

void DrawPolygonCMPT::onSetColor(wxCommandEvent& event)
{
	if (m_fillingTypeChoice->GetSelection() == 0)
	{
		wxColourDialog dialog(this, &m_colorData);
		dialog.SetTitle(wxT("选择颜色"));
		if (dialog.ShowModal() == wxID_OK)
		{
			m_color = dialog.GetColourData().GetColour();
			m_colorData = dialog.GetColourData();
		}
	}
	else
	{
		wxFileDialog dlg(this, wxT("选择纹理"), wxEmptyString, wxEmptyString, wxT("*.png;*.jpg"), wxFD_OPEN);
		if (dlg.ShowModal() == wxID_OK)
		{
			m_filePath = dlg.GetPath();
			m_bitmap.loadFromFile(m_filePath);
		}
	}

	fillingButton();
}

void DrawPolygonCMPT::onChangeFillingType(wxCommandEvent& event)
{
	fillingButton();
}

void DrawPolygonCMPT::onTriggerFillingColor(wxCommandEvent& event)
{
	std::vector<PolygonShape*> polys;
	m_shapesImpl->getShapeSelection()->traverse(FetchAllVisitor<PolygonShape>(polys));

	for (size_t i = 0, n = polys.size(); i < n; ++i)
	{
		PolygonShape* poly = polys[i];
		if (m_fillingTypeChoice->GetSelection() == 0)
		{
			poly->m_fillingType = PolygonShape::e_Color;
			poly->m_fillingColor = Colorf(m_color.Red() / 255.0f, m_color.Green() / 255.0f, m_color.Blue() / 255.0f);
		}
		else
		{
			poly->m_fillingType = PolygonShape::e_Texture;
			poly->m_fillingTexture = static_cast<ImageSymbol*>(SymbolMgr::Instance()->getSymbol(m_filePath));
		}
		poly->buildFillingTris();

		m_editPanel->Refresh();
	}
}

void DrawPolygonCMPT::fillingButton()
{
	if (m_fillingTypeChoice->GetSelection() == 0)
	{
		wxImage img(m_btnReview->GetSize().GetWidth(), m_btnReview->GetSize().GetHeight());
		img.SetRGB(wxRect(m_btnReview->GetSize()), m_color.Red(), m_color.Green(), m_color.Blue());
		wxBitmap bitmap(img);
		m_btnReview->SetBitmap(bitmap);
	}
	else
	{
		if (const wxBitmap* bmp = m_bitmap.getBitmap())
			m_btnReview->SetBitmap(*bmp);
	}
}
