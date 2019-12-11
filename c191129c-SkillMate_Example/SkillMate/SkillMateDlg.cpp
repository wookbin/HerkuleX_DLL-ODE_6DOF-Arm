
// SkillMateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SkillMate.h"
#include "SkillMateDlg.h"
#include "mm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CString strArmData = _T("");
CString strSend_EncoderData = _T("462642670D0A"); //F&Bg


CString strAngle[6] = {_T(""), };
double m_dAngle[6] = {0.0, };

//Save Angle Param//
double m_dSaveAngle[6] = { 0.0, };
double m_dSaveAngle1[6] = { 0.0, };
double m_dSaveAngle2[6] = { 0.0, };
double m_dSaveAngle3[6] = { 0.0, };


// 사용할 모터 id 번호. ex)1 ~ 253까지의 ID table정의
unsigned char szIDs[] = { 0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,
						  0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,
						  0x1f,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2a,0x2b,0x2c,0x2d,
						  0x2e,0x2f,0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,
						  0x3d,0x3e,0x3f,0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,0x4b,
						  0x4c,0x4d,0x4e,0x4f,0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5a,
						  0x5b,0x5c,0x5d,0x5e,0x5f,0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,
						  0x6a,0x6b,0x6c,0x6d,0x6e,0x6f,0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,
						  0x79,0x7a,0x7b,0x7c,0x7d,0x7e,0x7f,0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,
						  0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,0x90,0x91,0x92,0x93,0x94,0x95,0x96,
						  0x97,0x98,0x99,0x9a,0x9b,0x9c,0x9d,0x9e,0x9f,0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,
						  0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,0xb0,0xb1,0xb2,0xb3,0xb4,
						  0xb5,0xb6,0xb7,0xb8,0xb9,0xba,0xbb,0xbc,0xbd,0xbe,0xbf,0xc0,0xc1,0xc2,0xc3,
						  0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,0xd0,0xd1,0xd2,
						  0xd3,0xd4,0xd5,0xd6,0xd7,0xd8,0xd9,0xda,0xdb,0xdc,0xdd,0xde,0xdf,0xe0,0xe1,
						  0xe2,0xe3,0xe4,0xe5,0xe6,0xe7,0xe8,0xe9,0xea,0xeb,0xec,0xed,0xee,0xef,0xf0,
						  0xf1,0xf2,0xf3,0xf4,0xf5,0xf6,0xf7,0xf8,0xf9,0xfa,0xfb,0xfc,0xfd,0xfe };



// CAboutDlg dialog used for App About
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CSkillMateDlg dialog

CSkillMateDlg::CSkillMateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSkillMateDlg::IDD, pParent)
	, m_dIArm1(0)
	, m_dIArm2(0)
	, m_dIArm3(0)
	, m_dIArm4(0)
	, m_dIArm5(0)
	, m_dIArm6(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSkillMateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT7, m_dIArm1);
	DDX_Text(pDX, IDC_EDIT8, m_dIArm2);
	DDX_Text(pDX, IDC_EDIT9, m_dIArm3);
	DDX_Text(pDX, IDC_EDIT10, m_dIArm4);
	DDX_Text(pDX, IDC_EDIT11, m_dIArm5);
	DDX_Text(pDX, IDC_EDIT12, m_dIArm6);
}

BEGIN_MESSAGE_MAP(CSkillMateDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CSkillMateDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CSkillMateDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CSkillMateDlg::OnBnClickedButton3)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON4, &CSkillMateDlg::OnBnClickedButton4)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON5, &CSkillMateDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CSkillMateDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CSkillMateDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CSkillMateDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, &CSkillMateDlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, &CSkillMateDlg::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON11, &CSkillMateDlg::OnBnClickedButton11)
	ON_BN_CLICKED(IDC_BUTTON12, &CSkillMateDlg::OnBnClickedButton12)
	ON_BN_CLICKED(IDC_BUTTON13, &CSkillMateDlg::OnBnClickedButton13)
END_MESSAGE_MAP()


//Thread
UINT CSkillMateDlg::ThreadProc(LPVOID lpParam)
{
	CSkillMateDlg* pDlg = (CSkillMateDlg*)lpParam;
	SetThreadPriority(pDlg->m_hThread, THREAD_PRIORITY_HIGHEST);
	while (pDlg->m_bExit == FALSE)
	{
		for (int i = 0; i < 6; i++)
		{
			RAM_RegisterData_Read(i+1, RAM_CALIBRATED_POSITION, 1);

			m_dAngle[i] = (RAM_MAP_Read(i + 1, RAM_CALIBRATED_POSITION) - 512.0) * 0.325;
			strAngle[i].Format("%.2f", m_dAngle[i]);

			pDlg->GetDlgItem(IDC_EDIT1+i)->SetWindowText(strAngle[i]);

			Sleep(10);
		}
		
	}
	return 0;
}

BOOL CSkillMateDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	initModel();
	
	SetTimer(1, 1, NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSkillMateDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSkillMateDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSkillMateDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSkillMateDlg::OnBnClickedButton1()
{
	// Serial Port Open
	bool m_bResult = false;

	//USB to Serial Port: COM3 , BaudRate: 115200bps
	m_bResult = Connect(3, 115200);
	Sleep(1);
	if (!m_bResult)
		AfxMessageBox(_T("[Error]: Serial Port Open Error !"));

	//초기 OnInitialize호출 시에는 최소 10ms의 간격이 필요합니다.
	//해당 Sample Code에서는 총 6개(ID: 1 ~ 6)의 HerkuleX를 기준으로 설명합니다.
	for (int i = 0; i < 6; i++)
	{
		OnInitialize(i+1);  
		Sleep(10);
	}

	//10ms Graph Display Thread의 생성//
	m_bExit = FALSE;
	AfxBeginThread(ThreadProc, (LPVOID)this);

}

void CSkillMateDlg::OnBnClickedButton2()
{
	//Serial Port Close
	Disconnect();

}

void CSkillMateDlg::OnBnClickedButton3()
{
	for (int i = 0; i < 6; i++)
	{
		m_dSaveAngle[i] = m_dAngle[i];
	}
}


void CSkillMateDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
 
	//_ogl->MoveWindow (0, 0, cx, cy);
}

void CSkillMateDlg::OnDestroy()
{
	CDialog::OnDestroy();

	KillTimer(1);

	//Kill Thread//
	SuspendThread(m_hThread);

	DWORD dwValue;
	DWORD dwExitCode;
	if (m_hThread != NULL)
	{
		if ((dwValue = WaitForSingleObject(m_hThread, 100)) == WAIT_TIMEOUT)
		{
			GetExitCodeThread(m_hThread, &dwExitCode);
			CloseHandle(m_hThread);
		}
		m_hThread = NULL;
	}

	_ogl->DestroyWindow (); 
	delete _ogl; 
	delete _ode;

}

void CSkillMateDlg::initModel()
{
	CRect rect;
	GetClientRect(rect);
	rect.top += 30;
	rect.bottom = 600;
	
	_ode = new COde ();

	//Arm Modeling 
	double m_HerkuleX_size_dx = 0.024;
	double m_HerkuleX_size_dy = 0.045;
	double m_HerkuleX_size_dz = 0.029;
	double m_HerkuleX1_x = 0.0;
	double m_HerkuleX1_y = 0.0097;
	double m_HerkuleX1_z = ((0.01/2.0) + (m_HerkuleX_size_dz/2.0));

	double m_Horn1_r = (0.019/2.0);
	double m_Horn1_h = 0.0046;

	double m_Horn1_x = 0.0;
	double m_Horn1_y = 0.0;
	double m_Horn1_z = (0.01/2.0) + m_HerkuleX_size_dz + (m_Horn1_h / 2.0);

	double m_Braket01_size_dx = 0.034;
	double m_Braket01_size_dy = 0.037;
	double m_Braket01_size_dz = 0.005;
	double m_Braket01_x = 0.0;
	double m_Braket01_y = 0.003;
	double m_Braket01_z = (0.01 / 2.0) + m_HerkuleX_size_dz + (m_Horn1_h / 2.0) + (m_Braket01_size_dz/2.0);

	double m_Braket011_size_dx = 0.024;
	double m_Braket011_size_dy = 0.029;
	double m_Braket011_size_dz = 0.003;
	double m_Braket011_x = 0.0;
	double m_Braket011_y = 0.0;
	double m_Braket011_z = m_Braket01_z + m_Braket011_size_dz;

	double m_HerkuleX2_size_dx = 0.029;
	double m_HerkuleX2_size_dy = 0.024;
	double m_HerkuleX2_size_dz = 0.045;
	double m_HerkuleX2_x = 0.0;
	double m_HerkuleX2_y = 0.0;
	double m_HerkuleX2_z =  (m_Braket011_size_dz/2.0) + m_Braket011_z + (m_HerkuleX2_size_dz / 2.0);

	double m_Horn2_x = -1.0 * ((m_HerkuleX2_size_dx/2.0)+(m_Horn1_h/2.0));
	double m_Horn2_y =  0.0;
	double m_Horn2_z = (m_HerkuleX2_z + 0.0128);

	double m_Braket02_size_dx = 0.041;
	double m_Braket02_size_dy = 0.025;
	double m_Braket02_size_dz = 0.0215;
	double m_Braket02_x = 0.0;
	double m_Braket02_y = 0.0;
	double m_Braket02_z = m_Horn2_z + m_Horn1_r;

	double m_Braket012_x = 0.0;
	double m_Braket012_y = 0.0;
	double m_Braket012_z = m_Braket02_z + (m_Braket02_size_dz/2.0) + (m_Braket011_size_dz/2.0);

	double m_HerkuleX3_size_dx = 0.029;
	double m_HerkuleX3_size_dy = 0.024;
	double m_HerkuleX3_size_dz = 0.045;
	double m_HerkuleX3_x = 0.0;
	double m_HerkuleX3_y = 0.0;
	double m_HerkuleX3_z =  m_Braket012_z + (m_Braket011_size_dz / 2.0) + (m_HerkuleX3_size_dz / 2.0);

	double m_Horn3_x = -1.0 * ((m_HerkuleX3_size_dx / 2.0) + (m_Horn1_h / 2.0));
	double m_Horn3_y = 0.0;
	double m_Horn3_z = (m_HerkuleX3_z + 0.0128);

	double m_Braket03_x = 0.0;
	double m_Braket03_y = 0.0;
	double m_Braket03_z = m_Horn3_z + m_Horn1_r;

	double m_Braket04_size_dx = 0.03;
	double m_Braket04_size_dy = 0.035;
	double m_Braket04_size_dz = 0.012;
	double m_Braket04_x = 0.0;
	double m_Braket04_y = 0.0;
	double m_Braket04_z = m_Braket03_z + (m_Braket02_size_dz/2.0) + (m_Braket04_size_dz/2.0);

	double m_HerkuleX4_size_dx = 0.045;
	double m_HerkuleX4_size_dy = 0.024;
	double m_HerkuleX4_size_dz = 0.029;
	double m_HerkuleX4_x = 0.0097;
	double m_HerkuleX4_y = 0.0;
	double m_HerkuleX4_z = m_Braket04_z + (m_Braket04_size_dz/2.0) + (m_HerkuleX4_size_dz/2.0);

	double m_Horn4_x = 0.0;
	double m_Horn4_y = 0.0;
	double m_Horn4_z = m_HerkuleX4_z + (m_HerkuleX4_size_dz /2.0)+ (m_Horn1_h / 2.0);

	double m_Braket05_size_dx = 0.037;
	double m_Braket05_size_dy = 0.034;
	double m_Braket05_size_dz = 0.005;
	double m_Braket05_x = 0.003;
	double m_Braket05_y = 0.0;
	double m_Braket05_z = m_Horn4_z + (m_Braket05_size_dz/2.0);

	double m_HerkuleX5_size_dx = 0.029;
	double m_HerkuleX5_size_dy = 0.024;
	double m_HerkuleX5_size_dz = 0.045;
	double m_HerkuleX5_x = 0.0;
	double m_HerkuleX5_y = 0.0;
	double m_HerkuleX5_z = m_Braket05_z + (m_HerkuleX5_size_dz / 2.0);

	double m_Horn5_x = -1.0 * ((m_HerkuleX5_size_dx / 2.0) + (m_Horn1_h / 2.0));
	double m_Horn5_y = 0.0;
	double m_Horn5_z = (m_HerkuleX5_z + 0.0128);

	double m_Braket06_x = 0.0;
	double m_Braket06_y = 0.0;
	double m_Braket06_z = m_Horn5_z + m_Horn1_r;

	double m_Braket07_x = 0.0;
	double m_Braket07_y = 0.0;
	double m_Braket07_z = m_Braket06_z + (m_Braket02_size_dz / 2.0) + (m_Braket04_size_dz / 2.0);

	double m_HerkuleX6_x = 0.0097;
	double m_HerkuleX6_y = 0.0;
	double m_HerkuleX6_z = m_Braket07_z + (m_Braket04_size_dz / 2.0) + (m_HerkuleX4_size_dz / 2.0);

	double m_Horn6_x = 0.0;
	double m_Horn6_y = 0.0;
	double m_Horn6_z = m_HerkuleX6_z + (m_HerkuleX4_size_dz / 2.0) + (m_Horn1_h / 2.0);

	double m_Braket08_x = 0.003;
	double m_Braket08_y = 0.0;
	double m_Braket08_z = m_Horn6_z + (m_Braket05_size_dz / 2.0);

	//Base
	_ode->AddBox("BASE", 100, RGB(0, 0, 0), 0.0, m_HerkuleX1_y, 0.0, _DEG2RAD * 0, _DEG2RAD * 0, _DEG2RAD * 0, 0.045, 0.050, 0.010); //BASE
	/********************************************************************************************************************************************************************/
    //1Axis
	_ode->AddBox("HerkuleX-1", 100, RGB(98, 100, 105), m_HerkuleX1_x, m_HerkuleX1_y, m_HerkuleX1_z, 
		_DEG2RAD * 0, _DEG2RAD * 0, _DEG2RAD * 0, 
		m_HerkuleX_size_dx, m_HerkuleX_size_dy, m_HerkuleX_size_dz); //HerkuleX1
	//Horn 1
	_ode->AddCylinder("Horn-1", 100, RGB(213, 213, 211), m_Horn1_x, m_Horn1_y, m_Horn1_z,
		_DEG2RAD * 0, _DEG2RAD * 0, _DEG2RAD * 0, 
		m_Horn1_r, m_Horn1_h); //Horn
	//Braket 1
	_ode->AddBox("Braket-01", 50, RGB(160, 160, 160), m_Braket01_x, m_Braket01_y, m_Braket01_z,
		_DEG2RAD * 0, _DEG2RAD * 0, _DEG2RAD * 0,
		m_Braket01_size_dx, m_Braket01_size_dy, m_Braket01_size_dz); //Braket
	//Braket 1-1
	_ode->AddBox("Braket-011", 100, RGB(10, 10, 10), m_Braket011_x, m_Braket011_y, m_Braket011_z,
		_DEG2RAD * 0, _DEG2RAD * 0, _DEG2RAD * 0,
		m_Braket011_size_dx, m_Braket011_size_dy, m_Braket011_size_dz); //Braket
	/********************************************************************************************************************************************************************/
	 //2Axis
	_ode->AddBox("HerkuleX-2", 100, RGB(98, 100, 105), m_HerkuleX2_x, m_HerkuleX2_y, m_HerkuleX2_z,
		_DEG2RAD * 0, _DEG2RAD * 0, _DEG2RAD * 0,
		m_HerkuleX2_size_dx, m_HerkuleX2_size_dy, m_HerkuleX2_size_dz); //HerkuleX2
	//Horn 2
	_ode->AddCylinder("Horn-2", 100, RGB(213, 213, 211), m_Horn2_x, m_Horn2_y, m_Horn2_z,
		_DEG2RAD * 0, _DEG2RAD * 90, _DEG2RAD * 0,
		m_Horn1_r, m_Horn1_h); //Horn
	//Braket 2
	_ode->AddBox("Braket-02", 50, RGB(160, 160, 160), m_Braket02_x, m_Braket02_y, m_Braket02_z,
		_DEG2RAD * 0, _DEG2RAD * 0, _DEG2RAD * 0, 
		m_Braket02_size_dx, m_Braket02_size_dy, m_Braket02_size_dz); // ㄷ자 braket
	//Braket 1-2
	_ode->AddBox("Braket-012", 50, RGB(10, 10, 10), m_Braket012_x, m_Braket012_y, m_Braket012_z,
		_DEG2RAD * 0, _DEG2RAD * 0, _DEG2RAD * 0,
		m_Braket011_size_dx, m_Braket011_size_dy, m_Braket011_size_dz); //Braket
	/********************************************************************************************************************************************************************/
	//3Axis
	_ode->AddBox("HerkuleX-3", 100, RGB(98, 100, 105), m_HerkuleX3_x, m_HerkuleX3_y, m_HerkuleX3_z,
		_DEG2RAD * 0, _DEG2RAD * 0, _DEG2RAD * 0,
		m_HerkuleX3_size_dx, m_HerkuleX3_size_dy, m_HerkuleX3_size_dz); //HerkuleX3
	//Horn 3
	_ode->AddCylinder("Horn-3", 100, RGB(213, 213, 211), m_Horn3_x, m_Horn3_y, m_Horn3_z,
		_DEG2RAD * 0, _DEG2RAD * 90, _DEG2RAD * 0,
		m_Horn1_r, m_Horn1_h); //Horn
	//Braket 3
	_ode->AddBox("Braket-03", 50, RGB(160, 160, 160), m_Braket03_x, m_Braket03_y, m_Braket03_z,
		_DEG2RAD * 0, _DEG2RAD * 0, _DEG2RAD * 0,
		m_Braket02_size_dx, m_Braket02_size_dy, m_Braket02_size_dz); // ㄷ자 braket
	/********************************************************************************************************************************************************************/
	//4Axis
	_ode->AddBox("Braket-04", 100, RGB(10, 10, 10), m_Braket04_x, m_Braket04_y, m_Braket04_z,
		_DEG2RAD * 0, _DEG2RAD * 0, _DEG2RAD * 0,
		m_Braket04_size_dx, m_Braket04_size_dy, m_Braket04_size_dz); // ㄱ자 braket

	_ode->AddBox("HerkuleX-4", 100, RGB(98, 100, 105), m_HerkuleX4_x, m_HerkuleX4_y, m_HerkuleX4_z,
		_DEG2RAD * 0, _DEG2RAD * 0, _DEG2RAD * 0,
		m_HerkuleX4_size_dx, m_HerkuleX4_size_dy, m_HerkuleX4_size_dz); //HerkuleX4
	//Horn 4
	_ode->AddCylinder("Horn-4", 100, RGB(213, 213, 211), m_Horn4_x, m_Horn4_y, m_Horn4_z,
		_DEG2RAD * 0, _DEG2RAD * 0, _DEG2RAD * 0,
		m_Horn1_r, m_Horn1_h); //Horn
	//Braket 5
	_ode->AddBox("Braket-05", 50, RGB(160, 160, 160), m_Braket05_x, m_Braket05_y, m_Braket05_z,
		_DEG2RAD * 0, _DEG2RAD * 0, _DEG2RAD * 0,
		m_Braket05_size_dx, m_Braket05_size_dy, m_Braket05_size_dz); //Braket
	/********************************************************************************************************************************************************************/
	//5Axis
	_ode->AddBox("HerkuleX-5", 100, RGB(98, 100, 105), m_HerkuleX5_x, m_HerkuleX5_y, m_HerkuleX5_z,
		_DEG2RAD * 0, _DEG2RAD * 0, _DEG2RAD * 0,
		m_HerkuleX5_size_dx, m_HerkuleX5_size_dy, m_HerkuleX5_size_dz); //HerkuleX4
	//Horn 5
	_ode->AddCylinder("Horn-5", 100, RGB(213, 213, 211), m_Horn5_x, m_Horn5_y, m_Horn5_z,
		_DEG2RAD * 0, _DEG2RAD * 90, _DEG2RAD * 0,
		m_Horn1_r, m_Horn1_h); //Horn
	//Braket 6
	_ode->AddBox("Braket-06", 50, RGB(160, 160, 160), m_Braket06_x, m_Braket06_y, m_Braket06_z,
		_DEG2RAD * 0, _DEG2RAD * 0, _DEG2RAD * 0,
		m_Braket02_size_dx, m_Braket02_size_dy, m_Braket02_size_dz); // ㄷ자 braket
	/********************************************************************************************************************************************************************/
	//Axis 6
	_ode->AddBox("Braket-07", 50, RGB(10, 10, 10), m_Braket07_x, m_Braket07_y, m_Braket07_z,
		_DEG2RAD * 0, _DEG2RAD * 0, _DEG2RAD * 0,
		m_Braket04_size_dx, m_Braket04_size_dy, m_Braket04_size_dz); // ㄱ자 braket

	_ode->AddBox("HerkuleX-6", 100, RGB(98, 100, 105), m_HerkuleX6_x, m_HerkuleX6_y, m_HerkuleX6_z,
		_DEG2RAD * 0, _DEG2RAD * 0, _DEG2RAD * 0,
		m_HerkuleX4_size_dx, m_HerkuleX4_size_dy, m_HerkuleX4_size_dz); //HerkuleX6
	//Horn 6
	_ode->AddCylinder("Horn-6", 100, RGB(213, 213, 211), m_Horn6_x, m_Horn6_y, m_Horn6_z,
		_DEG2RAD * 0, _DEG2RAD * 0, _DEG2RAD * 0,
		m_Horn1_r, m_Horn1_h); //Horn
	//Braket 8
	_ode->AddBox("Braket-08", 50, RGB(160, 160, 160), m_Braket08_x, m_Braket08_y, m_Braket08_z,
		_DEG2RAD * 0, _DEG2RAD * 0, _DEG2RAD * 0,
		m_Braket05_size_dx, m_Braket05_size_dy, m_Braket05_size_dz); //Braket


	//Link Assemble
	_ode->SetRigidBody("HerkuleX-1", "BASE");
	_ode->SetRigidBody("Horn-1", "Braket-01");
	_ode->SetRigidBody("Braket-01", "Braket-011");
	_ode->SetRigidBody("Braket-011", "HerkuleX-2");
	_ode->SetRigidBody("Horn-2", "Braket-02");
	_ode->SetRigidBody("Braket-02", "Braket-012");
	_ode->SetRigidBody("Braket-012", "HerkuleX-3");
	_ode->SetRigidBody("Horn-3", "Braket-03");
	_ode->SetRigidBody("Braket-03", "Braket-04");
	_ode->SetRigidBody("Braket-04", "HerkuleX-4");
	_ode->SetRigidBody("Horn-4", "Braket-05");
	_ode->SetRigidBody("Braket-05", "HerkuleX-5");
	_ode->SetRigidBody("Horn-5", "Braket-06");
	_ode->SetRigidBody("Braket-06", "Braket-07");
	_ode->SetRigidBody("Braket-07", "HerkuleX-6");
	_ode->SetRigidBody("Horn-6", "Braket-08");


	//Base...
	_ode->AttachFixedJoint("BASE0", "", "BASE");

	//Arm Joint...
	_ode->AttachBallJoint("JR1", "HerkuleX-1", "Horn-1", 0.0, 0.0, m_Horn1_z, 6000, _DEG2RAD * -180, _DEG2RAD * 180);
	_ode->AttachBallJoint("JR2", "HerkuleX-2", "Horn-2", 0.0, 0.0, m_Horn2_z, 5000, _DEG2RAD * -180, _DEG2RAD * 180);
	_ode->AttachBallJoint("JR3", "HerkuleX-3", "Horn-3", 0.0, 0.0, m_Horn3_z, 4000, _DEG2RAD * -180, _DEG2RAD * 180);
	_ode->AttachBallJoint("JR4", "HerkuleX-4", "Horn-4", 0.0, 0.0, m_Horn4_z, 3000, _DEG2RAD * -180, _DEG2RAD * 180);
	_ode->AttachBallJoint("JR5", "HerkuleX-5", "Horn-5", 0.0, 0.0, m_Horn5_z, 2000, _DEG2RAD * -180, _DEG2RAD * 180);
	_ode->AttachBallJoint("JR6", "HerkuleX-6", "Horn-6", 0.0, 0.0, m_Horn6_z, 1000, _DEG2RAD * -180, _DEG2RAD * 180);
	

	_ogl = new COdeOgl(_ode, "ODE", rect, this, 90000); 
	_ogl->_showAxis = true; 


}
void CSkillMateDlg::OnBnClickedButton4()
{
	UpdateData();

	_ode->SetJointAngle ("JR1", 2, _DEG2RAD* m_dAngle[0]);
	_ode->SetJointAngle ("JR2", 0, _DEG2RAD* m_dAngle[1]);
	_ode->SetJointAngle ("JR3", 0, _DEG2RAD* m_dAngle[2]);
	_ode->SetJointAngle ("JR4", 2, _DEG2RAD* m_dAngle[3]);
	_ode->SetJointAngle ("JR5", 0, _DEG2RAD* m_dAngle[4]);
	_ode->SetJointAngle ("JR6", 2, _DEG2RAD* m_dAngle[5]);

}

void CSkillMateDlg::OnTimer(UINT_PTR nIDEvent)
{ 
	if (nIDEvent == 1)
	{
		_ode->Step(0.01);
		_ogl->UpdateWnd();

		_ode->SetJointAngle("JR1", 2, _DEG2RAD * m_dAngle[0] * -1.0);
		_ode->SetJointAngle("JR2", 0, _DEG2RAD * m_dAngle[1]);
		_ode->SetJointAngle("JR3", 0, _DEG2RAD * m_dAngle[2]);
		_ode->SetJointAngle("JR4", 2, _DEG2RAD * m_dAngle[3] * -1.0);
		_ode->SetJointAngle("JR5", 0, _DEG2RAD * m_dAngle[4]);
		_ode->SetJointAngle("JR6", 2, _DEG2RAD * m_dAngle[5]);
	}

	CDialog::OnTimer(nIDEvent);
}


void CSkillMateDlg::OnBnClickedButton5()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int m_iInfiniteTurn = 0x00;

	//ID 1번에 대한 Setting
	_p_sjog[0].ID = szIDs[0];
	_p_sjog[0].LED = 1;//Green
	_p_sjog[0].NoAction = 0;
	_p_sjog[0].Stop = 0;
	_p_sjog[0].InfiniteTurn = m_iInfiniteTurn;
	_p_sjog[0].Profile = 0;
	_p_sjog[0].Value = 512;

	//ID 2번에 대한 Setting
	_p_sjog[1].ID = szIDs[1];
	_p_sjog[1].LED = 1;//Green
	_p_sjog[1].NoAction = 0;
	_p_sjog[1].Stop = 0;
	_p_sjog[1].InfiniteTurn = m_iInfiniteTurn;
	_p_sjog[1].Profile = 0;
	_p_sjog[1].Value = 512;

	//ID 3번에 대한 Setting
	_p_sjog[2].ID = szIDs[2];
	_p_sjog[2].LED = 1;//Green
	_p_sjog[2].NoAction = 0;
	_p_sjog[2].Stop = 0;
	_p_sjog[2].InfiniteTurn = m_iInfiniteTurn;
	_p_sjog[2].Profile = 0;
	_p_sjog[2].Value = 512;

	//ID 4번에 대한 Setting
	_p_sjog[3].ID = szIDs[3];
	_p_sjog[3].LED = 1;//Green
	_p_sjog[3].NoAction = 0;
	_p_sjog[3].Stop = 0;
	_p_sjog[3].InfiniteTurn = m_iInfiniteTurn;
	_p_sjog[3].Profile = 0;
	_p_sjog[3].Value = 512;

	//ID 5번에 대한 Setting
	_p_sjog[4].ID = szIDs[4];
	_p_sjog[4].LED = 1;//Green
	_p_sjog[4].NoAction = 0;
	_p_sjog[4].Stop = 0;
	_p_sjog[4].InfiniteTurn = m_iInfiniteTurn;
	_p_sjog[4].Profile = 0;
	_p_sjog[4].Value = 512;

	//ID 6번에 대한 Setting
	_p_sjog[5].ID = szIDs[5];
	_p_sjog[5].LED = 1;//Green
	_p_sjog[5].NoAction = 0;
	_p_sjog[5].Stop = 0;
	_p_sjog[5].InfiniteTurn = m_iInfiniteTurn;
	_p_sjog[5].Profile = 0;
	_p_sjog[5].Value = 512;

	//S_JOG에서는 Play Time 하나를 공통으로사용
	// Total Axis는 6개 (ID 1~6)
	S_JOG_MOVE(100, 6, _p_sjog);

}


void CSkillMateDlg::OnBnClickedButton6()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	for (int i = 0; i < 6; i++)
	{
		Herkulex_Servo_Enable(i + 1, 1);
		Sleep(10);
		Herkulex_Set_LED(i + 1, 0, 1, 0);
		Sleep(10);
	}
}


void CSkillMateDlg::OnBnClickedButton7()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	for (int i = 0; i < 6; i++)
	{
		Herkulex_Servo_Enable(i + 1, 0);
		Sleep(10);
		Herkulex_Set_LED(i + 1, 0, 0, 0);
		Sleep(10);
	}
}


void CSkillMateDlg::OnBnClickedButton8()
{
	// Save Pose 1
	for (int i = 0; i < 6; i++)
	{
		m_dSaveAngle1[i] = m_dAngle[i];
	}
}


void CSkillMateDlg::OnBnClickedButton9()
{
	// Save Pose 2
	for (int i = 0; i < 6; i++)
	{
		m_dSaveAngle2[i] = m_dAngle[i];
	}
}


void CSkillMateDlg::OnBnClickedButton10()
{
	// Save Pose 3
	for (int i = 0; i < 6; i++)
	{
		m_dSaveAngle3[i] = m_dAngle[i];
	}
}


void CSkillMateDlg::OnBnClickedButton11()
{
	// Move Pose 1
	for (int i = 0; i < 6; i++)
	{
		_p_sjog[i].ID = szIDs[i];
		_p_sjog[i].LED = 1;//Green
		_p_sjog[i].NoAction = 0;
		_p_sjog[i].Stop = 0;
		_p_sjog[i].InfiniteTurn = 0x00;
		_p_sjog[i].Profile = 0;
		_p_sjog[i].Value = Set_AngleToCount(m_dSaveAngle1[i]);
	}
	
	//S_JOG에서는 Play Time 하나를 공통으로사용
	// Total Axis는 6개 (ID 1~6)
	S_JOG_MOVE(100, 6, _p_sjog);
}


void CSkillMateDlg::OnBnClickedButton12()
{
	// Move Pose 2
	for (int i = 0; i < 6; i++)
	{
		_p_sjog[i].ID = szIDs[i];
		_p_sjog[i].LED = 1;//Green
		_p_sjog[i].NoAction = 0;
		_p_sjog[i].Stop = 0;
		_p_sjog[i].InfiniteTurn = 0x00;
		_p_sjog[i].Profile = 0;
		_p_sjog[i].Value = Set_AngleToCount(m_dSaveAngle2[i]);
	}

	//S_JOG에서는 Play Time 하나를 공통으로사용
	// Total Axis는 6개 (ID 1~6)
	S_JOG_MOVE(100, 6, _p_sjog);
}


void CSkillMateDlg::OnBnClickedButton13()
{
	// Move Pose 3
	for (int i = 0; i < 6; i++)
	{
		_p_sjog[i].ID = szIDs[i];
		_p_sjog[i].LED = 1;//Green
		_p_sjog[i].NoAction = 0;
		_p_sjog[i].Stop = 0;
		_p_sjog[i].InfiniteTurn = 0x00;
		_p_sjog[i].Profile = 0;
		_p_sjog[i].Value = Set_AngleToCount(m_dSaveAngle3[i]);
	}

	//S_JOG에서는 Play Time 하나를 공통으로사용
	// Total Axis는 6개 (ID 1~6)
	S_JOG_MOVE(100, 6, _p_sjog);

}

int CSkillMateDlg::Set_AngleToCount(double dAngle)
{
	int iResult = 0;

	iResult  = (int)(dAngle / 0.325) + 512; // 0101 & 0201

	return iResult;
}