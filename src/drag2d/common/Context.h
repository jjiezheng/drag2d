#ifndef D2D_CONTEXT_H
#define D2D_CONTEXT_H

#include <wx/string.h>

namespace d2d
{
	class Context
	{
	public:
		// The executable's directory
		wxString exePath;

		// Resource's dialog, usually is open file's directory
		wxString resPath;

	public:
		wxString getDir();

	public:
		static Context* Instance();

	private:
		Context();

	private:
		static Context* m_instance;

	}; // Context
}

#endif // D2D_CONTEXT_H