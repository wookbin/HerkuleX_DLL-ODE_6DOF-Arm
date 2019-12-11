//////////////////////////////////////////////////////
// Cam files for Robot applications
// 
// Copyright (c) 2002-2009. All Rights Reserved.
// Division of Applied Robot Technology KITECH
// Web: http://www.orobot.net
// Written by Seungsuk Ha<hass1004@gmail.com>
//

#include "stdafx.h"
#include "../../ode-0.11.1/include/ode/ode.h"
#include <gl/glu.h>
#include <math.h>
#include "Geometry.h"
#include "CamOgl.h"

CCamOgl::CCamOgl(const char *name, int width, int height, int fov) 
{		
	_width = width;
	_height = height;
	_fovAngle = fov;

	_image = new BYTE[_width*_height*3];

	InitBitmapInfo (_width, _height, 3);
}

CCamOgl::~CCamOgl()
{
	delete [] _image;
}

static void GetTransformationMatrix(dGeomID geomID, double *m)
{
	// ������Ʈ���� ��ġ(3x1 ���)�� �ڼ�(3x3 ���)�� ���´�.
	const dReal *pos = dGeomGetPosition(geomID); 
	const dReal *rot = dGeomGetRotation(geomID); 

	// OpenGL�� ��ǥ��ȯ ����� �÷� ������ ����ȴ�.
	m[0] = rot[0];	m[4] = rot[1];	m[8]  = rot[2];		m[12] = pos[0]; 
	m[1] = rot[4];	m[5] = rot[5];	m[9]  = rot[6];		m[13] = pos[1]; 
	m[2] = rot[8];	m[6] = rot[9];	m[10] = rot[10];	m[14] = pos[2]; 
	m[3] = 0.;		m[7] = 0.;		m[11] = 0.;			m[15] = 1.; 
}

void CCamOgl::DrawObjects(map<string, sGeometry *> *geoms)
{
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
		
		glPopMatrix();
	}
}

void CCamOgl::Draw (const double pos[3], const double rot[12], map<string, sGeometry *> *geoms)
{
	// ����Ʈ�� �����Ѵ�.
	glViewport(0, 0, _width, _height);

	// �׸��� ���� Render ���� �����Ѵ�.
	glRenderMode(GL_RENDER);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// ȭ���� �������� aspect ratio, �þ� ������ �����Ѵ�.
	gluPerspective(_fovAngle, (double)_width/_height, 0.1, 100.);

	// Modelview Matrix�� �����ϰ� �ʱ�ȭ�Ѵ�. 
	glMatrixMode(GL_MODELVIEW);    
	glLoadIdentity();

	// Screen �� Depth Buffer�� �����.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	gluLookAt( pos[0], pos[1], pos[2], pos[0]+rot[0], pos[1]+rot[4], pos[2]+rot[8],  rot[2], rot[6], rot[10] );
	
	DrawObjects(geoms);

	glFinish();

	glReadPixels (0, 0, _width, _height, GL_BGR_EXT, GL_UNSIGNED_BYTE, _image);
}

void CCamOgl::InitBitmapInfo (int width, int height, int pixelByte)
{
	memset( &_bitmapInfo, 0, sizeof( BITMAPINFO ) );
	
	_bitmapInfo.bmiHeader.biSize          = sizeof( BITMAPINFOHEADER );
	_bitmapInfo.bmiHeader.biWidth         = width;
	_bitmapInfo.bmiHeader.biHeight        = height; // top-down bitmap, negative height
	_bitmapInfo.bmiHeader.biPlanes        = 1;
	_bitmapInfo.bmiHeader.biBitCount      = pixelByte*8;
	_bitmapInfo.bmiHeader.biCompression   = BI_RGB;
	_bitmapInfo.bmiHeader.biSizeImage     = width*height*pixelByte;
	_bitmapInfo.bmiHeader.biXPelsPerMeter = 100;
	_bitmapInfo.bmiHeader.biYPelsPerMeter = 100;
	_bitmapInfo.bmiHeader.biClrUsed       = 0;
	_bitmapInfo.bmiHeader.biClrImportant  = 0;
}
