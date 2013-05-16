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

#ifndef D2D_UNIVERSAL_ARRANGE_CMPT_H
#define D2D_UNIVERSAL_ARRANGE_CMPT_H

#include "common/visitors.h"

#include "AbstractEditCMPT.h"

#include <Box2D/Box2D.h>

namespace d2d
{
	class MultiSpritesImpl;
	class PropertySettingPanel;

	class UniversalArrangeCMPT : public AbstractEditCMPT
	{
	public:
		UniversalArrangeCMPT(wxWindow* parent, const wxString& name,
			EditPanel* editPanel, MultiSpritesImpl* spritesImpl,
			PropertySettingPanel* propertyPanel);

		virtual void updateControlValue();

		void addPhysicsEditOP(b2World* world, b2Body* ground);

	protected:
		virtual wxSizer* initLayout();

	private:
		class GetPhysicsStaticVisitor : public IVisitor
		{
		public:
			enum TYPE
			{
				e_checked,
				e_unchecked,
				e_uncertain
			};

		public:
			GetPhysicsStaticVisitor();
			virtual void visit(ICloneable* object, bool& bFetchNext);
			TYPE getType() const { return m_type; }

		private:
			TYPE m_type;	// 0 all open, 1 all close, 2 uncertain

		}; // GetPhysicsStaticVisitor

		class SetPhysicsStaticVisitor : public IVisitor
		{
		public:
			SetPhysicsStaticVisitor(bool bChecked);
			virtual void visit(ICloneable* object, bool& bFetchNext);

		private:
			bool m_bChecked;

		}; // SetPhysicsStaticVisitor

	private:
		void onChangeStaticType(wxCommandEvent& event);

	private:
		MultiSpritesImpl* m_spritesImpl;

		wxCheckBox* m_physicsStaticCtrl;
		
	}; // UniversalArrangeCMPT
}

#endif // D2D_UNIVERSAL_ARRANGE_CMPT_H
