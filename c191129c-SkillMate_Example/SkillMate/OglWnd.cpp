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
	, _fovAngle (45) //기본: 45도 
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
	// OnEraseBkgnd() 는 WM_ERASEBKGND 의 메시지의 처리 함수로서 굳이 오버로딩할 필요는 없지만, 
	// TRUE를 반환함으로써 OpenGL 스크린을 렌더링하기 전에 스크린에 배경색을 칠하는 것을 막아줄 수 있습니다. 
	// 결과적으로 화면의 번쩍거림 현상을 없애줄 겁니다. 

	return TRUE;
}

int COglWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1) {
		TRACE ("OnCreate failed\n"); 
		return -1;
	}

	// DC에 대한 픽셀 포맷을 설정한 후 이와 연관된 RC를 생성합니다. 
	// RC는 모든 OpenGL 명령어들이 통과해야 하는 일종의 포트로 생각할 수 있습니다. 
	// RC는 DC에 지정된 것과 같은 픽셀 포맷을 지니는데 RC와 DC는 서로 같은 것이 아니며, 
	// DC는 GDI를 위한 정보를 포함하는 반면 RC는 OpenGL을 위한 정보를 담고 있습니다. 

	// 먼저 디바이스 컨텍스트를 얻는다. 
	CDC *dc = GetDC ();

	if (!SetupPixelFormat(dc->m_hDC)) {
		TRACE ("SetupPixelFormat failed\n"); 
		return -1;
	}
	// 렌더링 컨텍스트를 얻는다 
	_glrc = wglCreateContext(dc->m_hDC);
	if (!_glrc) {
		TRACE ("wglCreateContext failed\n"); 
		return -1; 
	} 
	// 현재의 렌더링 컨텍스트를 만든다. 
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

	if(!wglMakeCurrent(0, 0)) { // 현재 렌더링 컨텍스트 해제 
		TRACE ("wglMakeCurrent failed\n"); 
	} 
	if(!wglDeleteContext(_glrc)) { // 렌더링 컨텍스트 제거 
		TRACE ("wglDeleteContext failed\n"); 
	} 

	CWnd::OnDestroy();
}


bool COglWnd::SetupPixelFormat(HDC hdc)
{
	// 픽셀 코드를 설정합니다. 
	// 픽셀 포맷은 직접 그릴 객체의 대상, 즉 윈도우나 비트맵에 대한 컬러 비트의 구조에 대한 속성을 지정하는 것을 의미합니다. 
	// PIXELFORMATDESCRIPTOR 구조체는 DC에 의해 사용되는 픽셀 포맷을 지정하며, 각 윈도우는 각자의 픽셀 포맷을 가지고 있습니다. 
	// 여기서 PIXELFORMATDESCRIPTOR 구조체에는 크기, 버전 넘버, 속성 플래그, 컬러 비트 수, 어큐뮬레이터(accumulate)나 
	// 깊이(depth) 버퍼, 스텐슬(stencil), 보조(auxiliary) 버퍼 등의 지정, 메인 레이어(layer)타입등의 정보를 지정할 수 있죠. 
	// 더블 버퍼링은 전면 버퍼(Screen Buffer)와 이면 버퍼(Off-Screen Buffer, 후면 버퍼라고도 한다.)를 이용하는데, 
	// 이면 버퍼에 먼저 렌더링한 후 전면 버퍼와 교체하는 방식을 통해 보다 빠른 애니메이션을 구현할 수 있습니다. 

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

	// 바탕화면의 색을 설정한다. 여기서는 윈도우 대화상자 배경색을 가져와서 설정한다.
	COLORREF color = ::GetSysColor(COLOR_3DSHADOW); //COLOR_3DFACE
	glClearColor((float)GetRValue(color)/250.0f,(float)GetGValue(color)/250.0f,(float)GetBValue(color)/250.0f, 0.8);
	
	// Depth Buffer를 설정하고 초기화 한다.
	glClearDepth(1.);			// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);	// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);		// The Type Of Depth Test To Do

	// 가장 훌륭한 퍼스펙티브한 화면을 요청한다.
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	

	// 조명 설정
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// 재질의 속성 설정
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

	// 뷰포트(그려질 화면의 크기)를 설정한다.
	glViewport(0, 0, _width, _height);

	// 그리기 위한 Render 모드로 변경한다.
	glRenderMode(GL_RENDER);

	// Projection Matrix를 선택하고 초기화 한다.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// 화각과 윈도우의 aspect ratio, 시야 범위를 설정한다.
	gluPerspective(_fovAngle, (double)_width/_height, 0.1, 100.);

	// Modelview Matrix를 선택하고 초기화한다. 
	glMatrixMode(GL_MODELVIEW);    
	glLoadIdentity();

	// Screen 과 Depth Buffer를 지운다.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 보는 시각 설정
	gluLookAt( _eyePos[0], _eyePos[1], _eyePos[2], _centerPos[0], _centerPos[1], _centerPos[2], 0,0,1 );

	// 좌표계 회전
	glRotated( _rotateVer, 1,0,0 );
	glRotated( _rotateHor, 0,0,1 );

	// virtual 함수를 호출하여 그림을 그린다.
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
		int choose = names[3]; // 처음 선택된 ID를 설정
		int depth  = names[1]; // 최소 거리를 설정

		for (int i = 1; i < hits; i++) {
			// 만일 이 오브젝트가 선택된 오브젝트보다 가까우면 선택된 오브젝트를 바꾼다.
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
	// 뷰포트(그려질 화면의 크기)를 설정한다.
	glViewport(0, 0, _width, _height);

	// 객체 선택을 위한 Selecton 모드로 변경한다.
	glRenderMode(GL_SELECT);

	// 선택된 데이터를 저장하기 위한 버퍼 할당
	GLuint selectBuff[256] = {0, };
	glSelectBuffer(sizeof(selectBuff)/sizeof(GLuint), selectBuff);

	// Projection Matrix를 선택하고 초기화 한다.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// 뷰포트 정보를 설정한다.
	GLint viewport[4] = {0, 0, _width, _height};
	// 그리는 영역을 커서 근처로 제한한다.
	gluPickMatrix(x, _height - y, 5, 5, viewport);
	// 화각과 윈도우의 aspect ratio, 시야 범위를 설정한다.
	gluPerspective(_fovAngle, (double)_width/_height, 0.1, 1000.);

	// Modelview 모드를 선택하고 그림을 그린다.
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); 

	// 이름(ID) 스택을 모두 지운다.
	glInitNames();

	// 이름 스택에 사용하지 않는 ID -1을 지정한다. 
	// 만일 이름 스택에 아무런 값도 지정하지 않으면 glLoadName 함수가 에러를 발생시킨다.
	glPushName(-1);

	// 보는 시각 설정
	gluLookAt( _eyePos[0], _eyePos[1], _eyePos[2], _centerPos[0], _centerPos[1], _centerPos[2], 0,0,1 );

	// 좌표계 회전
	glRotated( _rotateVer, 1,0,0 );
	glRotated( _rotateHor, 0,0,1 );

	// virtual 함수를 호출하여 그림을 그린다.
	OnDraw ();

	glFlush();

	// get number of objects drawed in that area and return to render mode
	GLint hits = glRenderMode(GL_RENDER);

	ListHits(hits, selectBuff);
}

void COglWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	// cx, cy의 최소 크기를 1로 한다.
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

