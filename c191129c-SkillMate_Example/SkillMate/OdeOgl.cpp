
#include "StdAfx.h"
#include "Ode.h"
#include "OdeOgl.h"
#include "OglObjects.h"
#include <gl/glu.h>
#include <math.h>

COdeOgl::COdeOgl (COde *ode, const char *name, const RECT &rect, CWnd *parent, UINT ID) 
: COglWnd (0.9, -45, 45), _ode(ode), _showAxis(false)
{
	// OpenGL 윈도우를 생성하고 화면에 표시한다.
	CreateEx(0, "#32770", name, WS_TABSTOP | WS_CHILD | WS_VISIBLE, rect, parent, ID);
	ShowWindow(SW_SHOW);
	SetFocus ();
}

COdeOgl::~COdeOgl ()
{
	DestroyWindow ();
}

void COdeOgl::OnDraw() 
{ 
	// 바닥의 격자를 10x10 m, 눈금 1m로 그린다. _ wbjin
	oglPlane (1.0, 0.1);
	//oglPlane (10.0, 2.0);
	// ODE에 등록된 객체들을 가져와서 OpenGL 윈도우에 그린다.
	DrawObjects ();
}

static void GetTransformationMatrix(dGeomID geomID, double *m)
{
	// 지오메트리의 위치(3x1 행렬)와 자세(3x3 행렬)를 얻어온다.
	const dReal* pos = dGeomGetPosition(geomID); 
	const dReal* rot = dGeomGetRotation(geomID); 

	// OpenGL의 좌표변환 행렬은 컬럼 순으로 저장된다.
	m[0] = rot[0];	m[4] = rot[1];	m[8]  = rot[2];		m[12] = pos[0]; 
	m[1] = rot[4];	m[5] = rot[5];	m[9]  = rot[6];		m[13] = pos[1]; 
	m[2] = rot[8];	m[6] = rot[9];	m[10] = rot[10];	m[14] = pos[2]; 
	m[3] = 0.;		m[7] = 0.;		m[11] = 0.;			m[15] = 1.; 
}

void COdeOgl::DrawObjects()
{
	// ODE 객체의 리스트의 포인터를 가져온다.
	map<string, sGeometry *> *geoms = _ode->pGetOdeGeomList ();

	for (map<string, sGeometry *>::iterator it = geoms->begin (); it != geoms->end (); ++it) {
		const string &name = it->first;
		sGeometry *geom = it->second;

		glPushMatrix();

		double m[16];	// OpenGL 좌표변환 행렬을 저장할 4x4 배열
		GetTransformationMatrix(geom->_geomID, m);
		
		// 지오메트리(geom)가 ray 인 경우만 좀 특별하게 처리한다.
		sRay *ray = dynamic_cast<sRay *>(geom);

		// 지오메트리의 위치로 좌표를 변환한다.
		// 선분(ray)인 경우는 좌표변환을 하지 않음.
		if(!ray) glMultMatrixd(m);

		glLoadName ((GLuint)name.c_str());
		// 지오메트리를 그린다.
		geom->Draw();
		
		// _showAxis가 true인 경우 좌표계를 표시한다.
		if(_showAxis)	oglCoordinate(0.5);

		glPopMatrix();
	}
}

void COdeOgl::OnHits (int name) 
{
	// 마우스로 지오메트리를 클릭 한 경우 이벤트가 발생한다.

	TRACE ("Name of the hit(glLoadName): %s \n", (const char *)name);
}
