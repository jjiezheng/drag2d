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

#include "FilenameTools.h"
#include "Settings.h"

#include "common/tools.h"
#include "common/Exception.h"

#include <wx/filename.h>

using namespace d2d;

wxString FilenameTools::getFilenameAddTag(const wxString& filename, const wxString& tag, 
										   const wxString& extension)
{
	wxString fixed;
	int start = filename.find_last_of('_');
	if (start != -1)
	{
		wxString check = filename.substr(start + 1, filename.find_last_of('.') - start - 1);
		if (check == tag)
			fixed = filename;
		else
			fixed = filename.substr(0, filename.find_last_of('.')) + wxT("_" + tag + "." + extension);
	}
	else
	{
		fixed = filename.substr(0, filename.find_last_of('.')) + wxT("_" + tag + "." + extension);
	}
	return fixed;
}

wxString FilenameTools::getFilenameTag(const wxString& filepath)
{
	const size_t start = filepath.find_last_of('_') + 1,
		end = filepath.find_last_of('.');
	return filepath.substr(start, end - start);
}

wxString FilenameTools::getFilename(const wxString& filepath)
{
	const size_t start = filepath.find_last_of('\\') + 1,
		end = filepath.find_last_of('.');
	return filepath.substr(start, end - start);
}

wxString FilenameTools::getFilenameWithExtension(const wxString& filepath)
{
	return filepath.substr(filepath.find_last_of('\\') + 1);
}

wxString FilenameTools::getRelativePath(const wxString& dir, const wxString& absolute)
{
// 	size_t start = 0;
// 	while (dir.size() > start && filepath.size() > start && dir[start] == filepath[start])
// 		++start;
// 	wxString ret = filepath.substr(start);
// 	ret.Replace('\\', '/');
// 	return ret;

	//////////////////////////////////////////////////////////////////////////

	wxFileName filename(absolute);
	filename.MakeRelativeTo(dir);
	return filename.GetFullPath().ToStdString();
}

wxString FilenameTools::getAbsolutePath(const wxString& dir, const wxString& relative)
{
	wxFileName filename(relative);
	filename.MakeAbsolute(dir);
	wxString filepath = filename.GetFullPath();
 	if (!isExist(filepath))
 		return getExistFilepath(relative, dir);
 	else
		return filepath;
}

wxString FilenameTools::getFilePathExceptExtension(const wxString& filepath)
{
	return filepath.substr(0, filepath.find_last_of('.'));
}

wxString FilenameTools::getExtension(const wxString& filepath)
{
	return filepath.substr(filepath.find_last_of('.') + 1);
}

wxString FilenameTools::getFileDir(const wxString& filepath)
{
	return filepath.substr(0, filepath.find_last_of('\\'));
}

bool FilenameTools::isExist(const wxString& filepath)
{
	return wxFileName::FileExists(filepath);
}

wxString FilenameTools::getExistFilepath(const wxString& filepath, const wxString& dir /*= wxEmptyString*/)
{
	wxString filepathFixed = filepath;
	if (!isExist(filepathFixed))
	{
		wxString filename = filepathFixed = getFilenameWithExtension(filepathFixed);
		if (!isExist(filepathFixed))
		{
			wxString cwd = wxFileName::GetCwd();

			std::set<wxString>::iterator itr = Settings::RESOURCE_PATH.begin();
			for ( ; itr != Settings::RESOURCE_PATH.end(); ++itr)
			{
				filepathFixed = *itr + filename;
				if (isExist(filepathFixed))
					return filepathFixed;

				filepathFixed = cwd + *itr + filename;
				if (isExist(filepathFixed))
					return filepathFixed;
			}
			if (dir != wxEmptyString)
			{
				filepathFixed = dir + filename;
				if (isExist(filepathFixed))
					return filepathFixed;
			}
			throw Exception("File: %s don't exist!", filepath.ToStdString().c_str());
			return wxEmptyString;
		}
		else
		{
			return filepathFixed;
		}
	}
	else
	{
		return filepathFixed;
	}
}

void FilenameTools::formatSeparators(std::string& filepath)
{
//	StringTools::toLower(filepath);
	const std::string oldVal = "\\", newVal = "/";
	for(std::string::size_type pos(0); pos != std::string::npos; pos += oldVal.length())   
	{   
		if((pos = filepath.find(oldVal, pos)) != std::string::npos)
			filepath.replace(pos, oldVal.length(), newVal);   
		else   
			break;   
	}   
}