
// SkillMateDlg.h : header file
//

#pragma once

#include "Ode.h" 
#include "OdeOgl.h"
#include "resource.h"
#include "Mmsystem.h"
#include "HDLL.h" //HerkuleX DLL Header


// CSkillMateDlg dialog
class CSkillMateDlg : public CDialog
{
// Construction
public:
	CSkillMateDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SKILLMATE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


public:
	//Jog±¸Á¶Ã¼
	CMD_SJog		_p_sjog[253];
	CMD_IJog		_p_ijog[253];
	CMD_MSJog	_p_msjog[253];
	CMD_MIJog		_p_mijog[253];

public:
	// ODE...
	COde *_ode; 
	COdeOgl *_ogl;

	//Display Init//
	void initModel(void);

	//Thread0//
	HANDLE		m_hThread;	
	BOOL			m_bExit;

	// Thread
	static UINT ThreadProc(LPVOID lpParam);

	// Calc Angle to count
	int Set_AngleToCount(double dAngle);

// Implementation
protected:
	HICON m_hIcon;
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButton4();
	double m_dIArm1;
	double m_dIArm2;
	double m_dIArm3;
	double m_dIArm4;
	double m_dIArm5;
	double m_dIArm6;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton10();
	afx_msg void OnBnClickedButton11();
	afx_msg void OnBnClickedButton12();
	afx_msg void OnBnClickedButton13();
};
