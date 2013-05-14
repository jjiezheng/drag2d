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

#include "Bitmap.h"

using namespace d2d;

Bitmap::Bitmap()
	: m_bitmap(NULL)
	, m_scale(1.0f)
{
}

Bitmap::Bitmap(wxBitmap* bitmap)
	: m_bitmap(bitmap)
	, m_scale(1.0f)
{
}

Bitmap::~Bitmap()
{
	delete m_bitmap;
}

bool Bitmap::loadFromFile(const wxString& filepath)
{
	static bool init = false;
	if (!init)
	{
		wxInitAllImageHandlers();
//		wxImage::AddHandler(new wxPNGHandler);
//		wxImage::AddHandler(new wxJPEGHandler);
	//	wxImage::AddHandler(new wxBMPHandler);
		init = true;
	}

 	wxImage image;
 	image.LoadFile(filepath);
 
	m_scale = computeScale(image.GetWidth());
 	wxImage scaled = image.Scale(image.GetWidth() * m_scale, image.GetHeight() * m_scale);
 	m_bitmap = new wxBitmap(scaled);

	return true;
}

float Bitmap::computeScale(float width)
{
	return width > 300 ? 150.0f / width : 0.5f;
}
