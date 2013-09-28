#pragma once

#include "interfaces.h"
#include "PasteSymbolOP.h"

#include "component/PasteSymbolOffsetCMPT.h"

namespace d2d
{
	class MultiSpritesImpl;
	class LibraryPanel;
	class ISprite;

	class PasteSymbolTileOP : public PasteSymbolOP
	{
	public:
		PasteSymbolTileOP(EditPanel* editPanel, MultiSpritesImpl* spritesImpl, 
			LibraryPanel* libraryPanel, PasteSymbolOffsetCMPT<PasteSymbolTileOP>* cmpt);

		virtual bool onMouseLeftDown(int x, int y);
		virtual bool onMouseRightDown(int x, int y);
		virtual bool onMouseMove(int x, int y);

		virtual bool onDraw() const;

	private:
		class NearestQueryVisitor : public IVisitor
		{
		public:
			NearestQueryVisitor(const Vector& pos, ISprite** ret);

			virtual void visit(Object* object, bool& bFetchNext);

		private:
			const Vector& m_pos;

			float m_dis;

			ISprite** m_result;

		}; // NearestQueryVisitor

	private:
		PasteSymbolOffsetCMPT<PasteSymbolTileOP>* m_cmpt;

		bool m_bCaptured;

		float m_rotate;

	}; // PasteSymbolTileOP
}

