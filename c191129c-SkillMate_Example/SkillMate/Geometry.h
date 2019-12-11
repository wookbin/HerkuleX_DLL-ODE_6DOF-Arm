#pragma once 

// ODE에 정의된 자료구조
struct dxGeom;
struct dxJoint;

typedef struct dxGeom *dGeomID;
typedef struct dxJoint *dJointID;

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Geometry
//////////////////////////////////////////////////////////////////////////////////////////////////////

struct sSensor;

struct sGeometry 
{
	COLORREF _color;		// 지오메트리의 색
	dGeomID _geomID;		// ODE geometry의 ID
	sSensor *_sensor;	// 지오메트리에 센서 속성이 설정된 경우 sSensor 구조체의 객체 할당		

	sGeometry (COLORREF color, dGeomID geomID) 
		: _color(color), _geomID(geomID), _sensor(NULL) { }
	
	virtual ~sGeometry ();

	virtual void Draw () const { }
};

struct sBox : public sGeometry 
{
	double _dx, _dy, _dz;

	sBox (COLORREF color, dGeomID geomID, double dx, double dy, double dz) 
		: sGeometry (color, geomID), _dx(dx), _dy(dy), _dz(dz) { }

	virtual void Draw() const;
};

struct sSphere : public sGeometry 
{
	double _radius;

	sSphere (COLORREF color, dGeomID geomID, double radius) 
		: sGeometry (color, geomID), _radius(radius) { }

	virtual void Draw() const;
};

struct sCylinder : public sGeometry 
{
	double _radius, _height;

	sCylinder (COLORREF color, dGeomID geomID, double radius, double height) 
		: sGeometry (color, geomID), _radius(radius), _height(height) { }

	virtual void Draw() const;
};

struct sCapsule : public sGeometry 
{
	double _radius, _height;

	sCapsule (COLORREF color, dGeomID geomID, double radius, double height) 
		: sGeometry (color, geomID), _radius(radius), _height(height) { }

	virtual void Draw() const;
};

struct sRay : public sGeometry 
{
	double _length;

	sRay (COLORREF color, dGeomID geomID, double length) 
		: sGeometry (color, geomID), _length(length) { }

	virtual void Draw() const;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Joint
//////////////////////////////////////////////////////////////////////////////////////////////////////

struct sJoint
{	
	dJointID _jointID;
	bool _posCtrl;

	sJoint (dJointID jointID_);
	~sJoint ();

	virtual void PosController () { } 
};

struct sFixedJoint : public sJoint
{
	sFixedJoint (dJointID jointID)
		: sJoint (jointID) { }
};

struct sHingeJoint : public sJoint
{	
	// hinge joint가 추종해야 할 각도
	double _angle;

	sHingeJoint (dJointID jointID) 
		: sJoint (jointID), _angle(0.) { }

	virtual void PosController ();
};

struct sBallJoint : public sJoint
{	
	// ball joint가 추종해야 할 roll, pitch, yaw 각도
	double _roll, _pitch, _yaw;

	sBallJoint (dJointID jointID) 
		: sJoint (jointID), _roll(0.), _pitch(0.), _yaw(0.) { }

	virtual void PosController ();
};

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Sensor
//////////////////////////////////////////////////////////////////////////////////////////////////////

class CCamOgl;

enum eSensorType {
	ST_LOCATION,
	ST_TOUCH,
	ST_DISTANCE,
	ST_GYRO,
	ST_ACCEL,
	ST_FT,
	ST_CAMERA,
};

struct sSensor {
	eSensorType _type;
	bool _touch;
	double _distance;
	CCamOgl *_cam;

	sSensor (eSensorType type, CCamOgl *cam = NULL) 
		: _type(type), _cam(cam), _touch(false), _distance(0) { }

	virtual ~sSensor ();
};

