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

#include "AnimFileAdapter.h"

#include "common/FileNameTools.h"

#include <fstream>

using namespace d2d;

void AnimFileAdapter::load(const char* filename)
{
	Json::Value value;
	Json::Reader reader;
	std::ifstream fin(filename);
	reader.parse(fin, value);
	fin.close();

	name = value["name"].asString();

	fps = value["fps"].asInt();

	int i = 0;
	Json::Value layerValue = value["layer"][i++];
	while (!layerValue.isNull()) {
		Layer* layer = loadLayer(layerValue, FilenameTools::getFileDir(filename));
		layers.push_back(layer);
		layerValue = value["layer"][i++];
	}
}

AnimFileAdapter::Layer* AnimFileAdapter::loadLayer(const Json::Value& layerValue, 
												   const wxString& dlg)
{
	Layer* layer = new Layer;

	layer->name = layerValue["name"].asString();

	int i = 0;
	Json::Value frameValue = layerValue["frame"][i++];
	while (!frameValue.isNull()) {
		Frame* frame = loadFrame(frameValue, dlg);
		layer->frames.push_back(frame);
		frameValue = layerValue["frame"][i++];
	}

	return layer;
}

AnimFileAdapter::Frame* AnimFileAdapter::loadFrame(const Json::Value& frameValue, 
												   const wxString& dlg)
{
	int time = frameValue["time"].asInt();

	Frame* frame = new Frame;
	frame->id = frameValue["id"].asInt();
	frame->index = time;
	frame->bClassicTween = frameValue["tween"].asBool();

	int i = 0;
	Json::Value entryValue = frameValue["actor"][i++];
	while (!entryValue.isNull()) {
		Entry* entry = loadEntry(entryValue, dlg);
		frame->entries.push_back(entry);
		entryValue = frameValue["actor"][i++];
	}

	return frame;
}

AnimFileAdapter::Entry* AnimFileAdapter::loadEntry(const Json::Value& entryValue, 
												   const wxString& dlg)
{
	Entry* entry = new Entry;

	entry->filepath = entryValue["filepath"].asString();

	entry->name = entryValue["name"].asString();

	entry->pos.x = entryValue["position"]["x"].asDouble();
	entry->pos.y = entryValue["position"]["y"].asDouble();

	entry->angle = entryValue["angle"].asDouble();

	entry->xScale = entryValue["x scale"].asDouble();
	entry->yScale = entryValue["y scale"].asDouble();

	entry->xMirror = entryValue["x mirror"].asBool();
	entry->yMirror = entryValue["y mirror"].asBool();

	return entry;
}
