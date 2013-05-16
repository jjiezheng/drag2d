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

#ifndef D2D_FILENAME_TOOLS_H
#define D2D_FILENAME_TOOLS_H

#include <wx/wx.h>

namespace d2d
{
	class FilenameTools
	{
	public:
		// add _* between end and '.', will not add if already has one
		static wxString getFilenameAddTag(const wxString& filename, const wxString& tag, 
			const wxString& extension);

		// get string between the last '_' and '.'
		static wxString getFilenameTag(const wxString& filepath);

		// get string between the last '\' and '.'
		static wxString getFilename(const wxString& filepath);

		// get string after the last '\'
		static wxString getFilenameWithExtension(const wxString& filepath);

		// get string from begin to '.'
		static wxString getFilePathExceptExtension(const wxString& filepath);

		// get string after '.'
		static wxString getExtension(const wxString& filepath);

		// get string from begin to the last '\'
		static wxString getFileDir(const wxString& filepath);

		//////////////////////////////////////////////////////////////////////////

		static bool isExist(const wxString& filepath);

		static wxString getExistFilepath(const wxString& filepath, const wxString& dir = wxEmptyString);

	}; // FilenameTools
}

#endif // D2D_FILENAME_TOOLS_H
