#pragma once
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>  
#include <atlstr.h> 
#include <iostream>   
#include <Mmsystem.h>
#include "winsock.h"
#pragma comment(lib,"ws2_32.lib")


using namespace std;

class CONNET_PORT {
public:
	HANDLE hComm;
	OVERLAPPED OverLapped;
	COMSTAT Comstat;
	bool OpenPort(int port);  //poen serial port 
	bool SetupDCB(int rate_arg);  //set DCB  
	bool SetupTimeout(DWORD ReadInterval, DWORD ReadTotalMultiplier, DWORD
		ReadTotalConstant, DWORD WriteTotalMultiplier, DWORD WriteTotalConstant);   //set timeout  
	string ReciveChar();   //recive seial data 
	bool WriteChar(const char* szWriteBuffer, DWORD dwSend);  //send string to seial  
};


string TestCommandResult(SOCKET sockclient, char* cmd);

string TestCommand(const char* cmd);

extern "C" {
	//_declspec(dllexport) const char* FwVersion();
	_declspec(dllexport) int Connect(const char* IPAdrress, int port);
	//_declspec(dllexport) int Connect();
	_declspec(dllexport) int DisConnect();
	_declspec(dllexport) int TypeCTestTop(int usbVer);
	_declspec(dllexport) int TypeCTestBot(int usbVer);
	_declspec(dllexport) int DisplayPortTopTest(const char* linkRate, int laneCount, double* resolution_X, double* resolution_Y, double* frameRate);
	_declspec(dllexport) int DisplayPortBotTest(const char* linkRate, int laneCount, double* resolution_X, double* resolution_Y, double* frameRate);
	_declspec(dllexport) int PDSinkVBusTestTop(int eLoadCurrent, int* current, int* voltage, int delay_ms);
	_declspec(dllexport) int PDSinkVBusTestBot(int eLoadCurrent, int* current, int* voltage, int delay_ms);
	_declspec(dllexport) int TypeCPinTest(int Pin_Flag, char* test_value);
	_declspec(dllexport) int EarphoneSuspend();
	_declspec(dllexport) int EarphoneInsertTestTop();
	_declspec(dllexport) int EarphoneInsertTestBot();
	_declspec(dllexport) int EarphonePressKeyTest(int pressTime);

	_declspec(dllexport) int HelloWorldPrintf();

	_declspec(dllexport) bool connet_port(int port); //port: serial port
	_declspec(dllexport) const char* fw_version();
	_declspec(dllexport) int comTypeCTestTop(int usbVer, char* usb_debug_info);
	_declspec(dllexport) int comTypeCTestBot(int usbVer, char* usb_debug_info);
	_declspec(dllexport) int comDisplayPortTopTest(const char* linkRate, int laneCount, double* resolution_X, double* resolution_Y, double* frameRate, char* dp_debug_info);
	_declspec(dllexport) int comDisplayPortBotTest(const char* linkRate, int laneCount, double* resolution_X, double* resolution_Y, double* frameRate, char* dp_debug_info);
	_declspec(dllexport) int comPDSinkVBusTestTop(int eLoadCurrent, int* current, int* voltage, int delay_ms);
	_declspec(dllexport) int comPDSinkVBusTestBot(int eLoadCurrent, int* current, int* voltage, int delay_ms);
	_declspec(dllexport) int comTypeCPinTest(int Pin_Flag, char* test_value);
	_declspec(dllexport) int comEarphoneSuspend();
	_declspec(dllexport) int comEarphoneInsertTestTop();
	_declspec(dllexport) int comEarphoneInsertTestBot();
	_declspec(dllexport) int comEarphonePressKeyTest(int pressTime);
}