#pragma once

#include "ISymbol.h"

namespace d2d
{
	class Font;

	class FontSymbol : public ISymbol
	{
	public:
		FontSymbol();
		virtual ~FontSymbol();

		//
		// IObject interface
		//	
		virtual FontSymbol* clone();

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

		// from NeHe's FreeType
		void print(float x, float y, const char* text) const;

	protected:
		virtual void loadResources();

	private:
		Font* m_font;

	}; // FontSymbol
}

