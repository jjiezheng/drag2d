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

#ifndef D2D_ANIM_FILE_ADAPTER_H
#define D2D_ANIM_FILE_ADAPTER_H

#include "IFileAdapter.h"

#include "common/Vector.h"

#include <json/json.h>

class wxString;

namespace d2d
{
	class AnimFileAdapter
	{
	public:
		struct Entry
		{
			std::string filepath;

			std::string name;

			Vector pos;
			float angle;
			float xScale, yScale;
			float xShear, yShear;
			bool xMirror, yMirror;
		};

		struct Frame
		{
			int id;

			int index;

			std::vector<Entry*> entries;

			bool bClassicTween;
		};

		struct Layer
		{
			std::string name;

			std::vector<Frame*> frames;
		};

	public:
		virtual void load(const char* filename);

	private:
		Layer* loadLayer(const Json::Value& layerValue, const wxString& dlg);
		Frame* loadFrame(const Json::Value& frameValue, const wxString& dlg);
		Entry* loadEntry(const Json::Value& entryValue, const wxString& dlg);

	public:
		std::string name;

		int fps;

		std::vector<Layer*> layers;

	}; // AnimFileAdapter
}

#endif // D2D_ANIM_FILE_ADAPTER_H