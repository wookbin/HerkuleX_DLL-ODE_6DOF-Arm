//////////////////////////////////////////////////////
// Windows Control files for Robot applications
// 
// Copyright (c) 2002-2009. All Rights Reserved.
// Division of Applied Robot Technology KITECH
// Web: http://www.orobot.net
// Written by KwangWoong Yang<page365@gmail.com>
//

#pragma once
#pragma comment(lib, "Opengl32.lib")
#pragma comment(lib, "Glu32.lib")

//  OpenGL 윈도우 조작 명령:
//	마우스 왼쪽 버튼 클릭 후 이동 – 로봇을 좌우, 상하로 회전한다.
//	마우스 오른쪽 버튼 클릭 후 이동 – 로봇을 좌우, 상하로 이동한다.
//	마우스 스크롤 이동 – 로봇의 원근을 조절한다.


class COglWnd : public CWnd
{
public:
	COglWnd (double eyeDist, double rotateHor, double rotateVer);
	virtual ~COglWnd ();

	void UpdateWnd ();

	virtual void OnDraw () { }
	virtual void OnHits (int name) { }

protected:
	// Viewport 크기와 Perspective projection 설정
	int _width;		// Viewport의 넓이
	int _height;	// Viewport의 높이
	double _fovAngle;	// 화각(단위: deg)

private:
	void InitGL();
	void DeinitGL();
	bool SetupPixelFormat(HDC hdc);

	void ListHits(int hits, unsigned int *names);
	void SelectionTest(int x, int y);

private:
	// 로봇을 바라보는 눈의 위치와 방향을 설정한다.
	double _eyePos[3];	
	double _centerPos[3];
	double _rotateHor;
	double _rotateVer;

	// 마우스 눌린 버튼의 상태와 위치 저장
	int _mouseButton;
	CPoint _mouseDownPoint;

	HGLRC _glrc;	// Rendering Context

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};
