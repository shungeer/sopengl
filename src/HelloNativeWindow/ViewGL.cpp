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
	// ����OpenGL��Ⱦ������
	///////////////////////////////////////////////////////////////////////////////
	bool CViewGL::createContext(HWND handle, int colorBits, int depthBits, int stencilBits)
	{
		// ��ȡ��ʾ�豸��������DC
		m_rHdc = ::GetDC(handle);

		// ���óɺ�DCͬ�������ظ�ʽ
		if (setPixelFormat(m_rHdc, colorBits, depthBits, stencilBits))
		{
			::MessageBox(0, L"Cannot set a suitable pixel format.", L"Error", MB_ICONEXCLAMATION | MB_OK);
			::ReleaseDC(handle, m_rHdc);                     // remove device context
			return false;
		}

		// �����µ�OpenGL��Ⱦ������
		m_rGlrc = ::wglCreateContext(m_rHdc);

		// �ͷ�ϵͳ�����Ļ���
		::ReleaseDC(handle, m_rHdc);
		return true;
	}

	///////////////////////////////////////////////////////////////////////////////
	// �ͷ�OpenGL��Ⱦ������
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
	// �ҳ���õ����ظ�ʽ
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
			// ��֧��OpenGL����
			if (!(pfd.dwFlags & PFD_SUPPORT_OPENGL)) continue;
			
			// ��֧����Ⱦ�����ں���
			if (!(pfd.dwFlags & PFD_DRAW_TO_WINDOW)) continue;

			// ��֧��rgba����
			if ((pfd.iPixelType != PFD_TYPE_RGBA) || (pfd.dwFlags & PFD_NEED_PALETTE)) continue;

			// ��֧��˫����
			if (!(pfd.dwFlags & PFD_DOUBLEBUFFER)) continue;

			// �������ѡ�֧�֣������ҵ���õĺ�ѡ����
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
	// ѡ�����ظ�ʽ
	// pdf.dwFlags ����Ϊ PFD_DRAW_TO_WINDOW, PFD_DOUBLEBUFFER and PFD_SUPPORT_OPENGL.
	///////////////////////////////////////////////////////////////////////////////
	bool CViewGL::setPixelFormat(HDC hdc, int colorBits, int depthBits, int stencilBits)
	{
		PIXELFORMATDESCRIPTOR pfd;

		// �ҳ���õ�ƥ������
		int pixelFormat = findPixelFormat(hdc, colorBits, depthBits, stencilBits);
		if (pixelFormat == 0) return false;

		// ���������������ݽṹ
		::DescribePixelFormat(hdc, pixelFormat, sizeof(pfd), &pfd);

		// �������ظ�ʽ
		if (!::SetPixelFormat(hdc, pixelFormat, &pfd)) return false;
		return true;
	}

	///////////////////////////////////////////////////////////////////////////////
	// ����OpenGL֡����
	///////////////////////////////////////////////////////////////////////////////
	void CViewGL::swapBuffers()
	{
		::SwapBuffers(m_rHdc);
	}
}
