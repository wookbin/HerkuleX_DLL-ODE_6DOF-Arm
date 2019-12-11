#include "stdafx.h"
#include "CamOgl.h"
#include "Ode.h"

COde::COde() 
{
	dInitODE();

	_world = dWorldCreate();				//월드를 생성
	_space = dHashSpaceCreate (0);			//공간을 생성
	_contactgroup = dJointGroupCreate (0);  //관절그룹을 생성

	dWorldSetGravity (_world,0, 0, -9.8);	//월드에 중력을 설정
	_ground = dCreatePlane (_space,0,0,1,0);//지면을 생성
}

COde::~COde ()
{
	for (map<string, sGeometry *>::iterator it = _geomList.begin(); it != _geomList.end(); it++) {
		delete it->second;
	}

	for (map<string, sJoint *>::iterator it = _jointList.begin(); it != _jointList.end(); it++) {
		delete it->second;		
	}

	dJointGroupDestroy (_contactgroup); 
	dSpaceDestroy (_space); 
	dWorldDestroy (_world); 

	dCloseODE();
}

static void nearCallback (void *data, dGeomID o1, dGeomID o2) 
{ 
	COde *ode = (COde *)data;

	dBodyID b1 = dGeomGetBody(o1);
	dBodyID b2 = dGeomGetBody(o2);

	if (b1 && b2 && dAreConnectedExcluding (b1, b2, dJointTypeContact)) return;

	const int MAX_CONTACTS = 8;
	dContact contact[MAX_CONTACTS];

	//충돌검사를 통해 충돌이 일어난 물체의 수 반환
	int numc = dCollide(o1, o2, MAX_CONTACTS, &contact[0].geom, sizeof(dContact)); 

	for (int i=0; i<numc; i++) {
		dContactGeom g = contact[i].geom;

		if (dGeomGetClass(o1) == dRayClass && dGeomGetClass(o2) == dRayClass) {
			// 두 선분(ray)가 충돌한 경우는 조인트를 만들지 않는다.
		}
		else if (ode->SetSensorData (g.g1, g.depth) || ode->SetSensorData (g.g2, g.depth)) {
			// touch, distance, acceleration 센서는 충돌한 경우 조인트를 만들지 않는다.
		}
		else {
			//contact이 일어난 점의 모드를 설정
			//mu (쿨롱 마찰계수 0에서 dInfinity범위다. 0은 마찰이없는 접촉, dInfinity는 절대로 미끄러지지 않는 접촉
			//slip1과 slip2는 마찰방향 1과 2의 힘종속 미끄럼
			//ERP는 시뮬레이션 도중 일어나는 관절의 오류 비율이다. 0과 1사이의 값으로 설정한다.
			//CFM = 0 이면 joint의 구속이 딱딱해지고, 증가함에따라 부드러움도 증가한다.
			contact[i].surface.mode = dContactSlip1 | dContactSlip2 | dContactSoftERP | dContactSoftCFM | dContactApprox1;

			contact[i].surface.mu       = 5;
			contact[i].surface.slip1    = 0.001;
			contact[i].surface.slip2    = 0.001;
			contact[i].surface.soft_erp = 0.9;
			contact[i].surface.soft_cfm = 0.002;

			//contact 모드에따라 contact이 일어난 점 생성
			dJointID c = dJointCreateContact(ode->_world, ode->_contactgroup, &contact[i]);
			dJointAttach (c, b1, b2);
		}
	}
} 

void COde::Step (double dt)
{
	StepJointController();

	// 새로운 스탭을 시작하기 전에 이전 스탭의 모든 distance와 touch 측정 결과를 지운다.
	ResetSensorData();

	//dSpaceCollide (_space, this, &nearCallback); //ODE 공간상에 충돌 선별 차단_wbjin 150227
	dWorldStep (_world, dt);					 //시뮬레이션 stepsize 설정
	dJointGroupEmpty (_contactgroup);			 //다음 충돌검사를 위해 contactgroup 초기화
}

void COde::OdeSetBodyPosition (dGeomID geomID, double x, double y, double z, double roll, double pitch, double yaw)
{
	dBodyID bodyID = dGeomGetBody (geomID);

	//물체의 위치조정
	dBodySetPosition (bodyID, x, y, z); 
	
	//오일러 각을 사용하여 물체의 방향 조정
	dMatrix3 R;
	dRFromEulerAngles (R, roll, pitch, yaw);	
	dBodySetRotation (bodyID, R);				
}

const char *COde::FindGeomName(dGeomID geomID)
{
	for (map<string, sGeometry *>::iterator it = _geomList.begin (); it != _geomList.end (); ++it) {
		sGeometry *geom = it->second;
		if (geom->_geomID == geomID) return it->first.c_str();
	}
	return NULL;
}


sGeometry *COde::FindGeometry (const char *name)
{
	map<string, sGeometry *>::iterator it = _geomList.find (name);
	if (it != _geomList.end ()) {
		return it->second;
	}
	return NULL;
}

sJoint *COde::FindJoint (const char *name)
{
	map<string, sJoint *>::iterator it = _jointList.find(name);
	if (it != _jointList.end ()) {
		return it->second;
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Geometry
//////////////////////////////////////////////////////////////////////////////////////////////////////

void COde::AddBox (const char *name, double mass, COLORREF color,	double x, double y, double z, double roll, double pitch, double yaw, double dx, double dy, double dz)
{
	dBodyID bodyID = dBodyCreate (_world);

	dMass m;
	dMassSetZero (&m);
	dMassSetBoxTotal (&m, mass, dx, dy, dz);
	dMassAdjust (&m, mass); 	
	dBodySetMass (bodyID, &m); 
	
	dGeomID geomID = dCreateBox (_space, dx, dy, dz); 
	dGeomSetBody (geomID, bodyID); 

	OdeSetBodyPosition (geomID, x, y, z, roll, pitch, yaw);

	pair<map<string, sGeometry *>::iterator, bool> ret = 
		_geomList.insert (make_pair(name, new sBox (color, geomID, dx, dy, dz)));
	assert (ret.second && "Geometry already existed");
	dGeomSetData (geomID, (void *)ret.first->second);
}

void COde::AddSphere (const char *name, double mass, COLORREF color,	double x, double y, double z, double roll, double pitch, double yaw, double radius)
{
	dBodyID bodyID = dBodyCreate (_world); 

	dMass m;
	dMassSetZero (&m);
	dMassSetSphereTotal (&m, mass, radius);
	dMassAdjust (&m, mass); 
	dBodySetMass (bodyID, &m); 

	dGeomID geomID = dCreateSphere (_space, radius);
	dGeomSetBody (geomID, bodyID); 

	OdeSetBodyPosition (geomID, x, y, z, roll, pitch, yaw);

	pair<map<string, sGeometry *>::iterator, bool> ret = 
		_geomList.insert (make_pair(name, new sSphere (color, geomID, radius)));
	assert (ret.second && "Geometry already existed");
	dGeomSetData (geomID, ret.first->second);
}

void COde::AddCylinder (const char *name, double mass, COLORREF color,	double x, double y, double z, double roll, double pitch, double yaw, double radius, double height)
{
	dBodyID bodyID = dBodyCreate (_world); 

	dMass m;
	dMassSetZero(&m);
	dMassSetCylinderTotal (&m, mass, 3, radius, height);
	dMassAdjust (&m, mass); 
	dBodySetMass (bodyID, &m); 

	dGeomID geomID = dCreateCylinder (_space, radius, height);
	dGeomSetBody (geomID, bodyID); 

	OdeSetBodyPosition (geomID, x, y, z, roll, pitch, yaw);

	pair<map<string, sGeometry *>::iterator, bool> ret = 
		_geomList.insert (make_pair(name, new sCylinder (color, geomID, radius, height)));
	assert (ret.second && "Geometry already existed");
	dGeomSetData (geomID, ret.first->second);
}

void COde::AddCapsule (const char *name, double mass, COLORREF color,	double x, double y, double z, double roll, double pitch, double yaw, double radius, double height)
{
	dBodyID bodyID = dBodyCreate (_world); 
	
	dMass m;
	dMassSetZero (&m);
	dMassSetCapsuleTotal (&m, mass, 3, radius, height);
	dMassAdjust (&m, mass); 
	dBodySetMass (bodyID, &m); 

	dGeomID geomID = dCreateCapsule (_space, radius, height);
	dGeomSetBody (geomID, bodyID); 

	OdeSetBodyPosition (geomID, x, y, z, roll, pitch, yaw);

	pair<map<string, sGeometry *>::iterator, bool> ret = 
		_geomList.insert (make_pair(name, new sCapsule (color, geomID, radius, height)));
	assert (ret.second && "Geometry already existed");
	dGeomSetData (geomID, ret.first->second);
}

void COde::AddRay (const char *name, COLORREF color, double x, double y, double z, double roll, double pitch, double yaw, double length)
{
	dBodyID bodyID = dBodyCreate (_world); 
	
	dGeomID geomID = dCreateRay (_space, length);
	dGeomSetBody (geomID, bodyID);

	OdeSetBodyPosition (geomID, x, y, z, roll, pitch, yaw);

	dBodyDisable(bodyID);

	pair<map<string, sGeometry *>::iterator, bool> ret = 
		_geomList.insert (make_pair(name, new sRay (color, geomID, length)));
	assert (ret.second && "Geometry already existed");
	dGeomSetData (geomID, ret.first->second);
}

// 두 지오메트리를 하나의 강체로 연결한다.
void COde::SetRigidBody (const char *base_name, const char *name)
{
	sGeometry *geom1 = FindGeometry (base_name);
	sGeometry *geom2 = FindGeometry (name);
	assert (geom1 && geom2);
	
	dBodyID body1 = dGeomGetBody (geom1->_geomID);
	dBodyID body2 = dGeomGetBody (geom2->_geomID);
		
	dMass m1, m2;
	dBodyGetMass (body1, &m1);
	dBodyGetMass (body2, &m2);
	
	const dReal *pos1 = dBodyGetPosition (body1);
	const dReal *pos2 = dGeomGetPosition (geom2->_geomID);
	const dReal *rot1 = dBodyGetRotation (body1);
	const dReal *rot2 = dBodyGetRotation (body2);

	dMassRotate (&m1, rot1);		
	dMassTranslate (&m1, pos1[0], pos1[1], pos1[2]);

	// 지오메트리 종류가 ray가 아닌 경우만 수행
	if (dGeomGetClass(geom2->_geomID) != dRayClass) {
		dMassRotate (&m2, rot2);
		dMassTranslate (&m2,pos2[0], pos2[1], pos2[2]);

		dMassAdd(&m1, &m2);
	}

	dGeomID geom = dBodyGetFirstGeom (body1);
	while (geom) {
		const dReal *pos = dGeomGetPosition (geom);
		const dReal *rot = dGeomGetRotation (geom);
		dGeomSetOffsetPosition (geom, pos[0]-m1.c[0], pos[1]-m1.c[1], pos[2]-m1.c[2]);
		dGeomSetOffsetRotation (geom, rot);
		geom = dBodyGetNextGeom (geom);
	}

	dGeomSetBody (geom2->_geomID, body1);

	dGeomSetOffsetPosition (geom2->_geomID, pos2[0]-m1.c[0], pos2[1]-m1.c[1], pos2[2]-m1.c[2]);
	dGeomSetOffsetRotation (geom2->_geomID, rot2);		

	dBodySetPosition (body1, m1.c[0], m1.c[1], m1.c[2]); 
	dMatrix3 R;
	dRSetIdentity (R);
	dBodySetRotation (body1, R);
	
	dMassTranslate (&m1, -m1.c[0], -m1.c[1], -m1.c[2]);
	dBodySetMass (body1, &m1);
	dBodyDestroy (body2);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Joint
//////////////////////////////////////////////////////////////////////////////////////////////////////

//prev_name과 next_name를 고정시켜주는 Joint를 생성
void COde::AttachFixedJoint(const char *name, const char *prev_name, const char *next_name)
{
	dBodyID body_prev = 0;
	dBodyID body_next = 0;

	if (prev_name && *prev_name) {
		sGeometry *geom_prev = FindGeometry (prev_name);
		assert (geom_prev);

		body_prev = dGeomGetBody (geom_prev->_geomID);
	}
	if (next_name && *next_name) {
		sGeometry *geom_next = FindGeometry (next_name);
		assert (geom_next);

		body_next = dGeomGetBody (geom_next->_geomID);
	}
	assert (body_prev || body_next);

	// Fixed joint 생성
	dJointID jointID = dJointCreateFixed(_world, 0);

	dJointAttach (jointID, body_prev, body_next);

	dJointSetFixed (jointID);

	pair<map<string, sJoint *>::iterator, bool> ret = 
		_jointList.insert(make_pair(name, new sFixedJoint (jointID)));	
	assert (ret.second && "Joint already existed");
	dJointSetData (jointID, ret.first->second);
}

//prev_name과 next_name를 연결하는 힌지 Joint 생성
void COde::AttachHingeJoint(const char *name, const char *prev_name, const char *next_name, double hx, double hy, double hz, double ax, double ay, double az, double fMax, double lo, double hi)
{
	if (0 <= lo) lo = -0.001;
	if (hi <= 0) hi = +0.001;

	sGeometry *geom_prev = FindGeometry (prev_name);
	sGeometry *geom_next = FindGeometry (next_name);
	assert (geom_prev && geom_next);
	
	dBodyID body_prev = dGeomGetBody (geom_prev->_geomID);
	dBodyID body_next = dGeomGetBody (geom_next->_geomID);

	// Hinge joint 생성
	dJointID jointID = dJointCreateHinge(_world, 0);

	dJointAttach(jointID, body_prev, body_next);

	// Hinge joint의 회전 축과 회전 중심점 설정
	dJointSetHingeAxis(jointID, hx, hy, hz);
	dJointSetHingeAnchor(jointID, ax, ay, az);

	// Hinge joint의 각종 파라메터 설정
	dJointSetHingeParam(jointID, dParamLoStop, lo);
	dJointSetHingeParam(jointID, dParamHiStop, hi);
	dJointSetHingeParam(jointID, dParamVel, 0.);
	dJointSetHingeParam(jointID, dParamFMax, fMax);

	pair<map<string, sJoint *>::iterator, bool> ret = 
		_jointList.insert(make_pair(name, new sHingeJoint (jointID)));	
	assert (ret.second && "Joint already existed");
	dJointSetData (jointID, ret.first->second);
}

//prev_name과 next_name를 연결하는 볼 Joint 생성
void COde::AttachBallJoint(const char *name, const char *prev_name, const char *next_name, double ax, double ay, double az, double fMax, double lo, double hi)
{
	if (0 <= lo) lo = -0.001;
	if (hi <= 0) hi = +0.001;

	sGeometry *geom_prev = FindGeometry (prev_name);
	sGeometry *geom_next = FindGeometry (next_name);
	assert (geom_prev && geom_next);;
	
	dBodyID body_prev = dGeomGetBody (geom_prev->_geomID);
	dBodyID body_next = dGeomGetBody (geom_next->_geomID);

	// Ball joint 생성
	dJointID jointID  = dJointCreateBall (_world, 0);
	
	dJointAttach (jointID, body_prev, body_next);
	
	// Ball joint의 회전 중심점 설정
	dJointSetBallAnchor (jointID, ax, ay, az);

	// Ball joint를 움직일 모터 생성
	jointID = dJointCreateAMotor (_world, 0);

	dJointAttach (jointID, body_prev, body_next);

	dJointSetAMotorNumAxes(jointID, 3);

	dJointSetAMotorAxis(jointID, 0,1, 1,0,0);
	dJointSetAMotorAxis(jointID, 1,1, 0,1,0);
	dJointSetAMotorAxis(jointID, 2,1, 0,0,1);
	dJointSetAMotorMode(jointID, dAMotorEuler);

	dJointSetAMotorParam(jointID, dParamLoStop, lo);
	dJointSetAMotorParam(jointID, dParamHiStop, hi);
	dJointSetAMotorParam(jointID, dParamVel, 0.);
	dJointSetAMotorParam(jointID, dParamFMax, fMax); 

	dJointSetAMotorParam(jointID, dParamLoStop2, lo);
	dJointSetAMotorParam(jointID, dParamHiStop2, hi);
	dJointSetAMotorParam(jointID, dParamVel2, 0.);
	dJointSetAMotorParam(jointID, dParamFMax2, fMax); 

	dJointSetAMotorParam(jointID, dParamLoStop3, lo);
	dJointSetAMotorParam(jointID, dParamHiStop3, hi);
	dJointSetAMotorParam(jointID, dParamVel3, 0.);
	dJointSetAMotorParam(jointID, dParamFMax3, fMax);	

	pair<map<string, sJoint *>::iterator, bool> ret = 
		_jointList.insert(make_pair(name, new sBallJoint (jointID)));	
	assert (ret.second && "Joint already existed");
	dJointSetData (jointID, ret.first->second);
}

double COde::GetJointAngle(const char *jointName, int axis)
{
	// 조인트의 현재 각도값을 얻어온다.
	sJoint *joint = FindJoint(jointName);

	int type = dJointGetType(joint->_jointID);

	if (type == dJointTypeHinge) {
		return dJointGetHingeAngle(joint->_jointID);
	}
	else if (type == dJointTypeAMotor) {
		if      (axis == 0) return dJointGetAMotorAngle (joint->_jointID, 0);
		else if (axis == 1) return dJointGetAMotorAngle (joint->_jointID, 1);
		else if (axis == 2) return dJointGetAMotorAngle (joint->_jointID, 2);
	}
	return 0.;
}

void COde::SetJointAngle (const char *name, int axis, double pos)
{
	sJoint *joint = FindJoint(name);

	joint->_posCtrl = true;

	// 힌지 형태(회전 축이 1개)의 조인트에 대한 회전 각 설정
	if (sHingeJoint *hinge = dynamic_cast<sHingeJoint *>(joint)) {
		hinge->_angle = pos;
	}
	// 볼 형태(회전 축이 3개: roll, pitch, yaw)의 조인트의 각 축에 대한 회전 각 설정
	else if (sBallJoint *ball = dynamic_cast<sBallJoint *>(joint)) {
		// ODE의 회전 순서는 roll->pitch->yaw 순서임.
		if      (axis == 0) ball->_roll = pos;
		else if (axis == 1) ball->_pitch = pos;
		else if (axis == 2) ball->_yaw = pos;
	}
}

void COde::SetJointVelocity(const char *name, int axis, double vel)
{
	sJoint *joint = FindJoint(name);

	joint->_posCtrl = false;

	// 힌지 형태(회전 축이 1개)의 조인트에 대한 회전 속도 설정
	if (sHingeJoint *hinge = dynamic_cast<sHingeJoint *>(joint)) {
		dJointSetHingeParam (joint->_jointID, dParamVel, vel);
	}
	// 볼 형태(회전 축이 3개: roll, pitch, yaw)의 조인트의 각 축에 대한 회전 속도 설정
	else if (sBallJoint *ball = dynamic_cast<sBallJoint *>(joint)) {
		// ODE의 회전 순서는 roll->pitch->yaw 순서임.
		if      (axis == 0) dJointSetAMotorParam (joint->_jointID, dParamVel, vel);
		else if (axis == 1) dJointSetAMotorParam (joint->_jointID, dParamVel2, vel);
		else if (axis == 2) dJointSetAMotorParam (joint->_jointID, dParamVel3, vel);
	}
}

void COde::StepJointController()
{
	// 모든 조인트에 대한 위치제어기를 한 스탭 동작시킨다.
	for (map<string, sJoint *>::iterator it = _jointList.begin (); it != _jointList.end (); ++it) {
		sJoint *joint = it->second;

		if (joint->_posCtrl) {
			joint->PosController();
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Sensor
//////////////////////////////////////////////////////////////////////////////////////////////////////

void COde::ResetSensorData()
{
	for (map<string, sGeometry *>::iterator it = _geomList.begin (); it != _geomList.end (); ++it) {
		sGeometry *geom = it->second;
		if (geom->_sensor) {
			if (geom->_sensor->_type == ST_TOUCH) {
				geom->_sensor->_touch = false;
			}
			else if (geom->_sensor->_type == ST_DISTANCE) {
				geom->_sensor->_distance = dGeomRayGetLength(geom->_geomID);
			}
			else if (geom->_sensor->_type == ST_CAMERA) {
				const dReal *pos = dGeomGetPosition(geom->_geomID);
				const dReal *rot = dGeomGetRotation(geom->_geomID);
				geom->_sensor->_cam->Draw(pos, rot, &_geomList);
			}
		}
	}
}

bool COde::SetSensorData (dGeomID geomID, double distance)
{
	sGeometry *geom = (sGeometry *)dGeomGetData (geomID);
	if (geom && geom->_sensor) {
		if (geom->_sensor->_type == ST_TOUCH) {
			geom->_sensor->_touch = true;
			return true;
		}
		else if (geom->_sensor->_type == ST_DISTANCE) {
			geom->_sensor->_distance = distance;
			return true;
		}
		else if (geom->_sensor->_type == ST_ACCEL) {
			// 가속도 센서는 다른 지오메트리와 충돌하게 되면 접촉으로 인한 힘을 측정하기 때문에, 
			// 다른 지오메트리와 충돌하지 않도록 true를 리턴한다.
			return true;
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Location Sensor

void COde::SetLocationSensor(const char *name, const char *base_name)
{
	sGeometry *geom = FindGeometry (name);
	assert (geom && !geom->_sensor);

	SetRigidBody (base_name, name);

	geom->_sensor = new sSensor (ST_LOCATION);
}

void COde::GetLocation(const char *name, double &x,double &y, double &z)
{
	sGeometry *geom = FindGeometry (name);
	assert (geom && geom->_sensor && geom->_sensor->_type == ST_LOCATION);
	
	const dReal *pos = dGeomGetPosition(geom->_geomID);
	x = pos[0]; 
	y = pos[1]; 
	z = pos[2];

	const dReal *rot = dGeomGetRotation(geom->_geomID); 
	// Get roll, pitch, yaw from rotation matrix
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Touch Sensor

void COde::SetTouchSensor(const char *name, const char *base_name)
{
	sGeometry *geom = FindGeometry (name);
	assert (geom && !geom->_sensor);

	SetRigidBody (base_name, name);

	geom->_sensor = new sSensor (ST_TOUCH);
}

bool COde::GetTouch(const char *name)
{
	sGeometry *geom = FindGeometry (name);
	assert (geom && geom->_sensor && geom->_sensor->_type == ST_TOUCH);
	
	return geom->_sensor->_touch;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Distance Sensor

void COde::SetDistanceSensor(const char *name, const char *base_name, double length)
{
	sGeometry *geom = FindGeometry (name);
	assert (geom && !geom->_sensor && dGeomGetClass(geom->_geomID) == dRayClass);

	SetRigidBody (base_name, name);

	dGeomRaySetLength (geom->_geomID, length);

	geom->_sensor = new sSensor (ST_DISTANCE);
}

double COde::GetDistance(const char *name)
{	
	sGeometry *geom = FindGeometry(name);
	assert (geom && geom->_sensor && geom->_sensor->_type == ST_DISTANCE);

	return geom->_sensor->_distance;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Gyro Sensor

void COde::SetGyroSensor(const char *name, const char *base_name)
{
	sGeometry *geom = FindGeometry (name);
	assert (geom && !geom->_sensor);

	SetRigidBody (base_name, name);

	geom->_sensor = new sSensor (ST_GYRO);
}

void COde::GetGyroValue(const char *name, double &velX, double &velY, double &velZ)
{
	sGeometry *geom = FindGeometry (name);
	assert (geom && geom->_sensor && geom->_sensor->_type == ST_GYRO);

	dBodyID bodyID = dGeomGetBody (geom->_geomID);

	const dReal *rot_ = dGeomGetRotation(geom->_geomID); 
	const dReal *vel_ = dBodyGetAngularVel(bodyID);
	
	dMatrix R   (3,4, rot_, 1,1);
	dMatrix vel (3,1, vel_, 1,1);
	dMatrix vel_r = R.transpose() * vel;
			
	velX = vel_r(0,0);	
	velY = vel_r(1,0);	
	velZ = vel_r(2,0);	
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
// Acceleration Sensor

void COde::SetAccSensor(const char *name, const char *base_name)
{
	sGeometry *geom = FindGeometry(name);
	assert (geom && !geom->_sensor);

	AttachFixedJoint (name, base_name, name);

	geom->_sensor = new sSensor (ST_ACCEL);
}

void COde::GetAccValue(const char *name, double &accX, double &accY, double &accZ)
{
	sGeometry *geom = FindGeometry (name);
	assert (geom && geom->_sensor && geom->_sensor->_type == ST_ACCEL);

	sJoint *joint = FindJoint(name);
	assert (joint);
	dJointFeedback *feedback = dJointGetFeedback (joint->_jointID);

	dBodyID bodyID = dGeomGetBody (geom->_geomID);

	dMass m;
	dBodyGetMass (bodyID, &m);

	// const dReal *pos_ = dGeomGetPosition(geom->_geomID); 
	const dReal *rot_ = dGeomGetRotation(geom->_geomID); 

	dMatrix R(3,4, rot_, 1,1);
	dMatrix force(3,1, feedback->f2,1,1);
	dMatrix force_r = R.transpose() * force;
			
	accX = force_r(0,0)/m.mass; 
	accY = force_r(1,0)/m.mass; 
	accZ = force_r(2,0)/m.mass;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
// Force/Torque Sensor

void COde::SetFTSensor(const char *name, const char *prev_name, const char *next_name)
{
	sGeometry *geom = FindGeometry(name);
	assert (geom && !geom->_sensor);

	string name2 = string("@") + name;

	AttachFixedJoint (name, prev_name, name);
	AttachFixedJoint (name2.c_str(), name, next_name);

	geom->_sensor = new sSensor (ST_FT);
}

void COde::GetFTValue(const char *name, double &fx, double &fy, double &fz, double &tx, double &ty, double &tz)							   
{
	sGeometry *geom = FindGeometry (name);
	assert (geom && geom->_sensor && geom->_sensor->_type == ST_FT);

	sJoint *joint = FindJoint(name);
	dJointFeedback *feedback = dJointGetFeedback (joint->_jointID);

	//const dReal *pos_ = dGeomGetPosition(geom->_geomID); 
	const dReal *rot_ = dGeomGetRotation(geom->_geomID); 

	dMatrix R(3,4, rot_, 1,1);
	dMatrix force(3,1, feedback->f2,1,1);
	dMatrix torque(3,1, feedback->t2,1,1);
	dMatrix force_r = R.transpose() * force;
	dMatrix torque_r = R.transpose() * torque;
		
	fx = force_r(0,0); 
	fy = force_r(1,0); 
	fz = force_r(2,0);

	tx = torque_r(0,0); 
	ty = torque_r(1,0);	
	tz = torque_r(2,0);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Camera

void COde::SetCamera(const char *name, const char *base_name, int width, int height, int fov)
{
	sGeometry *geom = FindGeometry (name);
	assert (geom && !geom->_sensor);

	SetRigidBody(base_name, name);
	
	CCamOgl *cam = new CCamOgl(name, width, height, fov);

	geom->_sensor = new sSensor (ST_CAMERA, cam);
}

CCamOgl *COde::GetCamera (const char *name)
{
	sGeometry *geom = FindGeometry (name);
	assert (geom && geom->_sensor && geom->_sensor->_type == ST_CAMERA);

	return geom->_sensor->_cam;
}
