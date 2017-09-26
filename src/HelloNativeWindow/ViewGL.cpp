#include "ViewGL.h"

///////////////////////////////////////////////////////////////////////////////
// ViewGL.cpp
// ==========
// View component of OpenGL window
//
// CREATED: 2006-07-10
// UPDATED: 2006-07-10
// 
///////////////////////////////////////////////////////////////////////////////

namespace native
{
	CViewGL::CViewGL() :m_rHdc(0), m_rGlrc(0)
	{
	}

	CViewGL::~CViewGL()
	{
	}

	///////////////////////////////////////////////////////////////////////////////
	// 创建OpenGL渲染上下文
	///////////////////////////////////////////////////////////////////////////////
	bool CViewGL::createContext(HWND handle, int colorBits, int depthBits, int stencilBits)
	{
		// 获取显示设备的上下文DC
		m_rHdc = ::GetDC(handle);

		// 设置成和DC同样的像素格式
		if (setPixelFormat(m_rHdc, colorBits, depthBits, stencilBits))
		{
			::MessageBox(0, L"Cannot set a suitable pixel format.", L"Error", MB_ICONEXCLAMATION | MB_OK);
			::ReleaseDC(handle, m_rHdc);                     // remove device context
			return false;
		}

		// 创建新的OpenGL渲染上下文
		m_rGlrc = ::wglCreateContext(m_rHdc);

		// 释放系统上下文环境
		::ReleaseDC(handle, m_rHdc);
		return true;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 释放OpenGL渲染上下文
	///////////////////////////////////////////////////////////////////////////////
	void CViewGL::closeContext(HWND handle)
	{
		if (!m_rHdc || !m_rGlrc)
			return;

		// delete DC and RC
		::wglMakeCurrent(0, 0);
		::wglDeleteContext(m_rGlrc);
		::ReleaseDC(handle, m_rHdc);

		m_rHdc = 0;
		m_rGlrc = 0;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 找出最好的像素格式
	///////////////////////////////////////////////////////////////////////////////
	int CViewGL::findPixelFormat(HDC hdc, int colorBits, int depthBits, int stencilBits)
	{
		int currMode;                               // pixel format mode ID
		int bestMode = 0;                           // return value, best pixel format
		int currScore = 0;                          // points of current mode
		int bestScore = 0;                          // points of best candidate

		PIXELFORMATDESCRIPTOR pfd;

		for (currMode = 0; ::DescribePixelFormat(hdc, currMode, sizeof(pfd), &pfd) > 0; ++currMode)
		{
			// 不支持OpenGL忽略
			if (!(pfd.dwFlags & PFD_SUPPORT_OPENGL)) continue;
			
			// 不支持渲染到窗口忽略
			if (!(pfd.dwFlags & PFD_DRAW_TO_WINDOW)) continue;

			// 不支持rgba忽略
			if ((pfd.iPixelType != PFD_TYPE_RGBA) || (pfd.dwFlags & PFD_NEED_PALETTE)) continue;

			// 不支持双缓存
			if (!(pfd.dwFlags & PFD_DOUBLEBUFFER)) continue;

			// 如果上述选项都支持，则尝试找到最好的候选像素
			currScore = 0;

			// colour bits
			if (pfd.cColorBits >= colorBits) ++currScore;
			if (pfd.cColorBits == colorBits) ++currScore;

			// depth bits
			if (pfd.cDepthBits >= depthBits) ++currScore;
			if (pfd.cDepthBits == depthBits) ++currScore;

			// stencil bits
			if (pfd.cStencilBits >= stencilBits) ++currScore;
			if (pfd.cStencilBits == stencilBits) ++currScore;

			// alpha bits
			if (pfd.cAlphaBits > 0) ++currScore;

			// check if it is best mode so far
			if (currScore > bestScore)
			{
				bestScore = currScore;
				bestMode = currMode;
			}
		}
		return bestMode;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 选择像素格式
	// pdf.dwFlags 设置为 PFD_DRAW_TO_WINDOW, PFD_DOUBLEBUFFER and PFD_SUPPORT_OPENGL.
	///////////////////////////////////////////////////////////////////////////////
	bool CViewGL::setPixelFormat(HDC hdc, int colorBits, int depthBits, int stencilBits)
	{
		PIXELFORMATDESCRIPTOR pfd;

		// 找出最好的匹配像素
		int pixelFormat = findPixelFormat(hdc, colorBits, depthBits, stencilBits);
		if (pixelFormat == 0) return false;

		// 设置像素描述数据结构
		::DescribePixelFormat(hdc, pixelFormat, sizeof(pfd), &pfd);

		// 设置像素格式
		if (!::SetPixelFormat(hdc, pixelFormat, &pfd)) return false;
		return true;
	}

	///////////////////////////////////////////////////////////////////////////////
	// 交换OpenGL帧缓存
	///////////////////////////////////////////////////////////////////////////////
	void CViewGL::swapBuffers()
	{
		::SwapBuffers(m_rHdc);
	}
}
