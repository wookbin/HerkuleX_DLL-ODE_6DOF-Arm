//	��� Ŭ����	CCommThread 

#ifndef	__NT_COMMTHREAD_H__
#define	__NT_COMMTHREAD_H__

#define WM_COMM_READ	(WM_USER + 500)
#define BUFF_SIZE		4192

// ��Ʈ���� �б� :
//   ��Ʈ�� �� �Ŀ� ��Ʈ�� �ڷᰡ �����ϸ� WM_COMM_READ �޽����� ���� 
//   �����쿡 ���޵ȴ�. ON_MESSAGE ��ũ�θ� �̿�, �Լ��� �����ϰ�
//   m_ReadData String�� ����� �����͸� �̿� �б�
// ��Ʈ�� ���� : WriteComm(buff, 30)�� ���� ���ۿ� �� ũ�⸦ �ǳ׸� �ȴ�.
class CQueue
{
public:
	BYTE buff[BUFF_SIZE];//ť ���� ������� 4192�� ���� 
	int m_iHead, m_iTail;//ť ���ۿ� �����͸� �ְ� �� �ö� ����� ���� 
	CQueue();
	void Clear();//���۸� �ʱ�ȭ ��Ŵ
	int GetSize();//���� ���ۿ� ����ִ� �������� size�� ����
	BOOL PutByte(BYTE b);//ť���ۿ� 1����Ʈ�� ����
	BOOL GetByte(BYTE *pb);//ť���ۿ��� 1����Ʈ�� �� ��
};

class	CCommThread
{
public:
	CCommThread();
	~CCommThread();

// Member Variables
public:
	BOOL        check;
	HANDLE		m_hComm;				// ��� ��Ʈ ���� �ڵ�
	CString		m_sPortName;			// ��Ʈ �̸� (COM1 ..)
	BOOL		m_bConnected;			// ��Ʈ�� ���ȴ��� ������ ��Ÿ��.
	OVERLAPPED	m_osRead, m_osWrite;	// ��Ʈ ���� Overlapped structure
	HANDLE		m_hThreadWatchComm;		// Watch�Լ� Thread �ڵ�.
	WORD		m_wPortID;				// WM_COMM_READ�� �Բ� ������ �μ�.
	CQueue      m_QueueRead;			//ť����
	//BYTE		m_Newbuff[BUFF_SIZE];		//New ���� ������� 4192�� ����
	//int			m_iHead2, m_iTail2;//new ���ۿ� �����͸� �ְ� �� �ö� ����� ���� 

// Operations
public:
	BOOL	OpenPort(CString strPortName, 
					   DWORD dwBaud, BYTE byData, BYTE byStop, BYTE byParity );//��Ʈ ���� 
	BOOL	OpenPort(WORD wPortID, 
					   DWORD dwBaud, BYTE byData, BYTE byStop, BYTE byParity );
	void	ClosePort();				//��Ʈ �ݱ�
	DWORD	WriteComm(BYTE *pBuff, DWORD nToWrite);//��Ʈ�� ������ ����

	DWORD	ReadComm(BYTE *pBuff, DWORD nToRead);//��Ʈ���� ������ �о����
	static UINT ThreadWatchComm(LPVOID param);//(CCommThread* pComm);
	//BOOL	GetByte2(BYTE *pb);//ť���ۿ��� 1����Ʈ�� �� ��
	//BOOL	PutByte2(BYTE b);

};

// Watch Comm. Thread Function 
//DWORD	ThreadWatchComm(CCommThread* pComm);

#endif	__NT_COMMTHREAD_H__