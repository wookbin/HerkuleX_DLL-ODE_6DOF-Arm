//////////////////////////////////////////////////////
// Windows Control files for Robot applications
// 
// Copyright (c) 2002-2009. All Rights Reserved.
// Division of Applied Robot Technology KITECH
// Web: http://www.orobot.net
// Written by KwangWoong Yang<page365@gmail.com>
//

#include "StdAfx.h"
#include <gl/gl.h>
#include <gl/glu.h>
#include "OglWnd.h"

BEGIN_MESSAGE_MAP(COglWnd, CWnd)
	ON_WM_ERASEBKGND ()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
END_MESSAGE_MAP()

COglWnd::COglWnd(double eyeDist, double rotateHor, double rotateVer) 
	: _rotateHor (rotateHor)
	, _rotateVer (rotateVer)
	, _mouseButton (0)
	, _fovAngle (45) //�⺻: 45�� 
{
	_eyePos[0] = 0;
	_eyePos[1] = -eyeDist*1.1;
	_eyePos[2] = 0;

	_centerPos[0] = 0;
	_centerPos[1] = 0;
	_centerPos[2] = 0;
}

COglWnd::~COglWnd ()
{
}

BOOL COglWnd::OnEraseBkgnd(CDC* pDC)
{
	// OnEraseBkgnd() �� WM_ERASEBKGND �� �޽����� ó�� �Լ��μ� ���� �����ε��� �ʿ�� ������, 
	// TRUE�� ��ȯ�����ν� OpenGL ��ũ���� �������ϱ� ���� ��ũ���� ������ ĥ�ϴ� ���� ������ �� �ֽ��ϴ�. 
	// ��������� ȭ���� ��½�Ÿ� ������ ������ �̴ϴ�. 

	return TRUE;
}

int COglWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1) {
		TRACE ("OnCreate failed\n"); 
		return -1;
	}

	// DC�� ���� �ȼ� ������ ������ �� �̿� ������ RC�� �����մϴ�. 
	// RC�� ��� OpenGL ��ɾ���� ����ؾ� �ϴ� ������ ��Ʈ�� ������ �� �ֽ��ϴ�. 
	// RC�� DC�� ������ �Ͱ� ���� �ȼ� ������ ���ϴµ� RC�� DC�� ���� ���� ���� �ƴϸ�, 
	// DC�� GDI�� ���� ������ �����ϴ� �ݸ� RC�� OpenGL�� ���� ������ ��� �ֽ��ϴ�. 

	// ���� ����̽� ���ؽ�Ʈ�� ��´�. 
	CDC *dc = GetDC ();

	if (!SetupPixelFormat(dc->m_hDC)) {
		TRACE ("SetupPixelFormat failed\n"); 
		return -1;
	}
	// ������ ���ؽ�Ʈ�� ��´� 
	_glrc = wglCreateContext(dc->m_hDC);
	if (!_glrc) {
		TRACE ("wglCreateContext failed\n"); 
		return -1; 
	} 
	// ������ ������ ���ؽ�Ʈ�� �����. 
	if(!wglMakeCurrent(dc->m_hDC, _glrc)) { 
		TRACE ("wglMakeCurrent failed\n"); 
		return -1; 
	} 
	ReleaseDC (dc);

	InitGL ();
	return 0;
}

void COglWnd::OnDestroy()
{
	DeinitGL ();

	if(!wglMakeCurrent(0, 0)) { // ���� ������ ���ؽ�Ʈ ���� 
		TRACE ("wglMakeCurrent failed\n"); 
	} 
	if(!wglDeleteContext(_glrc)) { // ������ ���ؽ�Ʈ ���� 
		TRACE ("wglDeleteContext failed\n"); 
	} 

	CWnd::OnDestroy();
}


bool COglWnd::SetupPixelFormat(HDC hdc)
{
	// �ȼ� �ڵ带 �����մϴ�. 
	// �ȼ� ������ ���� �׸� ��ü�� ���, �� �����쳪 ��Ʈ�ʿ� ���� �÷� ��Ʈ�� ������ ���� �Ӽ��� �����ϴ� ���� �ǹ��մϴ�. 
	// PIXELFORMATDESCRIPTOR ����ü�� DC�� ���� ���Ǵ� �ȼ� ������ �����ϸ�, �� ������� ������ �ȼ� ������ ������ �ֽ��ϴ�. 
	// ���⼭ PIXELFORMATDESCRIPTOR ����ü���� ũ��, ���� �ѹ�, �Ӽ� �÷���, �÷� ��Ʈ ��, ��ť�ķ�����(accumulate)�� 
	// ����(depth) ����, ���ٽ�(stencil), ����(auxiliary) ���� ���� ����, ���� ���̾�(layer)Ÿ�Ե��� ������ ������ �� ����. 
	// ���� ���۸��� ���� ����(Screen Buffer)�� �̸� ����(Off-Screen Buffer, �ĸ� ���۶�� �Ѵ�.)�� �̿��ϴµ�, 
	// �̸� ���ۿ� ���� �������� �� ���� ���ۿ� ��ü�ϴ� ����� ���� ���� ���� �ִϸ��̼��� ������ �� �ֽ��ϴ�. 

	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),  // size of this pfd
		1,                              // version number
		PFD_DRAW_TO_WINDOW |            // support window
		PFD_SUPPORT_OPENGL |		    // support OpenGL
		PFD_DOUBLEBUFFER,				// double buffered
		PFD_TYPE_RGBA,                  // RGBA type
		24,                             // 24-bit color depth
		0, 0, 0, 0, 0, 0,               // color bits ignored
		0,                              // no alpha buffer
		0,                              // shift bit ignored
		0,                              // no accumulation buffer
		0, 0, 0, 0,                     // accum bits ignored
		16,                             // 16-bit z-buffer
		0,                              // no stencil buffer
		0,                              // no auxiliary buffer
		PFD_MAIN_PLANE,                 // main layer
		0,                              // reserved
		0, 0, 0                         // layer masks ignored
	};

	int pixelFormat = ChoosePixelFormat(hdc, &pfd);
	if(pixelFormat == 0) { 
		::AfxMessageBox("ChoosePixelFormat failed.\n"); 
		return false;
	} 

	BOOL ret = SetPixelFormat(hdc, pixelFormat, &pfd);
	if(ret == FALSE) { 
		::AfxMessageBox("SetPixelFormat failed.\n"); 
		return false; 
	} 
	return true; 
}

void COglWnd::InitGL()  
{
	// Enables Smooth Shading
	glShadeModel(GL_SMOOTH);	

	// ����ȭ���� ���� �����Ѵ�. ���⼭�� ������ ��ȭ���� ������ �����ͼ� �����Ѵ�.
	COLORREF color = ::GetSysColor(COLOR_3DSHADOW); //COLOR_3DFACE
	glClearColor((float)GetRValue(color)/250.0f,(float)GetGValue(color)/250.0f,(float)GetBValue(color)/250.0f, 0.8);
	
	// Depth Buffer�� �����ϰ� �ʱ�ȭ �Ѵ�.
	glClearDepth(1.);			// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);	// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);		// The Type Of Depth Test To Do

	// ���� �Ǹ��� �۽���Ƽ���� ȭ���� ��û�Ѵ�.
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	

	// ���� ����
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// ������ �Ӽ� ����
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
} 

void COglWnd::DeinitGL()
{
}

void COglWnd::UpdateWnd () 
{
	RECT rect;
	GetClientRect(&rect);
	InvalidateRect(&rect,FALSE);
}

void COglWnd::OnPaint() 
{
	CPaintDC dc(this);

	// ����Ʈ(�׷��� ȭ���� ũ��)�� �����Ѵ�.
	glViewport(0, 0, _width, _height);

	// �׸��� ���� Render ���� �����Ѵ�.
	glRenderMode(GL_RENDER);

	// Projection Matrix�� �����ϰ� �ʱ�ȭ �Ѵ�.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// ȭ���� �������� aspect ratio, �þ� ������ �����Ѵ�.
	gluPerspective(_fovAngle, (double)_width/_height, 0.1, 100.);

	// Modelview Matrix�� �����ϰ� �ʱ�ȭ�Ѵ�. 
	glMatrixMode(GL_MODELVIEW);    
	glLoadIdentity();

	// Screen �� Depth Buffer�� �����.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// ���� �ð� ����
	gluLookAt( _eyePos[0], _eyePos[1], _eyePos[2], _centerPos[0], _centerPos[1], _centerPos[2], 0,0,1 );

	// ��ǥ�� ȸ��
	glRotated( _rotateVer, 1,0,0 );
	glRotated( _rotateHor, 0,0,1 );

	// virtual �Լ��� ȣ���Ͽ� �׸��� �׸���.
	OnDraw ();

	glFinish();

	SwapBuffers(dc.m_hDC);
}

void COglWnd::ListHits(GLint hits, GLuint *names)
{
	// For each hit in the buffer are allocated 4 bytes:
	// 1. Number of hits selected
	// 2. Min Z
	// 3. Max Z
	// 4. Name of the hit (glLoadName)

	if (hits > 0) {
		int choose = names[3]; // ó�� ���õ� ID�� ����
		int depth  = names[1]; // �ּ� �Ÿ��� ����

		for (int i = 1; i < hits; i++) {
			// ���� �� ������Ʈ�� ���õ� ������Ʈ���� ������ ���õ� ������Ʈ�� �ٲ۴�.
			if (names[i*4+1] < GLuint(depth)) {
				choose = names[i*4+3]; 
				depth  = names[i*4+1]; 
			}
		}
		if (0 <= choose) {
			OnHits (choose);
		}
	}
}

void COglWnd::SelectionTest(int x, int y)
{
	// ����Ʈ(�׷��� ȭ���� ũ��)�� �����Ѵ�.
	glViewport(0, 0, _width, _height);

	// ��ü ������ ���� Selecton ���� �����Ѵ�.
	glRenderMode(GL_SELECT);

	// ���õ� �����͸� �����ϱ� ���� ���� �Ҵ�
	GLuint selectBuff[256] = {0, };
	glSelectBuffer(sizeof(selectBuff)/sizeof(GLuint), selectBuff);

	// Projection Matrix�� �����ϰ� �ʱ�ȭ �Ѵ�.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// ����Ʈ ������ �����Ѵ�.
	GLint viewport[4] = {0, 0, _width, _height};
	// �׸��� ������ Ŀ�� ��ó�� �����Ѵ�.
	gluPickMatrix(x, _height - y, 5, 5, viewport);
	// ȭ���� �������� aspect ratio, �þ� ������ �����Ѵ�.
	gluPerspective(_fovAngle, (double)_width/_height, 0.1, 1000.);

	// Modelview ��带 �����ϰ� �׸��� �׸���.
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); 

	// �̸�(ID) ������ ��� �����.
	glInitNames();

	// �̸� ���ÿ� ������� �ʴ� ID -1�� �����Ѵ�. 
	// ���� �̸� ���ÿ� �ƹ��� ���� �������� ������ glLoadName �Լ��� ������ �߻���Ų��.
	glPushName(-1);

	// ���� �ð� ����
	gluLookAt( _eyePos[0], _eyePos[1], _eyePos[2], _centerPos[0], _centerPos[1], _centerPos[2], 0,0,1 );

	// ��ǥ�� ȸ��
	glRotated( _rotateVer, 1,0,0 );
	glRotated( _rotateHor, 0,0,1 );

	// virtual �Լ��� ȣ���Ͽ� �׸��� �׸���.
	OnDraw ();

	glFlush();

	// get number of objects drawed in that area and return to render mode
	GLint hits = glRenderMode(GL_RENDER);

	ListHits(hits, selectBuff);
}

void COglWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	// cx, cy�� �ּ� ũ�⸦ 1�� �Ѵ�.
	_width = max (1, cx);
	_height = max (1, cy);

	InvalidateRect(NULL, FALSE);
}

void COglWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	_mouseButton = 1;
	_mouseDownPoint=point;
	SetCapture();
	SetFocus ();

	SelectionTest (point.x, point.y);

	CWnd::OnLButtonDown(nFlags, point);
}

void COglWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	_mouseDownPoint=CPoint(0, 0);
	ReleaseCapture();
	_mouseButton = 0;

	CWnd::OnLButtonUp(nFlags, point);
}

void COglWnd::OnRButtonDown(UINT nFlags, CPoint point)
{
	_mouseButton = 2;
	_mouseDownPoint=point;
	SetCapture();
	SetFocus ();

	CWnd::OnRButtonDown(nFlags, point);
}

void COglWnd::OnRButtonUp(UINT nFlags, CPoint point)
{
	_mouseDownPoint=CPoint(0, 0);
	ReleaseCapture();
	_mouseButton = 0;

	CWnd::OnRButtonUp(nFlags, point);
} 

void COglWnd::OnMButtonDown(UINT nFlags, CPoint point)
{
	_mouseButton = 3;
	_mouseDownPoint=point;
	SetCapture();

	CWnd::OnMButtonDown(nFlags, point);
}

void COglWnd::OnMButtonUp(UINT nFlags, CPoint point)
{
	_mouseDownPoint=CPoint(0, 0);
	ReleaseCapture();
	_mouseButton = 0;
	SetFocus ();

	CWnd::OnMButtonUp(nFlags, point);
}

void COglWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (GetCapture()==this) {
		if (_mouseButton == 1) {
			//Increment the object rotation angles
			_rotateHor+=(point.x-_mouseDownPoint.x)/3.6;
			_rotateVer+=(point.y-_mouseDownPoint.y)/3.6;
		}
		else if (_mouseButton == 2) {
			_centerPos[0] -= (point.x-_mouseDownPoint.x)/360.;
			_centerPos[2] += (point.y-_mouseDownPoint.y)/360.;
		}
		else if (_mouseButton == 3) {
		}

		//Redraw the viewport
		InvalidateRect(NULL,FALSE);
		//Set the mouse point
		_mouseDownPoint=point;
	}

	CWnd::OnMouseMove(nFlags, point);
}

BOOL COglWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	_eyePos[1] += (zDelta < 0) ? -0.1 : 0.1;

	InvalidateRect(NULL,FALSE);

	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

