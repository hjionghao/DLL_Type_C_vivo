#include "dll.h"


//SOCKET sockclient = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
SOCKET sockclient;

/*
		/// <summary>
		/// TypeC测试仪建立连接。
		/// </summary>
		/// <param name="IPAdrress">IP地址</param>
		/// <param name="port">端口号</param>
		/// <returns>0：建立连接成功，-1：连接失败</returns>
		public int Connect(string IPAdrress, int port);
*/
_declspec(dllexport) int Connect(const char* IPAdrress, int port)
//_declspec(dllexport) int Connect()
{
	//初始化
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	//创建socket
	//SOCKET sockclient = socket(AF_INET, SOCK_STREAM, 0);
	sockclient = socket(AF_INET, SOCK_STREAM, 0);
	//cout << sockclient << endl;
	if (INVALID_SOCKET == sockclient)
	{
		//cout << "INVALID_SOCKET" << strerror(errno) << endl;
		return -1;
	}
	//连接服务器，建立服务器端套接字地址
	struct sockaddr_in addr = { 0 };
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	//对于inet_addr()函数，它是把“xxx.xxx.xxx.xxx”形式表示的IPV4地址，转换为IN_ADDR结构体能够
	//接收的形式（unsigned long型，因为IN_ADDR结构体中的负责接收的S_addr成员变量的类型是unsigned long型）
	//const char* ipaddr = IPAdrress;

	//cout << IPAdrress << endl;
	//cout << port << endl;
	
	//const char* ipaddr = IPAdrress.c_str();
	addr.sin_addr.s_addr = inet_addr(IPAdrress);//本机ip
	//addr.sin_addr.s_addr = inet_addr("169.254.1.32");//本机ip


	//向服务器发出连接请求，当然我们也可以通过connet函数的返回值判断到底有无连接成功。
	int iRetVal = connect(sockclient, (struct sockaddr*)&addr, sizeof(addr));
	if (SOCKET_ERROR == iRetVal)
	{
		cout << "服务器连接失败！" << endl;
		closesocket(sockclient);
		return -1;
	}
	
	cout << "服务器连接成功！\n" << endl;
	return 0;
	//return sockclient;
}

/*
		/// <summary>
		/// TypeC测试仪断开连接。
		/// </summary>
		/// <returns>0：成功，-1：失败</returns>
		public int DisConnect();
*/
_declspec(dllexport) int DisConnect()
{
	int ret_socket = closesocket(sockclient);
	//cout << ret_socket << endl;
	if (0 == ret_socket)
	{
		cout << "服务器断开成功！\n" << endl;
		return 0;
	}
	else
		return -1;
}

string TestCommandResult(SOCKET sockclient, const char* cmd)
{
	char recvbuf[4096];
	if (send(sockclient, cmd, strlen(cmd), 0) < 0) {
		return "TestCommandResult fail";
	}
	memset(recvbuf, 0, 4096);
	recv(sockclient, recvbuf, 4096, 0);
	return recvbuf;
}

/*
_declspec(dllexport) const char* FwVersion()
{
	string version_msg;
	version_msg = TestCommandResult(sockclient, "[1]version(255)\r\n");
	const char* ret = version_msg.c_str();
	return ret;
}
*/

/*
		/// <summary>
		/// 正插测试
		/// 函数接口返回值，当测试PASS,返回0；当测试失败时，返回非零的错误码。
		/// 需要提供所有错误码对应的中、英文的错误描述信息对照表。
		/// </summary>
		/// <param name="usbVer">指定测试的内容：1:PIN test; 2:USB2.0 Test; 3:USB3.0</param>
		/// <returns>0：Pass; 非0：失败</returns>
		public int TypeCTestTop(TypeC_Flag flag);
*/

_declspec(dllexport) int TypeCTestTop(int usbVer)
{
	// 判断入口参数；


	int pin_test_flag = 0;

	if (1 == usbVer)
	{
		string ret_GND = TestCommandResult(sockclient, "[1]pin test(GND)\r\n");
		int ret_find = ret_GND.find("PASS");
		if (ret_find == string::npos)
			return -10;
		else
			pin_test_flag += 1;

		string ret_GND_selftest = TestCommandResult(sockclient, "[2]pin test(VBUS)\r\n");
		ret_find = ret_GND_selftest.find("PASS");
		if (ret_find == string::npos)
			return -11;
		else
			pin_test_flag += 1;

		if (2 == pin_test_flag)
			return 0;
	}
	else if (2 == usbVer)
	{
		char cmd[100]{};
		sprintf_s(cmd, 100, "[3]otg usb2_0 test(top)\r\n");
		const char* real_cmd = cmd;
		string ret_usb2_0_top = TestCommandResult(sockclient, real_cmd);
		int ret_find = ret_usb2_0_top.find("PASS");
		if (ret_find == string::npos)
			return -12;
		else
			return 0;
	}
	else if (3 == usbVer)
	{
		char cmd[100]{};
		sprintf_s(cmd, 100, "[4]otg usb3_0 test(top)\r\n");
		const char* real_cmd = cmd;
		string ret_usb3_0_top = TestCommandResult(sockclient, real_cmd);
		int ret_find = ret_usb3_0_top.find("PASS");
		if (ret_find == string::npos)
			return -13;
		else
			return 0;
	}
	else
	{
		return -2;
	}
}

/*
		/// <summary>
		/// 反插测试
		/// 函数接口返回值，当测试PASS,返回0；当测试失败时，返回非零的错误码。
		/// 需要提供所有错误码对应的中、英文的错误描述信息对照表。
		/// </summary>
		/// <param name="usbVer">指定测试的内容：1:PIN test; 2:USB2.0 Test; 3:USB3.0</param>
		/// <returns>0：Pass; 非0：失败</returns>
		public int TypeCTestBot(TypeC_Flag flag);
*/
_declspec(dllexport) int TypeCTestBot(int usbVer)
{
	// 判断入口参数；


	int pin_test_flag = 0;

	if (1 == usbVer)
	{
		string ret_GND = TestCommandResult(sockclient, "[5]pin test(GND)\r\n");
		int ret_find = ret_GND.find("PASS");
		if (ret_find == string::npos)
			return -10;
		else
			pin_test_flag += 1;

		string ret_GND_selftest = TestCommandResult(sockclient, "[6]pin test(VBUS)\r\n");
		ret_find = ret_GND_selftest.find("PASS");
		if (ret_find == string::npos)
			return -11;
		else
			pin_test_flag += 1;

		if (2 == pin_test_flag)
			return 0;
	}
	else if (2 == usbVer)
	{
		char cmd[100]{};
		sprintf_s(cmd, 100, "[7]otg usb2_0 test(bot)\r\n");
		const char* real_cmd = cmd;
		string ret_usb2_0_bot = TestCommandResult(sockclient, real_cmd);
		int ret_find = ret_usb2_0_bot.find("PASS");
		if (ret_find == string::npos)
			return -14;
		else
			return 0;
	}
	else if (3 == usbVer)
	{
		char cmd[100]{};
		sprintf_s(cmd, 100, "[8]otg usb3_0 test(bot)\r\n");
		const char* real_cmd = cmd;
		string ret_usb3_0_bot = TestCommandResult(sockclient, real_cmd);
		int ret_find = ret_usb3_0_bot.find("PASS");
		if (ret_find == string::npos)
			return -15;
		else
			return 0;
	}
	else
	{
		return -2;
	}
}

/*
		/// <summary>
		/// DisplayPort正插测试
		/// 函数接口返回值，当测试PASS,返回0；当测试失败时，返回非零的错误码。
		/// 需要提供所有错误码对应的中、英文的错误描述信息对照表。
		/// </summary>
		/// <param name="linkRate">LinkRate</param>
		/// <param name="laneCount">LaneCount</param>
		/// <sparam name="resolution_X">输出视频最大分辨率的水平分辨率值</param>
		/// <param name="resolution_Y">输出视频最大分辨率的垂直分辨率值</param>
		/// <param name="frameRate">输出视频最大帧率值</param>
		/// <returns>0：Pass; 非0：失败</returns>
		public int DisplayPortTopTest(LinkeRate linkRate,int laneCount, out double resolution_X, out double resolution_Y, out double frameRate);
*/
_declspec(dllexport) int DisplayPortTopTest(const char* linkRate, int laneCount, double* resolution_X, double* resolution_Y, double* frameRate)
{
	// 入口参数判断


	char cmd[100]{};
	//const char* linkrate = linkRate.c_str();

	sprintf_s(cmd, 100, "[9]displayport test(top,%s, %d)\r\n", linkRate, laneCount);
	//cout << cmd << endl;

	const char* real_cmd = cmd;
	string ret_dp_bot = TestCommandResult(sockclient, real_cmd);
	//cout << ret_dp_bot << endl;
	int ret_find = ret_dp_bot.find("PASS");
	if (ret_find == string::npos)
		return -16;
	else
	{
		char* data;
		int len = ret_dp_bot.length();
		data = (char*)malloc((len + 1) * sizeof(char));
		ret_dp_bot.copy(data, len, 0);

		const char* separator = "\"*fps";
		char* ret_strtok;
		char* buf;
		ret_strtok = strtok_s(data, separator, &buf);
		int index = 0;
		while (ret_strtok)
		{
			if (4 == index)
			{
				*resolution_X = atof(ret_strtok);
			}
			if (5 == index)
			{
				*resolution_Y = atof(ret_strtok);
			}
			if (10 == index)
			{
				*frameRate = atof(ret_strtok);
			}
			//p = strtok(NULL, d);
			ret_strtok = strtok_s(NULL, separator, &buf);
			index += 1;
		}
		free(data);
	}
	return 0;
}


/*
		/// <summary>
		/// DisplayPort反插测试
		/// 函数接口返回值，当测试PASS,返回0；当测试失败时，返回非零的错误码。
		/// 需要提供所有错误码对应的中、英文的错误描述信息对照表。
		/// </summary>
		/// <param name="linkRate">LinkRate</param>
		/// <param name="laneCount">LaneCount</param>
		/// <param name="resolution_X">输出视频最大分辨率的水平分辨率值</param>
		/// <param name="resolution_Y">输出视频最大分辨率的垂直分辨率值</param>
		/// <param name="frameRate">输出视频最大帧率值</param>
		/// <returns>0：Pass; 非0：失败</returns>
		public int DisplayPortBotTest(LinkeRate linkRate, int laneCount, out double resolution_X, out double resolution_Y, out double frameRate);
*/
_declspec(dllexport) int DisplayPortBotTest(const char* linkRate, int laneCount, double* resolution_X, double* resolution_Y, double* frameRate)
{
	// 入口参数判断


	char cmd[100]{};
	//const char* linkrate = linkRate.c_str();

	sprintf_s(cmd, 100, "[9]displayport test(bot,%s, %d)\r\n", linkRate, laneCount);
	//cout << cmd << endl;

	const char* real_cmd = cmd;
	string ret_dp_bot = TestCommandResult(sockclient, real_cmd);
	//cout << ret_dp_bot << endl;
	int ret_find = ret_dp_bot.find("PASS");
	if (ret_find == string::npos)
		return -17;
	else
	{
		char* data;
		int len = ret_dp_bot.length();
		data = (char*)malloc((len + 1) * sizeof(char));
		ret_dp_bot.copy(data, len, 0);

		const char* separator = "\"*fps";
		char* ret_strtok;
		char* buf;
		ret_strtok = strtok_s(data, separator, &buf);
		int index = 0;
		while (ret_strtok)
		{
			if (4 == index)
			{
				*resolution_X = atof(ret_strtok);
			}
			if (5 == index)
			{
				*resolution_Y = atof(ret_strtok);
			}
			if (10 == index)
			{
				*frameRate = atof(ret_strtok);
			}
			//p = strtok(NULL, d);
			ret_strtok = strtok_s(NULL, separator, &buf);
			index += 1;
		}
		free(data);
	}
	return 0;
}


/// <summary>
/// PDSink VBUS 正插测试输出电压电流测试
/// </summary>
/// <param name="eLoadCurrent">指定测试目标输出电流</param>
/// <param name="current">实际测试输出电流</param>
/// <param name="voltage">实际测试输出电压</param>
/// <param name="delay_ms">延时时间</param>
/// <returns>0：Pass; 非0：失败</returns>
_declspec(dllexport) int PDSinkVBusTestTop(int eLoadCurrent, int* current, int* voltage, int delay_ms)
{
	// 入口参数判断


	char cmd[100]{};
	//const char* linkrate = linkRate.c_str();

	sprintf_s(cmd, 100, "[9]pd sink test(vbus, top, %d, %d)\r\n", eLoadCurrent, delay_ms);
	//cout << cmd << endl;

	const char* real_cmd = cmd;
	string ret_pd_sink_bot = TestCommandResult(sockclient, real_cmd);
	//cout << ret_pd_sink_bot << endl;

	int ret_find = ret_pd_sink_bot.find("PASS");
	if (ret_find == string::npos)
		return -18;
	else
	{
		char* data;
		int len = ret_pd_sink_bot.length();
		data = (char*)malloc((len + 1) * sizeof(char));
		ret_pd_sink_bot.copy(data, len, 0);

		const char* separator = ":\ \"m";
		char* ret_strtok;
		char* buf;
		ret_strtok = strtok_s(data, separator, &buf);

		int index = 0;
		while (ret_strtok)
		{
			//cout << ret_strtok << endl;
			//cout << index << endl;
			if (2 == index)
			{
 				*current = atof(ret_strtok);
				//cout << *current << endl;
			}
			if (6 == index)
			{
				*voltage = atof(ret_strtok);
				//cout << *voltage << endl;
			}

			//p = strtok(NULL, d);
			ret_strtok = strtok_s(NULL, separator, &buf);
			index += 1;
		}
		free(data);
	}
	return 0;
}

/// <summary>
/// PDSink VBUS 反插测试输出电压电流测试
/// </summary>
/// <param name="eLoadCurrent">指定测试目标输出电流</param>
/// <param name="current">实际测试输出电流</param>
/// <param name="voltage">实际测试输出电压</param>
/// <param name="delay_ms">延时时间</param>
/// <returns>0：Pass; 非0：失败</returns>
_declspec(dllexport) int PDSinkVBusTestBot(int eLoadCurrent, int* current, int* voltage, int delay_ms)
{
	// 入口参数判断


	char cmd[100]{};
	//const char* linkrate = linkRate.c_str();

	sprintf_s(cmd, 100, "[9]pd sink test(vbus, bot, %d, %d)\r\n", eLoadCurrent, delay_ms);
	//cout << cmd << endl;

	const char* real_cmd = cmd;
	string ret_pd_sink_bot = TestCommandResult(sockclient, real_cmd);
	//cout << ret_pd_sink_bot << endl;

	int ret_find = ret_pd_sink_bot.find("PASS");
	if (ret_find == string::npos)
		return -19;
	else
	{
		char* data;
		int len = ret_pd_sink_bot.length();
		data = (char*)malloc((len + 1) * sizeof(char));
		ret_pd_sink_bot.copy(data, len, 0);

		const char* separator = ":\ \"m";
		char* ret_strtok;
		char* buf;
		ret_strtok = strtok_s(data, separator, &buf);

		int index = 0;
		while (ret_strtok)
		{
			//cout << index << endl;
			if (2 == index)
			{
				*current = atof(ret_strtok);
				//cout << *current << endl;
			}
			if (6 == index)
			{
				*voltage = atof(ret_strtok);
				//cout << *voltage << endl;
			}

			//p = strtok(NULL, d);
			ret_strtok = strtok_s(NULL, separator, &buf);
			index += 1;
		}
		free(data);
	}
	return 0;
}

///// <summary>
///// USB PIN测试
///// 函数接口返回值，当测试PASS,返回0；当测试失败时，返回非零的错误码。
///// 需要提供所有错误码对应的中、英文的错误描述信息对照表。
///// </summary>
///// <param name="Pin_Flag">指定测试的内容：0:GND 1:VBUS</param>
///// <returns>0：Pass; 非0：失败</returns>
//_declspec(dllexport) int TypeCPinTest(int Pin_Flag)
//{
//	if (0 == Pin_Flag)
//	{
//		string ret_GND = TestCommandResult(sockclient, "[1]pin test(GND)\r\n");
//		int ret_find = ret_GND.find("PASS");
//		if (ret_find == string::npos)
//			return -10;
//	}
//	else if (1 == Pin_Flag)
//	{
//		string ret_VBUS = TestCommandResult(sockclient, "[2]pin test(VBUS)\r\n");
//		int ret_find = ret_VBUS.find("PASS");
//		if (ret_find == string::npos)
//			return -11;
//	}
//	else
//	{
//		//参数错误
//		return -2;
//	}
//
//	return 0;
//}

/// <summary>
/// USB PIN测试
/// 函数接口返回值，当测试PASS,返回0；当测试失败时，返回非零的错误码。
/// 需要提供所有错误码对应的中、英文的错误描述信息对照表。
/// </summary>
/// <param name="Pin_Flag">指定测试的内容：0:GND 1:VBUS</param>
/// <returns>0：Pass; 非0：失败</returns>
_declspec(dllexport) int TypeCPinTest(int Pin_Flag, char* test_value)
{
	char ret_value[100] = "";

	if (0 == Pin_Flag)
	{
		string ret_GND = TestCommandResult(sockclient, "[1]pin test(GND)\r\n");
		int ret_find = ret_GND.find("PASS");
		if (ret_find == string::npos)
			return -10;
	}
	else if (1 == Pin_Flag)
	{
		string ret_VBUS = TestCommandResult(sockclient, "[2]pin test(VBUS)\r\n");
		//cout << ret_GND << endl;
		int ret_find = ret_VBUS.find("PASS");
		if (ret_find == string::npos)
			return -11;
		else
		{
			char* data;
			int len = ret_VBUS.length();
			data = (char*)malloc((len + 1) * sizeof(char));
			ret_VBUS.copy(data, len, 0);

			const char* separator = ";";
			char* ret_strtok;
			char* buf;
			ret_strtok = strtok_s(data, separator, &buf);

			int index = 0;
			while (ret_strtok)
			{
				//cout << index << endl;
				//cout << ret_strtok << endl;
				if (1 == index)
				{
					strncpy_s(test_value, sizeof(ret_value), ret_strtok, (strlen(ret_strtok) + 1));
					//cout << test_value << endl;
				}

				//p = strtok(NULL, d);
				ret_strtok = strtok_s(NULL, separator, &buf);
				index += 1;
			}
			free(data);
		}
	}
	else
	{
		//参数错误
		return -2;
	}

	return 0;
}

/// <summary>
/// TypeC耳机设置悬空状态（即：耳机拔出），即耳机的各个管脚都置处于悬空状态
/// </summary>
/// <returns></returns>
_declspec(dllexport) int EarphoneSuspend()
{
	string ret = TestCommandResult(sockclient, "[1]earphone disconnect()\r\n");

	return 0;
}

/// <summary>
/// TypeC耳机正插测试
/// 将耳机各个管脚设置为正插状态（即：正插接入TypeC耳机）
/// </summary>
/// <returns>0：Pass; 非0：失败</returns>
_declspec(dllexport) int EarphoneInsertTestTop()
{
	string ret = TestCommandResult(sockclient, "[]earphone insert(top)\r\n");

	return 0;
}

/// <summary>
/// TypeC耳机反插测试
/// 将耳机各个管脚设置为反插状态（即：反插接入TypeC耳机）
/// </summary>
/// <returns>0：Pass; 非0：失败</returns>
_declspec(dllexport) int EarphoneInsertTestBot()
{
	string ret = TestCommandResult(sockclient, "[]earphone insert(bot)\r\n");

	return 0;
}


/// <summary>
/// TypeC耳机按键测试
/// </summary>
/// <param name="pressTime">耳机按键按下并保持按住的时间，单位：毫秒</param>
/// <returns>0：Pass; 非0：失败</returns>
_declspec(dllexport) int EarphonePressKeyTest(int pressTime)
{
	char cmd[100]{};
	//const char* linkrate = linkRate.c_str();

	sprintf_s(cmd, 100, "[]earphone presstime(%d)\r\n", pressTime);
	//cout << cmd << endl;

	const char* real_cmd = cmd;
	string ret = TestCommandResult(sockclient, real_cmd);

	return 0;
}

_declspec(dllexport) int HelloWorldPrintf()
{
	cout << "1234 Hello World!" << endl;
	return 0;
}