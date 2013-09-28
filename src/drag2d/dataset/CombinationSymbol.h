#pragma once

#include <vector>

#include "ISymbol.h"

#include "common/Rect.h"

namespace d2d
{
	class ISprite;

	class CombinationSymbol : public ISymbol
	{
	public:
		CombinationSymbol();
		CombinationSymbol(const std::vector<ISprite*>& children);
		virtual ~CombinationSymbol();

		//
		// IObject interface
		//	
		virtual CombinationSymbol* clone();

		//
		// ISerializable interface
		//
		virtual void loadFromTextFile(std::ifstream& fin);
		virtual void storeToTextFile(std::ofstream& fout) const;

		//
		// ISymbol interface
		//
		virtual void reloadTexture() const;
		virtual void draw(const ISprite* sprite = NULL) const;
		virtual float getWidth(const ISprite* sprite = NULL) const;
		virtual float getHeight(const ISprite* sprite = NULL) const;

 		const std::vector<ISprite*>& getChildren() const {
 			return m_sprites;
 		}
		const Rect& getRect() const { return m_rect; }

	protected:
		virtual void loadResources();

	private:
		void moveToCenter();

		void initBounding();

	private:
		std::vector<ISprite*> m_sprites;

		Rect m_rect;

	}; // CombinationSymbol
}

