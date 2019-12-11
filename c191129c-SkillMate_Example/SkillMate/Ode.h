#pragma once

#include "../../ode-0.11.1/include/ode/ode.h"
#include "../../ode-0.11.1/ode/src/mat.h"
#include "Geometry.h"
#include <string>
#include <map>

using namespace std;

class COde
{
public:
	COde(); 
	~COde ();

	void Step (double dt);
	map<string, sGeometry *> *pGetOdeGeomList () { return &_geomList; }

	// �ڽ�, ��, �Ǹ���, ĸ�� ����� ��ü�� �����ϱ� ���� �Լ�
	// name = ��ü�� �̸� (�����Ǵ� ��ü�� �̸��� �޶����)
	// mass = 0.�̻��� ��ü�� ����
	// color = �����Ǵ� ��ü�� ���� RGB()
	// x,y,z,roll,pitch,yaw = global ��ǥ������ ��ü�� �ʱ���ġ ����
	void AddBox		(const char *name, double mass, COLORREF color,	double x, double y, double z, double roll, double pitch, double yaw, double dx, double dy, double dz);
	void AddSphere	(const char *name, double mass, COLORREF color,	double x, double y, double z, double roll, double pitch, double yaw, double radius);
	void AddCylinder(const char *name, double mass, COLORREF color,	double x, double y, double z, double roll, double pitch, double yaw, double radius, double height);
	void AddCapsule (const char *name, double mass, COLORREF color,	double x, double y, double z, double roll, double pitch, double yaw, double radius, double height);
	// ray�� z������ length ��ŭ ����� �����̴�.
	void AddRay		(const char *name,              COLORREF color,	double x, double y, double z, double roll, double pitch, double yaw, double length);
	
	// �� ������Ʈ���� �ϳ��� ��ü�� �����Ѵ�.
	void SetRigidBody (const char *base_name, const char *name);

	// ������ Link���� ������� �ֱ� ���� joint �����Լ�
	// prev_name�� next_name�� joint�� ����
	// hx, hy, hz = ������ �߽��� ���� ���� 
	// 0,  0,  1 (z����� ȸ��)
	// 0,  1,  0 (y����� ȸ��)
	// 1,  0,  0 (x����� ȸ��)
	// ax, ay, az = joint�� �߽����� x�������� ax��ŭ, y�������� ay��ŭ, z�������� az ��ŭ�̵�
	// lo, hi = joint�� ���� ���㰢, ���� ���㰢
	// vel, fMax = joint�� �ӵ��� �ְ�ӵ�
	void AttachFixedJoint(const char *name, const char *prev_name, const char *next_name); 
	void AttachHingeJoint(const char *name, const char *prev_name, const char *next_name, double hx, double hy, double hz, double ax, double ay, double az, double fMax, double lo = -dInfinity, double hi = dInfinity);
	void AttachBallJoint (const char *name, const char *prev_name, const char *next_name, double ax,double ay,double az, double fMax, double lo = -dInfinity, double hi = dInfinity);

	double GetJointAngle(const char *jointName, int axis = 0);	

	// joint�� position�� ����
	// axis == 0�̸� roll, axis == 1�̸� pitch, axis == 2�̸� yaw �������� pos��ŭ ȸ�� (pos�� DEGREE ��)
	void SetJointAngle	(const char *name, int axis, double pos);

	//joint�� velocity�� ����
	//name�� �̸��� ������ joint�� vel��ŭ�� �ӵ��� ������.
	void SetJointVelocity	(const char *name, int axis, double vel);

	// Sensor 1:
	// ������ ������ ���� �׻� ������ �ٸ� ������Ʈ��(base_name)�� �����ؾ� �Ѵ�.

	// ������ ��ġ�� ���� x, y, z ��ġ�� ����
	void SetLocationSensor(const char *name, const char *base_name);
	// ������ �ٸ� ��ü�� ���� ���θ� ����
	void SetTouchSensor   (const char *name, const char *base_name);
	// ray�� �ٸ� ��ü�� ������ �Ÿ��� ����
	void SetDistanceSensor(const char *name, const char *base_name, double length);

	// Sensor�� ���� ������ ���� �����´�.
	void GetLocation(const char *name, double &x,double &y, double &z);
	bool GetTouch(const char *name);
	double GetDistance(const char *name);

	// Sensor 2:

	// ���ӵ��� �����ϱ����� ����.
	void SetGyroSensor(const char *name, const char *base_name);
	// ���ӵ��� �����ϱ����� ����. ���ӵ� ������ �ٸ� ������Ʈ���� �浹���� �ʴ´�.
	void SetAccSensor(const char *name, const char *base_name);
	// force�� torque�� �����ϱ����� ����. �ΰ��� ������Ʈ�� ���̿� �����ؾ� �Ѵ�.
	void SetFTSensor(const char *name, const char *prev_name, const char *next_name);

	// Sensor�� ���� ������ ���� �����´�.
	void GetGyroValue(const char *name, double &velX, double &velY, double &velZ);
	void GetAccValue(const char *name, double &accX, double &accY, double &accZ);
	void GetFTValue(const char *name, double &fx, double &fy, double &fz, double &tx, double &ty, double &tz);

	// Camera
	void SetCamera(const char *name, const char *base_name, int width, int height, int fov);

	CCamOgl *GetCamera (const char *name);

private:
	// Joint ����
	void StepJointController();		//P control�� �̿��� joint�� ��ġ����

	void ResetSensorData();
	bool SetSensorData (dGeomID geomID, double distance);

private:
	friend void nearCallback (void *data, dGeomID o1, dGeomID o2);

	void OdeSetBodyPosition (dGeomID geomID, double x, double y, double z, double roll, double pitch, double yaw);

	sGeometry *FindGeometry (const char *name);
	sJoint *FindJoint (const char *name);
	const char *FindGeomName(dGeomID geomID);

	dWorldID _world; 
	dSpaceID _space; 
	dJointGroupID _contactgroup; 
	dGeomID _ground; 

	map<string, sGeometry *> _geomList;
	map<string, sJoint *> _jointList;
};
