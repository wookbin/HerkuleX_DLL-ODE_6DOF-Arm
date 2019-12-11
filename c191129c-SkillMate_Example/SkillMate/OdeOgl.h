#pragma once

#include "OglWnd.h"

class COde;

class COdeOgl : public COglWnd
{
public:
	COdeOgl (COde *ode, const char *name, const RECT &rect, CWnd *parent, UINT ID);
	virtual ~COdeOgl ();

	virtual void OnDraw ();
	virtual void OnHits (int name);

	// 좌표계 xyz-축의 표시 여부를 정한다.
	bool _showAxis;

private:
	void DrawObjects();

	COde *_ode;
};
