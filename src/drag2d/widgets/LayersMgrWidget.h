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

#ifndef D2D_LAYERS_MGR_WIDGET_H
#define D2D_LAYERS_MGR_WIDGET_H

#include <wx/wx.h>
#include <vector>

namespace d2d
{
	class Layer;
	class IVisitor;

	class LayersMgrWidget : public wxPanel
	{
		class LayerWidget;

	public:
		LayersMgrWidget(wxWindow* parent, wxWindow* stage);
		virtual ~LayersMgrWidget();

		void traverseEditableLayers(IVisitor& visitor);
		void traverseVisibleLayers(IVisitor& visitor);
		void traverseSelectableLayers(IVisitor& visitor);
		void traverseAllLayers(IVisitor& visitor);

		void setEditable(LayerWidget* layer);

		Layer* getEditedLayer() { return m_editedLayer; }

		void getAllLayers(std::vector<Layer*>& layers) const;

		void addLayer(Layer* layer);
		void addLayerBottom(Layer* layer);

		void clear();

	private:
		class LayerWidget : public wxPanel
		{
		public:
			LayerWidget(wxWindow* parent, Layer* layer);
			
			bool isVisible() const;
			bool isSelectable() const;

			void setEditable(bool editable);

			Layer* getLayer() { return m_layer; }

		protected:
			void onSetVisible(wxCommandEvent& event);
			void onSetEditable(wxCommandEvent& event);
			void onSetSelectable(wxCommandEvent& event);
			void onSetName(wxCommandEvent& event);

		private:
			void initLayout();

		private:
			wxRadioButton* m_editable;

			wxCheckBox* m_visible;

			wxCheckBox* m_selectable;

			wxTextCtrl* m_name;

			Layer* m_layer;
		};

	private:
		void initLayout();

		wxSizer* initLayersPanel();
		wxSizer* initButtonsPanel();

		void onAddLayer(wxCommandEvent& event);
		void onDelLayer(wxCommandEvent& event);
		void onLayerUp(wxCommandEvent& event);
		void onLayerDown(wxCommandEvent& event);

		int getEditLayerIndex() const;

	private:
		wxWindow* m_stage;

		std::vector<LayerWidget*> m_layers;

		Layer* m_editedLayer;

		wxSizer* m_layersSizer;

		wxButton *m_btnAdd, *m_btnDel;
		wxButton *m_btnUp, *m_btnDown;

	}; // LayersMgrWidget
}

#endif // D2D_LAYERS_MGR_WIDGET_H
