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

#include "EditPolylinesCMPT.h"

#include "common/Math.h"
#include "common/visitors.h"
#include "dataset/ChainShape.h"
#include "operator/EditPolylinesOP.h"
#include "view/MultiShapesImpl.h"

#include <wx/spinctrl.h>

using namespace d2d;

EditPolylinesCMPT::EditPolylinesCMPT(wxWindow* parent, const wxString& name,
									 EditPanel* editPanel, MultiShapesImpl* shapesImpl,
									 PropertySettingPanel* propertyPanel)
	: AbstractEditCMPT(parent, name, editPanel)
	, m_shapesImpl(shapesImpl)
	, m_simplifySpin(NULL)
	, m_btnMerge(NULL)
{
	m_editOP = new EditPolylinesOP(editPanel, shapesImpl, propertyPanel, this);
}

void EditPolylinesCMPT::updateControlValue()
{
	bool valid;
	m_shapesImpl->getShapeSelection()->traverse(CountVerifyVisitor(valid, 2));
	m_btnMerge->Enable(valid);
}

float EditPolylinesCMPT::getSimplifyThreshold() const 
{
	return m_simplifySpin->GetValue() * 0.1f;
}

wxSizer* EditPolylinesCMPT::initLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	sizer->Add(initSimplifyPanel());
	sizer->AddSpacer(20);
	sizer->Add(initEditPanel());

	return sizer;
}

wxSizer* EditPolylinesCMPT::initSimplifyPanel()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("化简"));
	wxBoxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);

	m_simplifySpin = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, 
		wxSize(70, -1), wxSP_ARROW_KEYS, 0, 1000, 30);
	Connect(m_simplifySpin->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(EditPolylinesCMPT::onSimplifyThresholdChanged));
	sizer->Add(m_simplifySpin);

	wxButton* btnOK = new wxButton(this, wxID_ANY, wxT("确定"), wxDefaultPosition, wxSize(70, -1));
	Connect(btnOK->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(EditPolylinesCMPT::onUpdateFromSimplified));
	sizer->Add(btnOK);

	return sizer;
}

wxSizer* EditPolylinesCMPT::initSmoothPanel()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("平滑"));
	wxBoxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);

	return sizer;
}

wxSizer* EditPolylinesCMPT::initEditPanel()
{
	wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT(""));
	wxBoxSizer* sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
	{
		m_btnMerge = new wxButton(this, wxID_ANY, wxT("合并"));
		m_btnMerge->Enable(false);
		Connect(m_btnMerge->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(EditPolylinesCMPT::onMergeTwoChain));
		sizer->Add(m_btnMerge);
	}
	sizer->AddSpacer(10);
	{
		wxButton* btn = new wxButton(this, wxID_ANY, wxT("水平偏移到(x=0)"));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(EditPolylinesCMPT::onTranslate));
		sizer->Add(btn);
	}
	return sizer;
}

void EditPolylinesCMPT::onSimplifyThresholdChanged(wxSpinEvent& event)
{
	static_cast<EditPolylinesOP*>(m_editOP)->simplify();
}

void EditPolylinesCMPT::onUpdateFromSimplified(wxCommandEvent& event)
{
	static_cast<EditPolylinesOP*>(m_editOP)->updateFromSimplified();
}

void EditPolylinesCMPT::onMergeTwoChain(wxCommandEvent& event)
{
	std::vector<IShape*> shapes;
	m_shapesImpl->getShapeSelection()->traverse(FetchAllVisitor<IShape>(shapes));
	if (shapes.size() == 2)
	{
		ChainShape *chain0 = static_cast<ChainShape*>(shapes[0]),
			*chain1 = static_cast<ChainShape*>(shapes[1]);

		std::vector<Vector> merged;
		Math::mergeTwoChains(*chain0, *chain1, merged);

		chain0->setVertices(merged);
		chain0->refresh();
		m_shapesImpl->removeShape(chain1);

		m_shapesImpl->getShapeSelection()->clear();

		m_btnMerge->Enable(false);

		m_editPanel->Refresh();
	}
}

void EditPolylinesCMPT::onTranslate(wxCommandEvent& event)
{
	std::vector<IShape*> shapes;
	m_shapesImpl->getShapeSelection()->traverse(FetchAllVisitor<IShape>(shapes));

	float leftmost = FLT_MAX;
	std::vector<ChainShape*> chains;
	for (size_t i = 0, n = shapes.size(); i < n; ++i)
	{
		ChainShape* chain = dynamic_cast<ChainShape*>(shapes[i]);
		if (chain) 
		{
			chains.push_back(chain);
			if (chain->getRect().xMin < leftmost)
				leftmost = chain->getRect().xMin;
		}
	}

	const Vector offset(-leftmost, 0.0f);
	for (size_t i = 0, n = chains.size(); i < n; ++i)
	{
		std::vector<Vector> vertices = chains[i]->getVertices();
		for (size_t j = 0, m = vertices.size(); j < m; ++j)
			vertices[j] += offset;
		chains[i]->setVertices(vertices);
	}

	m_editPanel->Refresh();
}
