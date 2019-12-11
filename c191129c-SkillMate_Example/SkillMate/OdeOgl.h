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

	// ��ǥ�� xyz-���� ǥ�� ���θ� ���Ѵ�.
	bool _showAxis;

private:
	void DrawObjects();

	COde *_ode;
};
