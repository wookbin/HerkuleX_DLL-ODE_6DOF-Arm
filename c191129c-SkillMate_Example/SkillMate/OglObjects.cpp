//////////////////////////////////////////////////////
// Kinematics & Dynamics files for Robot applications
// 
// Copyright (c) 2002-2008. All Rights Reserved.
// Division of Applied Robot Technology KITECH
// Web: http://dart.kitech.re.kr
// Written by KwangWoong Yang<page365@gmail.com>
//

#include "StdAfx.h"
#include <math.h>
#include <gl/glu.h>
#include "OglObjects.h" 
#include "mm.h"


void oglPlane(double size, double stride)
{
	size /= 2.;
	int n = (int)(size/stride);

	glDisable(GL_LIGHTING);

	glColor3d(0.5, 0.5, 0.5);

	for (int i=-n ; i<=n; ++i) {
		oglLine (i*stride, -size, 0, i*stride, (i == 0) ? 0 : size, 0);
		oglLine (-size, i*stride, 0, (i == 0) ? 0 : size, i*stride, 0); 
	}
	// x-axis
	glColor3d(1., 0., 0.);
	oglLine (0, 0, 0, size, 0, 0);

	// y-axis
	glColor3d(0., 1., 0.);
	oglLine (0, 0, 0, 0, size, 0);

	// z-axis
	glColor3d(0., 0., 1.);
	oglLine (0, 0, 0, 0, 0, size);

	glEnable(GL_LIGHTING);
}

void oglCoordinate(double size)
{
	glDisable(GL_LIGHTING);
	//좌표계 RGB색상 지우기 _ wbjin//
	/*
	// The Z-axis
	glColor3d(0., 0., 1.);
	oglLine (0., 0., 0., 0., 0., size);
	
	// The Y-axis
	glColor3d(0., 1., 0.);
	oglLine(0., 0., 0., 0., size, 0.);

	// The X-axis
	glColor3d(1., 0., 0.);
	oglLine(0., 0., 0., size, 0., 0.);
	*/
	glEnable(GL_LIGHTING);
	
}

void oglLine (double sx, double sy, double sz, double ex, double ey, double ez)
{	
	glBegin(GL_LINES);
		glVertex3d(sx, sy, sz);
		glVertex3d(ex, ey, ez);
	glEnd();
}

void oglSphere (double radius) 
{
	int slices = 15;
	GLUquadricObj *obj = gluNewQuadric();
	
	gluSphere(obj, radius, slices, slices );

	gluDeleteQuadric(obj);
}

void oglSphere(double rx, double ry, double rz) 
{
	int lats = 15;
	int longs = 15;

	for(int i = 1; i <= lats; i++) {
        double lat0 = M_PI * (0.5 + (double)(i - 1)/lats);
        double z0  = rz*sin(lat0);
        double zr0 =  cos(lat0);

        double lat1 = M_PI * (0.5 + (double)i/lats);
        double z1 = rz*sin(lat1);
        double zr1 = cos(lat1);

        glBegin(GL_QUAD_STRIP);
        for(int j = 0; j <= longs; j++) {
            double lng = 2 * M_PI * (double)(j - 1)/longs;
            double x = rx*cos(lng);
            double y = ry*sin(lng);

            glNormal3d(x * zr0, y * zr0, z0);
            glVertex3d(x * zr0, y * zr0, z0);
            glNormal3d(x * zr1, y * zr1, z1);
            glVertex3d(x * zr1, y * zr1, z1);
        }
        glEnd();
    }
}

void oglCapsule (double height, double radius) 
{
	height /= 2.;

	int slices = 15;
	GLUquadricObj *obj = gluNewQuadric();
	
	glTranslated(0., 0., -height);
	oglSphere (radius);
	
	gluQuadricOrientation(obj, GLU_OUTSIDE);
    gluCylinder(obj, radius, radius, height*2, slices, 1);
	
	glTranslated(0., 0., height*2);
	oglSphere (radius);

	glTranslated(0., 0., -height);

	gluDeleteQuadric(obj);
}

void oglCylinder (double height, double radius) 
{
	height /= 2.;

	int slices = 15;
	GLUquadricObj *obj = gluNewQuadric();
	
	glTranslated(0., 0., -height);
	gluQuadricOrientation(obj, GLU_INSIDE);
	gluDisk(obj, 0, radius, slices, 1);
	
	gluQuadricOrientation(obj, GLU_OUTSIDE);
    gluCylinder(obj, radius, radius, height*2, slices, 1);
	
	glTranslated(0., 0., height*2);
	gluDisk(obj, 0., radius, slices, 1);

	glTranslated(0., 0., -height);

	gluDeleteQuadric(obj);
}

void oglCylinder(double height, double baseRadius, double topRadius, double xyRatio) 
{
	height /= 2.;

	int lats = 15;
	int longs = 15;

	for(int i = 1; i <= lats; i++) {
        double lat0 = -0.5 + (double)(i - 1)/lats;
        double z0  = height * lat0;
        double zr0 =  (topRadius - baseRadius) * lat0 + baseRadius;

        double lat1 = -0.5 + (double)i/lats;
        double z1 = height * lat1;
        double zr1 = (topRadius - baseRadius) * lat1 + baseRadius;

        glBegin(GL_QUAD_STRIP);
        for(int j = 0; j <= longs; j++) {
            double lng = 2 * M_PI * (double)(j - 1)/longs;
            double x = cos(lng);
            double y = xyRatio*sin(lng);

            glNormal3d(x * zr0, y * zr0, z0);
            glVertex3d(x * zr0, y * zr0, z0);
            glNormal3d(x * zr1, y * zr1, z1);
            glVertex3d(x * zr1, y * zr1, z1);
        }
        glEnd();
    }
}

void oglBox (double dx, double dy, double dz) 
{
	dx /= 2.;
	dy /= 2.;
	dz /= 2.;

	glBegin(GL_QUADS);
		glNormal3d(0., 0., 1.);
		glTexCoord2f(  0.0f,   0.0f); glVertex3d( dx,  dy,  dz);	
		glTexCoord2f(100.0f,   0.0f); glVertex3d( dx, -dy,  dz);
		glTexCoord2f(100.0f, 100.0f); glVertex3d(-dx, -dy,  dz);
		glTexCoord2f(  0.0f, 100.0f); glVertex3d(-dx,  dy,  dz);

		glNormal3d(0., 0., -1.);
		glTexCoord2f(  0.0f,   0.0f); glVertex3d(-dx,  dy, -dz);
		glTexCoord2f(100.0f,   0.0f); glVertex3d(-dx, -dy, -dz);
		glTexCoord2f(100.0f, 100.0f); glVertex3d( dx, -dy, -dz);
		glTexCoord2f(  0.0f, 100.0f); glVertex3d( dx,  dy, -dz);

		glNormal3d(1., 0., 0.);
		glTexCoord2f(  0.0f,   0.0f); glVertex3d( dx,  dy,  dz);
		glTexCoord2f(100.0f,   0.0f); glVertex3d( dx,  dy, -dz);
		glTexCoord2f(100.0f, 100.0f); glVertex3d( dx, -dy, -dz);
		glTexCoord2f(  0.0f, 100.0f); glVertex3d( dx, -dy,  dz);

		glNormal3d(-1., 0., 0.);
		glTexCoord2f(  0.0f,   0.0f); glVertex3d(-dx,  dy,  dz);
		glTexCoord2f(100.0f,   0.0f); glVertex3d(-dx,  dy, -dz);
		glTexCoord2f(100.0f, 100.0f); glVertex3d(-dx, -dy, -dz);
		glTexCoord2f(  0.0f, 100.0f); glVertex3d(-dx, -dy,  dz);

		glNormal3d(0., 1., 0.);
		glTexCoord2f(  0.0f,   0.0f); glVertex3d(-dx,  dy,  dz);
		glTexCoord2f(100.0f,   0.0f); glVertex3d( dx,  dy,  dz);
		glTexCoord2f(100.0f, 100.0f); glVertex3d( dx,  dy, -dz);
		glTexCoord2f(  0.0f, 100.0f); glVertex3d(-dx,  dy, -dz);

		glNormal3d(0., -1., 0.);
		glTexCoord2f(  0.0f,   0.0f); glVertex3d(-dx, -dy,  dz);
		glTexCoord2f(100.0f,   0.0f); glVertex3d( dx, -dy,  dz);
		glTexCoord2f(100.0f, 100.0f); glVertex3d( dx, -dy, -dz);
		glTexCoord2f(  0.0f, 100.0f); glVertex3d(-dx, -dy, -dz);
	glEnd();
}

void oglMesh (float *fVertex, float *fNormal, float *fTextureCoord, int iTriangleCount)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, fVertex);

	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, 0, fNormal);

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(3, GL_FLOAT, 0, fTextureCoord);
	glDrawArrays(GL_TRIANGLES, 0, 3*iTriangleCount);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}

