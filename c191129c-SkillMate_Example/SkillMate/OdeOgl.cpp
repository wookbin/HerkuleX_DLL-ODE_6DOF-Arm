
#include "StdAfx.h"
#include "Ode.h"
#include "OdeOgl.h"
#include "OglObjects.h"
#include <gl/glu.h>
#include <math.h>

COdeOgl::COdeOgl (COde *ode, const char *name, const RECT &rect, CWnd *parent, UINT ID) 
: COglWnd (0.9, -45, 45), _ode(ode), _showAxis(false)
{
	// OpenGL �����츦 �����ϰ� ȭ�鿡 ǥ���Ѵ�.
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
	// �ٴ��� ���ڸ� 10x10 m, ���� 1m�� �׸���. _ wbjin
	oglPlane (1.0, 0.1);
	//oglPlane (10.0, 2.0);
	// ODE�� ��ϵ� ��ü���� �����ͼ� OpenGL �����쿡 �׸���.
	DrawObjects ();
}

static void GetTransformationMatrix(dGeomID geomID, double *m)
{
	// ������Ʈ���� ��ġ(3x1 ���)�� �ڼ�(3x3 ���)�� ���´�.
	const dReal* pos = dGeomGetPosition(geomID); 
	const dReal* rot = dGeomGetRotation(geomID); 

	// OpenGL�� ��ǥ��ȯ ����� �÷� ������ ����ȴ�.
	m[0] = rot[0];	m[4] = rot[1];	m[8]  = rot[2];		m[12] = pos[0]; 
	m[1] = rot[4];	m[5] = rot[5];	m[9]  = rot[6];		m[13] = pos[1]; 
	m[2] = rot[8];	m[6] = rot[9];	m[10] = rot[10];	m[14] = pos[2]; 
	m[3] = 0.;		m[7] = 0.;		m[11] = 0.;			m[15] = 1.; 
}

void COdeOgl::DrawObjects()
{
	// ODE ��ü�� ����Ʈ�� �����͸� �����´�.
	map<string, sGeometry *> *geoms = _ode->pGetOdeGeomList ();

	for (map<string, sGeometry *>::iterator it = geoms->begin (); it != geoms->end (); ++it) {
		const string &name = it->first;
		sGeometry *geom = it->second;

		glPushMatrix();

		double m[16];	// OpenGL ��ǥ��ȯ ����� ������ 4x4 �迭
		GetTransformationMatrix(geom->_geomID, m);
		
		// ������Ʈ��(geom)�� ray �� ��츸 �� Ư���ϰ� ó���Ѵ�.
		sRay *ray = dynamic_cast<sRay *>(geom);

		// ������Ʈ���� ��ġ�� ��ǥ�� ��ȯ�Ѵ�.
		// ����(ray)�� ���� ��ǥ��ȯ�� ���� ����.
		if(!ray) glMultMatrixd(m);

		glLoadName ((GLuint)name.c_str());
		// ������Ʈ���� �׸���.
		geom->Draw();
		
		// _showAxis�� true�� ��� ��ǥ�踦 ǥ���Ѵ�.
		if(_showAxis)	oglCoordinate(0.5);

		glPopMatrix();
	}
}

void COdeOgl::OnHits (int name) 
{
	// ���콺�� ������Ʈ���� Ŭ�� �� ��� �̺�Ʈ�� �߻��Ѵ�.

	TRACE ("Name of the hit(glLoadName): %s \n", (const char *)name);
}
