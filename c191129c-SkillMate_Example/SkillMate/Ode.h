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

	// 박스, 구, 실린더, 캡슐 모양의 물체를 생성하기 위한 함수
	// name = 물체의 이름 (생성되는 물체의 이름은 달라야함)
	// mass = 0.이상의 물체의 무게
	// color = 생성되는 물체의 색깔 RGB()
	// x,y,z,roll,pitch,yaw = global 좌표에서의 물체의 초기위치 설정
	void AddBox		(const char *name, double mass, COLORREF color,	double x, double y, double z, double roll, double pitch, double yaw, double dx, double dy, double dz);
	void AddSphere	(const char *name, double mass, COLORREF color,	double x, double y, double z, double roll, double pitch, double yaw, double radius);
	void AddCylinder(const char *name, double mass, COLORREF color,	double x, double y, double z, double roll, double pitch, double yaw, double radius, double height);
	void AddCapsule (const char *name, double mass, COLORREF color,	double x, double y, double z, double roll, double pitch, double yaw, double radius, double height);
	// ray는 z축으로 length 만큼 뻗어나간 선분이다.
	void AddRay		(const char *name,              COLORREF color,	double x, double y, double z, double roll, double pitch, double yaw, double length);
	
	// 두 지오메트리를 하나의 강체로 연결한다.
	void SetRigidBody (const char *base_name, const char *name);

	// 생성한 Link들을 연결시켜 주기 위한 joint 생성함수
	// prev_name과 next_name에 joint를 생성
	// hx, hy, hz = 힌지의 중심축 방향 설정 
	// 0,  0,  1 (z축방향 회전)
	// 0,  1,  0 (y축방향 회전)
	// 1,  0,  0 (x축방향 회전)
	// ax, ay, az = joint의 중심축을 x방향으로 ax만큼, y방향으로 ay만큼, z방향으로 az 만큼이동
	// lo, hi = joint의 하한 멈춤각, 상한 멈춤각
	// vel, fMax = joint의 속도와 최고속도
	void AttachFixedJoint(const char *name, const char *prev_name, const char *next_name); 
	void AttachHingeJoint(const char *name, const char *prev_name, const char *next_name, double hx, double hy, double hz, double ax, double ay, double az, double fMax, double lo = -dInfinity, double hi = dInfinity);
	void AttachBallJoint (const char *name, const char *prev_name, const char *next_name, double ax,double ay,double az, double fMax, double lo = -dInfinity, double hi = dInfinity);

	double GetJointAngle(const char *jointName, int axis = 0);	

	// joint의 position을 설정
	// axis == 0이면 roll, axis == 1이면 pitch, axis == 2이면 yaw 방향으로 pos만큼 회전 (pos는 DEGREE 값)
	void SetJointAngle	(const char *name, int axis, double pos);

	//joint의 velocity을 설정
	//name의 이름을 가지는 joint를 vel만큼의 속도로 돌린다.
	void SetJointVelocity	(const char *name, int axis, double vel);

	// Sensor 1:
	// 센서를 설정할 때는 항상 센서를 다른 지오메트리(base_name)에 고정해야 한다.

	// 센서가 설치된 곳의 x, y, z 위치를 측정
	void SetLocationSensor(const char *name, const char *base_name);
	// 센서가 다른 물체와 접촉 여부를 측정
	void SetTouchSensor   (const char *name, const char *base_name);
	// ray가 다른 물체와 만나는 거리를 측정
	void SetDistanceSensor(const char *name, const char *base_name, double length);

	// Sensor로 부터 측정된 값을 가져온다.
	void GetLocation(const char *name, double &x,double &y, double &z);
	bool GetTouch(const char *name);
	double GetDistance(const char *name);

	// Sensor 2:

	// 각속도를 측정하기위한 센서.
	void SetGyroSensor(const char *name, const char *base_name);
	// 가속도를 측정하기위한 센서. 가속도 센서는 다른 지오메트리와 충돌하지 않는다.
	void SetAccSensor(const char *name, const char *base_name);
	// force와 torque를 측정하기위한 센서. 두개의 지오메트리 사이에 연결해야 한다.
	void SetFTSensor(const char *name, const char *prev_name, const char *next_name);

	// Sensor로 부터 측정된 값을 가져온다.
	void GetGyroValue(const char *name, double &velX, double &velY, double &velZ);
	void GetAccValue(const char *name, double &accX, double &accY, double &accZ);
	void GetFTValue(const char *name, double &fx, double &fy, double &fz, double &tx, double &ty, double &tz);

	// Camera
	void SetCamera(const char *name, const char *base_name, int width, int height, int fov);

	CCamOgl *GetCamera (const char *name);

private:
	// Joint 관련
	void StepJointController();		//P control를 이용한 joint의 위치제어

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
