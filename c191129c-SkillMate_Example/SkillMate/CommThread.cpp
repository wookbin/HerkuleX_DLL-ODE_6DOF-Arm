#include "stdafx.h"
#include "CommThread.h"

extern HWND hCommWnd;

void CQueue::Clear()
{
	m_iHead = m_iTail =0;
	memset(buff,0,BUFF_SIZE);
}
CQueue::CQueue()
{
	Clear();
}
int CQueue::GetSize()
{
	return (m_iHead - m_iTail + BUFF_SIZE) % BUFF_SIZE;
}
BOOL CQueue::PutByte(BYTE b)
{
	if(GetSize() == (BUFF_SIZE-1)) return FALSE;
	buff[m_iHead++] =b;
	m_iHead %= BUFF_SIZE;
	return TRUE;
}
BOOL CQueue::GetByte(BYTE *pb)
{
	if(GetSize() == 0) return FALSE;
	*pb = buff[m_iTail++];
	m_iTail %= BUFF_SIZE;
	return TRUE;
}
//--- 클래스 생성자
CCommThread::CCommThread()
{

	//--> 초기는 당연히..포트가 열리지 않은 상태여야겠죠?
	m_bConnected = FALSE;
	m_wPortID = 1;
}

CCommThread::~CCommThread()
{

}


// 포트 sPortName을 dwBaud 속도로 연다.
// ThreadWatchComm 함수에서 포트에 무언가 읽혔을 때 MainWnd에 알리기
// 위해 WM_COMM_READ메시지를 보낼때 같이 보낼 wPortID값을 전달 받는다.
BOOL CCommThread::OpenPort(CString strPortName, 
					   DWORD dwBaud, BYTE byData, BYTE byStop, BYTE byParity )
{

	// Local 변수.
		COMMTIMEOUTS	timeouts;
		DCB				dcb;
		
		// overlapped structure 변수 초기화.
		m_osRead.Offset = 0;
		m_osRead.OffsetHigh = 0;
		//--> Read 이벤트 생성에 실패..
		if ( !(m_osRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL)) ) 	
		{
			return FALSE;
		}
	

		m_osWrite.Offset = 0;
		m_osWrite.OffsetHigh = 0;
		//--> Write 이벤트 생성에 실패..
		if (! (m_osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL)))
		{
			return FALSE;
		}
		
		//--> 포트명 저장..
		m_sPortName = "Com1"; //strPortName;
	
		//--> 실제적인...RS 232 포트 열기..
		m_hComm = CreateFile( m_sPortName, 
							  GENERIC_READ | GENERIC_WRITE, 0, NULL,
							  OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 
							  NULL);

	
		//--> 포트 열기에 실해하면..
		if (m_hComm == (HANDLE) -1)
		{
			AfxMessageBox("fail Port open");
			return FALSE;
		}
	

	//===== 포트 상태 설정. =====

	// EV_RXCHAR event 설정...데이터가 들어오면.. 수신 이벤트가 발생하게끔..
	SetCommMask( m_hComm, EV_RXCHAR);	

	// InQueue, OutQueue 크기 설정.
	SetupComm( m_hComm, BUFF_SIZE, BUFF_SIZE);	

	// 포트 비우기.
	PurgeComm( m_hComm,					
			   PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR);

	// timeout 설정.
	timeouts.ReadIntervalTimeout = 0xFFFFFFFF;
	timeouts.ReadTotalTimeoutMultiplier = 0;
	timeouts.ReadTotalTimeoutConstant = 0;
	
	timeouts.WriteTotalTimeoutMultiplier = 2*CBR_9600 / dwBaud;
	timeouts.WriteTotalTimeoutConstant = 0;
	
	SetCommTimeouts( m_hComm, &timeouts);

	// dcb 설정.... 포트의 실제적인..제어를 담당하는 DCB 구조체값 셋팅..
	dcb.DCBlength = sizeof(DCB);

	//--> 현재 설정된 값 중에서..
	GetCommState( m_hComm, &dcb);	
	
	//--> 보드레이트를 바꾸고..
	dcb.BaudRate = dwBaud;
	

	//--> Data 8 Bit
	dcb.ByteSize = byData;

	//--> Noparity
	dcb.Parity = byParity;

	//--> 1 Stop Bit
	dcb.StopBits = byStop;


	//--> 포트를 재..설정값으로.. 설정해보고..
	if( !SetCommState( m_hComm, &dcb) )	
	{
		return FALSE;
	}


	// 포트 감시 쓰레드 생성.
	m_bConnected = TRUE;

	//--> 포트 감시 쓰레드 생성.
// 	m_hThreadWatchComm = CreateThread( NULL, 0, 
// 									   (LPTHREAD_START_ROUTINE)ThreadWatchComm, 
// 									   this, 0, &dwThreadID);
	AfxBeginThread(ThreadWatchComm,(LPVOID)this);

	//--> 쓰레드 생성에 실패하면..
// 	if (! m_hThreadWatchComm)
// 	{
// 		//--> 열린 포트를 닫고..
// 		AfxMessageBox("failed to create thread");
// 		ClosePort();
// 		return FALSE;
// 	}

	check = FALSE;

	return TRUE;
}

// 포트 wPortNum을 dwBaud 속도로 연다.
// ThreadWatchComm 함수에서 포트에 무언가 읽혔을 때 MainWnd에 알리기
// 위해 WM_COMM_READ메시지를 보낼때 같이 보낼 wPortID값을 전달 받는다.
BOOL CCommThread::OpenPort(WORD wPortID, 
					   DWORD dwBaud, BYTE byData, BYTE byStop, BYTE byParity )
{

	// Local 변수.
	COMMTIMEOUTS	timeouts;
	DCB				dcb;
	//DWORD			dwThreadID;
	
	// overlapped structure 변수 초기화.
	m_osRead.Offset = 0;
	m_osRead.OffsetHigh = 0;
	//--> Read 이벤트 생성에 실패..
	if ( !(m_osRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL)) ) 	
	{
		return FALSE;
	}

	m_osWrite.Offset = 0;
	m_osWrite.OffsetHigh = 0;
	//--> Write 이벤트 생성에 실패..
	if (! (m_osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL)))
	{
		return FALSE;
	}
	
	//--> 포트명 저장..
	m_wPortID = wPortID;
	m_sPortName.Format(_T("Com%d"), m_wPortID);

	//--> 실제적인...RS 232 포트 열기..
	m_hComm = CreateFile( m_sPortName, 
						  GENERIC_READ | GENERIC_WRITE, 0, NULL,
						  OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 
						  NULL);


	//--> 포트 열기에 실해하면..
	if (m_hComm == (HANDLE) -1)
	{
		AfxMessageBox("fail Port ofen");
		return FALSE;
	}


	//===== 포트 상태 설정. =====

	// EV_RXCHAR event 설정...데이터가 들어오면.. 수신 이벤트가 발생하게끔..
	SetCommMask( m_hComm, EV_RXCHAR);	

	// InQueue, OutQueue 크기 설정.
	SetupComm( m_hComm, BUFF_SIZE, BUFF_SIZE);	

	// 포트 비우기.
	PurgeComm( m_hComm,					
			   PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR);

	// timeout 설정.
	timeouts.ReadIntervalTimeout = 0xFFFFFFFF;
	timeouts.ReadTotalTimeoutMultiplier = 0;
	timeouts.ReadTotalTimeoutConstant = 0;
	
	timeouts.WriteTotalTimeoutMultiplier = 2*CBR_9600 / dwBaud;
	timeouts.WriteTotalTimeoutConstant = 0;
	
	SetCommTimeouts( m_hComm, &timeouts);

	// dcb 설정.... 포트의 실제적인..제어를 담당하는 DCB 구조체값 셋팅..
	dcb.DCBlength = sizeof(DCB);

	//--> 현재 설정된 값 중에서..
	GetCommState( m_hComm, &dcb);	
	
	//--> 보드레이트를 바꾸고..
	dcb.BaudRate = dwBaud;
	

	//--> Data 8 Bit
	dcb.ByteSize = byData;

	//--> Noparity
	dcb.Parity = byParity;

	//--> 1 Stop Bit
	dcb.StopBits = byStop;


	//--> 포트를 재..설정값으로.. 설정해보고..
	if( !SetCommState( m_hComm, &dcb) )	
	{
		return FALSE;
	}


	// 포트 감시 쓰레드 생성.
	m_bConnected = TRUE;

	//--> 포트 감시 쓰레드 생성.
// 	m_hThreadWatchComm = CreateThread( NULL, 0, 
// 									   (LPTHREAD_START_ROUTINE)ThreadWatchComm, 
// 									   this, 0, &dwThreadID);
	AfxBeginThread(ThreadWatchComm,(LPVOID)this);

	//--> 쓰레드 생성에 실패하면..
// 	if (! m_hThreadWatchComm)
// 	{
// 		//--> 열린 포트를 닫고..
// 		AfxMessageBox("failed to create thread");
// 		ClosePort();
// 		return FALSE;
// 	}

	check = FALSE;

	return TRUE;
}

// 포트를 닫는다.
void CCommThread::ClosePort()
{
	//--> 연결되지 않았음.
	m_bConnected = FALSE;
	

	//--> 마스크 해제..
	SetCommMask( m_hComm, 0);
	
	//--> 포트 비우기.
	PurgeComm( m_hComm,	PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR);
	
	CloseHandle(m_hComm);
	//--> 핸들 닫기
	
}

// 포트에 pBuff의 내용을 nToWrite만큼 쓴다.
// 실제로 쓰여진 Byte수를 리턴한다.
DWORD CCommThread::WriteComm(BYTE *pBuff, DWORD nToWrite)
{
	DWORD	dwWritten, dwError, dwErrorFlags;
	COMSTAT	comstat;

	//--> 포트가 연결되지 않은 상태이면..
	if( !m_bConnected )		
	{
		return 0;
	}


	//--> 인자로 들어온 버퍼의 내용을 nToWrite 만큼 쓰고.. 쓴 갯수를.,dwWrite 에 넘김.
	if( !WriteFile( m_hComm, pBuff, nToWrite, &dwWritten, &m_osWrite))
	{
		//--> 아직 전송할 문자가 남았을 경우..
		if (GetLastError() == ERROR_IO_PENDING)
		{
			// 읽을 문자가 남아 있거나 전송할 문자가 남아 있을 경우 Overapped IO의
			// 특성에 따라 ERROR_IO_PENDING 에러 메시지가 전달된다.
			//timeouts에 정해준 시간만큼 기다려준다.
			while (! GetOverlappedResult( m_hComm, &m_osWrite, &dwWritten, TRUE))
			{
				dwError = GetLastError();
				if (dwError != ERROR_IO_INCOMPLETE)
				{
					ClearCommError( m_hComm, &dwErrorFlags, &comstat);
					break;
				}
			}
		}
		else
		{
			dwWritten = 0;
			ClearCommError( m_hComm, &dwErrorFlags, &comstat);
		}
	}


	//--> 실제 포트로 쓰여진 갯수를 리턴..
	return dwWritten;
}


// 포트로부터 pBuff에 nToWrite만큼 읽는다.
// 실제로 읽혀진 Byte수를 리턴한다.
DWORD CCommThread::ReadComm(BYTE *pBuff, DWORD nToRead)
{
	DWORD	dwRead,dwError, dwErrorFlags;
	COMSTAT comstat;

	//--- system queue에 도착한 byte수만 미리 읽는다.
	ClearCommError( m_hComm, &dwErrorFlags, &comstat);

	//--> 시스템 큐에서 읽을 거리가 있으면..
	dwRead = comstat.cbInQue;
	if(dwRead > 0)
	{
		//--> 버퍼에 일단 읽어들이는데.. 만일..읽어들인값이 없다면..
		if( !ReadFile( m_hComm, pBuff, nToRead, &dwRead, &m_osRead) )
		{
			//--> 읽을 거리가 남았으면..
			if (GetLastError() == ERROR_IO_PENDING)
			{
				//--------- timeouts에 정해준 시간만큼 기다려준다.
				while (! GetOverlappedResult( m_hComm, &m_osRead, &dwRead, TRUE))
				{
					dwError = GetLastError();
					if (dwError != ERROR_IO_INCOMPLETE)
					{
						ClearCommError( m_hComm, &dwErrorFlags, &comstat);
						break;
					}
				}
			}
			else
			{
				dwRead = 0;
				ClearCommError( m_hComm, &dwErrorFlags, &comstat);
			}
		}
	}


	//--> 실제 읽어들인 갯수를 리턴.
	return dwRead;
}

// 포트를 감시하고, 읽힌 내용이 있으면
// m_ReadData에 저장한 뒤에 MainWnd에 메시지를 보내어 Buffer의 내용을
// 읽어가라고 신고한다.
//DWORD	ThreadWatchComm(CCommThread* pComm)
UINT CCommThread::ThreadWatchComm(LPVOID param)//(CCommThread* pComm))
{
	CCommThread* pComm = (CCommThread*) param;

	DWORD           dwEvent;
	OVERLAPPED      os;
	BOOL            bOk = TRUE;
	BYTE			buff[2048];	// 읽기 버퍼
	//BYTE			buff2[2048];	// 읽기 버퍼
	DWORD           dwRead;		// 읽은 바이트수.
	BYTE			*pDumpBin = NULL;
	BOOL			bSetflag = FALSE;

	// Event, OS 설정.
	memset(&os, 0, sizeof(OVERLAPPED));
	
	// 이벤트 설정..
	if (!(os.hEvent = CreateEvent( NULL, TRUE, FALSE, NULL)))
		bOk = FALSE;
	
	// 이벤트 마스크..
	if (!SetCommMask(pComm->m_hComm, EV_RXCHAR))
		bOk = FALSE;
	
	//--> 이벤트나..마스크 설정에 실패함..
	if( !bOk )
	{
		AfxMessageBox("Error while creating ThreadWatchComm, " + pComm->m_sPortName);
		return FALSE;
	}
	
	while (pComm ->m_bConnected)//포트가 연결되면 무한 루프에 들어감
	{
		dwEvent = 0;
		DWORD wParam = 0;
		
        // 포트에 읽을 거리가 올때까지 기다린다.
        WaitCommEvent( pComm->m_hComm, &dwEvent, NULL);
		
		//--> 데이터가 수신되었다는 메세지가 발생하면..
        if ((dwEvent & EV_RXCHAR) == EV_RXCHAR)
        {
			do
			{
				dwRead = pComm->ReadComm( buff, 2048); //들어온 데이터 읽어 오기 
				if(BUFF_SIZE - pComm->m_QueueRead.GetSize() > (int)dwRead)
				{
					for( WORD i = 0; i < dwRead; i++ )
					{
						pComm->m_QueueRead.PutByte(buff[i]);//큐 버퍼에  들어온 데이터 넣기 
					}
				}
				else
				{
					pComm->m_QueueRead.Clear();
					//AfxMessageBox("buff full"); //큐버퍼의 크기를 초과하면 경고 메시지 보냄
					TRACE("Buffer FULL");
				}

			/*
			do
			{
				dwRead = pComm->ReadComm( buff, 2048); //들어온 데이터 읽어 오기 
				
				if(BUFF_SIZE - pComm->m_QueueRead.GetSize() > (int)dwRead)
				{
					//bSetflag = FALSE;

					for( WORD i = 0; i < dwRead; i++ )
					{
						pComm->m_QueueRead.PutByte(buff[i]);//큐 버퍼에  들어온 데이터 넣기 
						
					}
				}
				else
				{
					pComm->m_QueueRead.Clear();
					pComm->m_wPortID = 0;
					break;
					//AfxMessageBox("buff full"); //큐버퍼의 크기를 초과하면 경고 메시지 보냄
					//pComm->m_QueueRead.Clear();
					for( WORD i = 0; i < dwRead; i++ )
					{
						pComm->m_QueueRead.PutByte(buff[i]);//큐 버퍼에  들어온 데이터 넣기 
						
					}
					break;
					
				}
				
				//to do....
			*/
					
			}while(dwRead);
			::PostMessage(hCommWnd, WM_COMM_READ, pComm->m_wPortID,0 );//CSerialComDlg로 데이터가 들어왔다는 메시지를 보냄
		}
		Sleep(10);	// 받은 데이터를 화면에 보여줄 시간을 벌기 위해.
					// 데이터를 연속으로 받으면 cpu점유율이 100%가 되어 화면에 뿌려주는 작업이 잘 안되고. 결과적으로 
					// 큐 버퍼에 데이터가 쌓이게 됨
	}
	
	CloseHandle( os.hEvent);
	
	//--> 쓰레드 종료가 되겠죠?
	pComm->m_hThreadWatchComm = NULL;
	
	return TRUE;
}
