#pragma once
///////////////////////////////////////////////////////////////////////////////
// ViewGL.h
// ========
// View component of OpenGL window
//
// CREATED: 2006-07-10
// UPDATED: 2006-08-15
///////////////////////////////////////////////////////////////////////////////
#include <Windows.h>

namespace native
{
	class CViewGL
	{
	public:
		CViewGL();
		~CViewGL();

		bool createContext(HWND handle, int colorBits, int depthBits, int stencilBits);  // create OpenGL rendering context
		void closeContext(HWND handle);
		void swapBuffers();

		HDC getDC() const { return m_rHdc; };
		HGLRC getRC() const { return m_rGlrc; };
	private:
		// member functions
		static bool setPixelFormat(HDC hdc, int colorBits, int depthBits, int stencilBits);
		static int findPixelFormat(HDC hdc, int colorbits, int depthBits, int stencilBits); // return best matched format ID

		HDC m_rHdc;                                        // handle to device context
		HGLRC m_rGlrc;
	};
}


