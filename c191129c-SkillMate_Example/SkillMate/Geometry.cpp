#include "stdafx.h"
#include "../../ode-0.11.1/include/ode/ode.h"
#include <gl/glu.h>
#include "mm.h"
#include "CamOgl.h"
#include "OglObjects.h" 
#include "Geometry.h" 

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Geometry
//////////////////////////////////////////////////////////////////////////////////////////////////////

sGeometry::~sGeometry ()
{
	if (_sensor) {
		delete _sensor;
	}
}

void sBox::Draw() const
{
	glColor3d (GetRValue(_color)/255., GetGValue(_color)/255., GetBValue(_color)/255.);

	oglBox (_dx, _dy, _dz);
}

void sSphere::Draw() const
{
	glColor3d(GetRValue(_color)/255., GetGValue(_color)/255., GetBValue(_color)/255.);

	oglSphere (_radius);
}

void sCylinder::Draw() const
{
	glColor3d(GetRValue(_color)/255., GetGValue(_color)/255., GetBValue(_color)/255.);

	oglCylinder (_height, _radius);
}

void sCapsule::Draw () const 
{
	glColor3d(GetRValue(_color)/255., GetGValue(_color)/255., GetBValue(_color)/255.);

	oglCapsule (_height, _radius);
}

// ODE에서 ray는 좀 특별하게 그린다.
static void oglRay (double length, dVector3 ori, dVector3 dir, float width)
{
	glDisable(GL_LIGHTING);

	float oldWidth;
	glGetFloatv (GL_LINE_WIDTH, &oldWidth);

	glLineWidth(1.0f);

	double x = ori[0] + (dir[0] * length);
	double y = ori[1] + (dir[1] * length);
	double z = ori[2] + (dir[2] * length);

	oglLine (ori[0], ori[1], ori[2], x, y, z);
	
	glLineWidth(oldWidth);

	glEnable(GL_LIGHTING);
}

void sRay::Draw () const 
{
	glColor3d(GetRValue(_color)/255., GetGValue(_color)/255., GetBValue(_color)/255.);

	dVector3 ori, dir;
	dGeomRayGet(_geomID, ori, dir);
	dReal length = dGeomRayGetLength(_geomID);
	
	if (_sensor && _sensor->_type == ST_DISTANCE) {
		length = min (length, _sensor->_distance);
	}

	oglRay (length, ori, dir, 1.f);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Joint
//////////////////////////////////////////////////////////////////////////////////////////////////////

sJoint::sJoint (dJointID jointID) 
	: _jointID(jointID), _posCtrl(false)
{ 
	dJointSetFeedback (jointID, new dJointFeedback);
}

sJoint::~sJoint ()
{
	delete dJointGetFeedback (_jointID);
}

void sHingeJoint::PosController ()
{
	const double kp = 10.0;
	
	dReal e = kp * (_angle - dJointGetHingeAngle (_jointID));
	dJointSetHingeParam (_jointID, dParamVel, e);
}

void sBallJoint::PosController ()
{
	double kp = 10.0;
/*
	// Current angle
	dMatrix3 Rc;
	dRFromEulerAngles (Rc, 
		dJointGetAMotorAngle (_jointID, 0), 
		dJointGetAMotorAngle (_jointID, 1), 
		dJointGetAMotorAngle (_jointID, 2));	

	dQuaternion qc;
	dQfromR (qc, Rc);

	// Target angle
	dMatrix3 Rt;
	dRFromEulerAngles (Rt, _roll, _pitch, _yaw);	

	dQuaternion qt;
	dQfromR (qt, Rt);

	// Target - Current angle
	dQuaternion qa;
	dQMultiply1 (qa, qt, qc);

	dJointSetAMotorParam (_jointID, dParamVel,  kp*qa[1]);
	dJointSetAMotorParam (_jointID, dParamVel2, kp*qa[2]);
	dJointSetAMotorParam (_jointID, dParamVel3, kp*qa[3]);
*/

	double e;

	e = kp * DeltaRad (_roll, dJointGetAMotorAngle (_jointID, 0));
	dJointSetAMotorParam (_jointID, dParamVel, e);

	e = kp * DeltaRad (_pitch, dJointGetAMotorAngle (_jointID, 1));
	dJointSetAMotorParam (_jointID, dParamVel2, e);

	e = kp * DeltaRad (_yaw, dJointGetAMotorAngle (_jointID, 2));
	dJointSetAMotorParam (_jointID, dParamVel3, e);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Sensor
//////////////////////////////////////////////////////////////////////////////////////////////////////

sSensor::~sSensor () 
{ 
	if (_cam) {
		delete _cam;
	}
}