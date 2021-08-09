
#include "dll.h"

bool CONNET_PORT::OpenPort(int port)
{
    CString portname = _T("");
    portname.Format(_T("\\\\.\\COM%d"), port);
    hComm = CreateFile(portname,
        GENERIC_READ | GENERIC_WRITE,
        0,
        0,
        OPEN_EXISTING,
        FILE_FLAG_OVERLAPPED,
        0);
    if (hComm == INVALID_HANDLE_VALUE)
        return FALSE;
    else
        return true;
}

bool CONNET_PORT::SetupDCB(int rate_arg)
{
    DCB dcb;
    memset(&dcb, 0, sizeof(dcb));
    if (!GetCommState(hComm, &dcb)) //Gets the current DCB configuration 
    {
        return false;
    }
    dcb.DCBlength = sizeof(dcb);
    /* ---------- Serial Port Config ------- */
    dcb.BaudRate = rate_arg;
    dcb.Parity = 0;
    dcb.fParity = 0;
    dcb.StopBits = ONESTOPBIT;
    dcb.ByteSize = 8;
    dcb.fOutxCtsFlow = 0;
    dcb.fOutxDsrFlow = 0;
    dcb.fDtrControl = DTR_CONTROL_DISABLE;
    dcb.fDsrSensitivity = 0;
    dcb.fRtsControl = RTS_CONTROL_DISABLE;
    dcb.fOutX = 0;
    dcb.fInX = 0;
    dcb.fErrorChar = 0;
    dcb.fBinary = 1;
    dcb.fNull = 0;
    dcb.fAbortOnError = 0;
    dcb.wReserved = 0;
    dcb.XonLim = 2;
    dcb.XoffLim = 4;
    dcb.XonChar = 0x13;
    dcb.XoffChar = 0x19;
    dcb.EvtChar = 0;
    if (!SetCommState(hComm, &dcb)) //Set the current DCB configuration
    {
        return false;
    }
    else
        return true;
}

bool CONNET_PORT::SetupTimeout(DWORD ReadInterval, DWORD ReadTotalMultiplier, DWORD
    ReadTotalConstant, DWORD WriteTotalMultiplier, DWORD WriteTotalConstant)
{
    COMMTIMEOUTS timeouts;
    timeouts.ReadIntervalTimeout = ReadInterval;
    timeouts.ReadTotalTimeoutConstant = ReadTotalConstant;
    timeouts.ReadTotalTimeoutMultiplier = ReadTotalMultiplier;
    timeouts.WriteTotalTimeoutConstant = WriteTotalConstant;
    timeouts.WriteTotalTimeoutMultiplier = WriteTotalMultiplier;
    if (!SetCommTimeouts(hComm, &timeouts))
    {
        return false;
    }
    else
        return true;
}

string CONNET_PORT::ReciveChar()
{
    BOOL bResult = TRUE;
    DWORD dwError = 0;
    DWORD BytesRead = 0;
    char RXBuff;
    string rec_str = "";
    DWORD  dwBegin, dwEnd, dwtime;
#pragma comment(lib, "winmm.lib")
    dwBegin = timeGetTime();
    int i = 0;

    for (;;)
    {
        dwEnd = timeGetTime();
        bResult = ClearCommError(hComm, &dwError, &Comstat);
        dwtime = dwEnd - dwBegin;
        if (dwtime > 500)
            break;
        if (Comstat.cbInQue == 0)
            continue;
        bResult = ReadFile(hComm,  //Handle to the serial port, which is returned by CreateFile()  
            &RXBuff,  //Point to receive buffer  
            1,  //Read one byte 
            &BytesRead,   //Stores number of bytes read 
            &OverLapped);  //pointer to the m_ov structure

        rec_str += RXBuff;

    }
    return rec_str;
}

bool CONNET_PORT::WriteChar(const char* szWriteBuffer, DWORD dwSend)
{
    BOOL bWrite = TRUE;
    BOOL bResult = TRUE;
    DWORD BytesSent = 0;
    HANDLE hWriteEvent = NULL;
    ResetEvent(hWriteEvent);
    if (bWrite)
    {
        OverLapped.Offset = 0;
        OverLapped.OffsetHigh = 0;
        bResult = WriteFile(hComm,  //Handle to the serial port, which is returned by CreateFile() 
            szWriteBuffer,  //Pointer to message buffer in calling finction  
            dwSend,  //Length of message to send  
            &BytesSent,  //Where to store the number of bytes sent  
            &OverLapped);  //Overlappedstructure
        if (!bResult)
        {
            DWORD dwError = GetLastError();
            switch (dwError)
            {
            case ERROR_IO_PENDING:
                BytesSent = 0;
                bWrite = FALSE;
                break;
            default:
                break;
            }
        }
    }
    if (!bWrite)
    {
        bWrite = TRUE;
        bResult = GetOverlappedResult(hComm,
            &OverLapped,
            &BytesSent,
            TRUE);
        if (!bResult)
        {
            cout << "GetOverlappedResults() in WriteFile()" << endl;
        }
    }
    if (BytesSent != dwSend)
    {
        cout << "ERROR: WriteFile() error.. Bytes Sent:" << BytesSent << "; Message Length: " << strlen((char*)szWriteBuffer) << endl;
        return false;
    }
    return true;
}
