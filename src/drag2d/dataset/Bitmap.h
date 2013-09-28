#pragma once

#include "common/ResourcesMgr.h"

#include <wx/wx.h>

namespace d2d
{
	class Bitmap;
	typedef ResourcesMgr<Bitmap> BitmapMgr;

	class Bitmap
	{
	public:
		Bitmap();
		Bitmap(wxBitmap* bitmap);
		virtual ~Bitmap();

		bool loadFromFile(const wxString& filepath);

		const wxBitmap* getBitmap() const {
			return m_bitmap;
		}

	private:
		static float computeScale(float width);

	private:
		wxBitmap* m_bitmap;

		float m_scale;

	}; // Bitmap
}

