#pragma once

//#include "OglWnd.h"
#include <gl/gl.h>
#include <gl/glu.h>
#include <string>
#include <map>

using namespace std;

struct sGeometry;

class CCamOgl
{
public:
	CCamOgl(const char *name, int width, int height, int fov); 
	~CCamOgl();

	void Draw (const double pos[3], const double rot[12], map<string, sGeometry *> *geoms);

	const BYTE *GetImage () { return _image; }
	BITMAPINFO *GetBitmapInfo () { return &_bitmapInfo; }

private:
	void InitBitmapInfo (int width, int height, int pixelByte);
	void DrawObjects(map<string, sGeometry *> *geoms);

private:
	int _width;
	int _height;
	double _fovAngle;

	BITMAPINFO _bitmapInfo;
	BYTE *_image;
};
