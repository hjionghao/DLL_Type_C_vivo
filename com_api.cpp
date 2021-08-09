#include "dll.h"

CONNET_PORT cport;

_declspec(dllexport) bool connet_port(int port)
{
    if (!cport.OpenPort(port))
    {
        cout << "OpenPort COM" << port << " fail!" << endl;
        return false;
    }

    if (!cport.SetupDCB(115200))
    {
        cout << "SetupDCB fail!" << endl;
        return false;
    }

    if (!cport.SetupTimeout(0, 0, 3000, 0, 0))
    {
        cout << "SetupTimeout fail!" << endl;
        return -1;
    }

    PurgeComm(cport.hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

    cout << "connet port COM" << port << " succeess!" << endl;
    return 0; // connert serial port succeess.
}


//string TestCommand(const char* cmd)
//{
//    cout << "cmd   :" << cmd << "result:";
//    cport.WriteChar(cmd, strlen(cmd));
//
//    return cport.ReciveChar();
//}

string TestCommand(const char* cmd)
{
	//cout << "cmd   :" << cmd << "result:";
	cport.WriteChar(cmd, strlen(cmd));
	//Sleep(5 * 1000);
	string ret_str;
	while (1)
	{
		ret_str = cport.ReciveChar();
		int ret_find = ret_str.find("DONE");
		if (ret_find == string::npos)
			continue;
		else
		{
			break;
		}
	}
	//string ret_str = cport.ReciveChar();
	//cout << ret_str << endl;

	return ret_str;
}


_declspec(dllexport) const char* fw_version()
{
    //char cmd[100]{};
    ////const char* linkrate = linkRate.c_str();

    //sprintf_s(cmd, 100, "[]earphone presstime(%d)\r\n", pressTime);
    ////cout << cmd << endl;

    //const char* real_cmd = cmd;

    //return TestCommand("[1]version(255)\r\n");

    string version_msg;
    version_msg = TestCommand("[1]version(255)\r\n");
    const char* ret = version_msg.c_str();
    return ret;
}


/*
		/// <summary>
		/// �������
		/// �����ӿڷ���ֵ��������PASS,����0��������ʧ��ʱ�����ط���Ĵ����롣
		/// ��Ҫ�ṩ���д������Ӧ���С�Ӣ�ĵĴ���������Ϣ���ձ�
		/// </summary>
		/// <param name="usbVer">ָ�����Ե����ݣ�1:PIN test; 2:USB2.0 Test; 3:USB3.0</param>
		/// <returns>0��Pass; ��0��ʧ��</returns>
		public int comTypeCTestTop(TypeC_Flag flag);
*/

_declspec(dllexport) int comTypeCTestTop(int usbVer, char* usb_debug_info)
{
	// �ж���ڲ�����


	int pin_test_flag = 0;
	char top_debug_info[200]{};

	if (1 == usbVer)
	{
		string ret_GND = TestCommand("[1]pin test(GND)\r\n");
		int ret_find = ret_GND.find("PASS");
		if (ret_find == string::npos)
			return -10;
		else
			pin_test_flag += 1;

		string ret_GND_selftest = TestCommand("[2]pin test(VBUS)\r\n");
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
		string ret_usb2_0_top = TestCommand("[3]otg usb2_0 test(top)\r\n");

		sprintf_s(top_debug_info, 200, "%s\r\n", ret_usb2_0_top);
		strncpy_s(usb_debug_info, 200, top_debug_info, (strlen(top_debug_info) + 1));

		int ret_find = ret_usb2_0_top.find("PASS");
		if (ret_find == string::npos)
			return -12;
		else
			return 0;
	}
	else if (3 == usbVer)
	{
		string ret_usb3_0_top = TestCommand("[4]otg usb3_0 test(top)\r\n");

		sprintf_s(top_debug_info, 200, "%s\r\n", ret_usb3_0_top);
		strncpy_s(usb_debug_info, 200, top_debug_info, (strlen(top_debug_info) + 1));

		//cout << usb_debug_info << endl;
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
		/// �������
		/// �����ӿڷ���ֵ��������PASS,����0��������ʧ��ʱ�����ط���Ĵ����롣
		/// ��Ҫ�ṩ���д������Ӧ���С�Ӣ�ĵĴ���������Ϣ���ձ�
		/// </summary>
		/// <param name="usbVer">ָ�����Ե����ݣ�1:PIN test; 2:USB2.0 Test; 3:USB3.0</param>
		/// <returns>0��Pass; ��0��ʧ��</returns>
		public int comTypeCTestBot(TypeC_Flag flag);
*/
_declspec(dllexport) int comTypeCTestBot(int usbVer, char* usb_debug_info)
{
	// �ж���ڲ�����

	int pin_test_flag = 0;
	char bot_debug_info[200]{};

	if (1 == usbVer)
	{
		string ret_GND = TestCommand("[5]pin test(GND)\r\n");
		int ret_find = ret_GND.find("PASS");
		if (ret_find == string::npos)
			return -10;
		else
			pin_test_flag += 1;

		string ret_GND_selftest = TestCommand("[6]pin test(VBUS)\r\n");
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
		string ret_usb2_0_bot = TestCommand("[7]otg usb2_0 test(bot)\r\n");

		sprintf_s(bot_debug_info, 200, "%s\r\n", ret_usb2_0_bot);
		strncpy_s(usb_debug_info, 200, bot_debug_info, (strlen(bot_debug_info) + 1));

		int ret_find = ret_usb2_0_bot.find("PASS");
		if (ret_find == string::npos)
			return -14;
		else
			return 0;
	}
	else if (3 == usbVer)
	{
		string ret_usb3_0_bot = TestCommand("[8]otg usb3_0 test(bot)\r\n");

		sprintf_s(bot_debug_info, 200, "%s\r\n", ret_usb3_0_bot);
		strncpy_s(usb_debug_info, 200, bot_debug_info, (strlen(bot_debug_info) + 1));

		//cout << ret_usb3_0_bot << endl;
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
		/// DisplayPort�������
		/// �����ӿڷ���ֵ��������PASS,����0��������ʧ��ʱ�����ط���Ĵ����롣
		/// ��Ҫ�ṩ���д������Ӧ���С�Ӣ�ĵĴ���������Ϣ���ձ�
		/// </summary>
		/// <param name="linkRate">LinkRate</param>
		/// <param name="laneCount">LaneCount</param>
		/// <sparam name="resolution_X">�����Ƶ���ֱ��ʵ�ˮƽ�ֱ���ֵ</param>
		/// <param name="resolution_Y">�����Ƶ���ֱ��ʵĴ�ֱ�ֱ���ֵ</param>
		/// <param name="frameRate">�����Ƶ���֡��ֵ</param>
		/// <returns>0��Pass; ��0��ʧ��</returns>
		public int comDisplayPortTopTest(LinkeRate linkRate,int laneCount, out double resolution_X, out double resolution_Y, out double frameRate);
*/
_declspec(dllexport) int comDisplayPortTopTest(const char* linkRate, int laneCount, double* resolution_X, double* resolution_Y, double* frameRate, char* dp_debug_info)
{
	// ��ڲ����ж�

	char debug_info[300]{};

	char cmd[100]{};
	//const char* linkrate = linkRate.c_str();
	//cout << "Param linkRate:" << linkRate << endl;
	//cout << "Param laneCount:" << laneCount << endl;
	sprintf_s(cmd, 100, "[9]displayport test(top,%s, %d)\r\n", linkRate, laneCount);
	//cout << "Cmd:" << cmd << endl;

	const char* real_cmd = cmd;
	string ret_dp_bot = TestCommand(real_cmd);
	//cout << "Cmd result:" <<ret_dp_bot << endl;

	sprintf_s(debug_info, 300, "%s, %d, %s, %s\r\n", linkRate, laneCount, cmd, ret_dp_bot);
	//strcpy(dp_debug_info, debug_info);
	//cout << "DP debug info:" << debug_info << endl;
	//cout << strlen(debug_info) << endl;
	strncpy_s(dp_debug_info, 300, debug_info, (strlen(debug_info) + 1));

	int ret_find = ret_dp_bot.find("PASS");
	if (ret_find == string::npos)
	{
		//cout << "DP top test fail!" << endl;
		return -16;
	}
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
				//cout << "resolution_X:" << ret_strtok << endl;
				*resolution_X = atof(ret_strtok);
			}
			if (5 == index)
			{
				//cout << "resolution_Y:" << ret_strtok << endl;
				*resolution_Y = atof(ret_strtok);
			}
			if (10 == index)
			{
				//cout << "frameRate:" << ret_strtok << endl;
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
		/// DisplayPort�������
		/// �����ӿڷ���ֵ��������PASS,����0��������ʧ��ʱ�����ط���Ĵ����롣
		/// ��Ҫ�ṩ���д������Ӧ���С�Ӣ�ĵĴ���������Ϣ���ձ�
		/// </summary>
		/// <param name="linkRate">LinkRate</param>
		/// <param name="laneCount">LaneCount</param>
		/// <param name="resolution_X">�����Ƶ���ֱ��ʵ�ˮƽ�ֱ���ֵ</param>
		/// <param name="resolution_Y">�����Ƶ���ֱ��ʵĴ�ֱ�ֱ���ֵ</param>
		/// <param name="frameRate">�����Ƶ���֡��ֵ</param>
		/// <returns>0��Pass; ��0��ʧ��</returns>
		public int comDisplayPortBotTest(LinkeRate linkRate, int laneCount, out double resolution_X, out double resolution_Y, out double frameRate);
*/
_declspec(dllexport) int comDisplayPortBotTest(const char* linkRate, int laneCount, double* resolution_X, double* resolution_Y, double* frameRate, char* dp_debug_info)
{
	// ��ڲ����ж�

	char debug_info[300]{};

	char cmd[100]{};
	//const char* linkrate = linkRate.c_str();

	//cout << "Param linkRate:" << linkRate << endl;
	//cout << "Param laneCount:" << laneCount << endl;
	sprintf_s(cmd, 100, "[9]displayport test(bot,%s, %d)\r\n", linkRate, laneCount);
	//cout << "Cmd:" << cmd << endl;

	const char* real_cmd = cmd;
	string ret_dp_bot = TestCommand(real_cmd);
	//cout << "Cmd result:" << ret_dp_bot << endl;

	sprintf_s(debug_info, 300, "%s, %d, %s, %s\r\n", linkRate, laneCount, cmd, ret_dp_bot);
	//strcpy(dp_debug_info, debug_info);
	//cout << "DP debug info:" << debug_info << endl;
	//cout << strlen(debug_info) << endl;
	strncpy_s(dp_debug_info, 300, debug_info, (strlen(debug_info) + 1));

	int ret_find = ret_dp_bot.find("PASS");
	if (ret_find == string::npos)
	{
		//cout << "DP bot test fail!" << endl;
		return -17;
	}
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
				//cout << "resolution_X:" << ret_strtok << endl;
				*resolution_X = atof(ret_strtok);
			}
			if (5 == index)
			{
				//cout << "resolution_Y:" << ret_strtok << endl;
				*resolution_Y = atof(ret_strtok);
			}
			if (10 == index)
			{
				//cout << "frameRate:" << ret_strtok << endl;
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
/// PDSink VBUS ������������ѹ��������
/// </summary>
/// <param name="eLoadCurrent">ָ������Ŀ���������</param>
/// <param name="current">ʵ�ʲ����������</param>
/// <param name="voltage">ʵ�ʲ��������ѹ</param>
/// <param name="delay_ms">��ʱʱ��</param>
/// <returns>0��Pass; ��0��ʧ��</returns>
_declspec(dllexport) int comPDSinkVBusTestTop(int eLoadCurrent, int* current, int* voltage, int delay_ms)
{
	// ��ڲ����ж�


	char cmd[100]{};
	//const char* linkrate = linkRate.c_str();

	sprintf_s(cmd, 100, "[9]pd sink test(vbus, top, %d, %d)\r\n", eLoadCurrent, delay_ms);
	//cout << cmd << endl;

	const char* real_cmd = cmd;
	string ret_pd_sink_bot = TestCommand(real_cmd);
	//cout << ret_pd_sink_bot << endl;

	//int ret_find = ret_pd_sink_bot.find("PASS");
	//if (ret_find == string::npos)
	//	return -18;
	//else
	//{
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
	//}
	return 0;
}

/// <summary>
/// PDSink VBUS ������������ѹ��������
/// </summary>
/// <param name="eLoadCurrent">ָ������Ŀ���������</param>
/// <param name="current">ʵ�ʲ����������</param>
/// <param name="voltage">ʵ�ʲ��������ѹ</param>
/// <param name="delay_ms">��ʱʱ��</param>
/// <returns>0��Pass; ��0��ʧ��</returns>
_declspec(dllexport) int comPDSinkVBusTestBot(int eLoadCurrent, int* current, int* voltage, int delay_ms)
{
	// ��ڲ����ж�


	char cmd[100]{};
	//const char* linkrate = linkRate.c_str();

	sprintf_s(cmd, 100, "[9]pd sink test(vbus, bot, %d, %d)\r\n", eLoadCurrent, delay_ms);
	//cout << cmd << endl;

	const char* real_cmd = cmd;
	string ret_pd_sink_bot = TestCommand(real_cmd);
	//cout << ret_pd_sink_bot << endl;

	//int ret_find = ret_pd_sink_bot.find("PASS");
	//if (ret_find == string::npos)
	//	return -19;
	//else
	//{
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
		//cout << ret_strtok << endl;
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
	//}
	return 0;
}


/// <summary>
/// USB PIN����
/// �����ӿڷ���ֵ��������PASS,����0��������ʧ��ʱ�����ط���Ĵ����롣
/// ��Ҫ�ṩ���д������Ӧ���С�Ӣ�ĵĴ���������Ϣ���ձ�
/// </summary>
/// <param name="Pin_Flag">ָ�����Ե����ݣ�0:GND 1:VBUS</param>
/// <returns>0��Pass; ��0��ʧ��</returns>
_declspec(dllexport) int comTypeCPinTest(int Pin_Flag, char* test_value)
{
	char ret_value[200] = "";

	if (0 == Pin_Flag)
	{
		string ret_GND = TestCommand("[1]pin test(GND)\r\n");
		//cout << ret_GND << endl;
		//int ret_find = ret_GND.find("PASS");
		//if (ret_find == string::npos)
		//{
		//	//return -10;
		//	char* data;
		//	int len = ret_GND.length();
		//	data = (char*)malloc((len + 1) * sizeof(char));
		//	ret_GND.copy(data, len, 0);

		//	const char* separator = ";";
		//	char* ret_strtok;
		//	char* buf;
		//	ret_strtok = strtok_s(data, separator, &buf);

		//	int index = 0;
		//	while (ret_strtok)
		//	{
		//		//cout << index << endl;
		//		//cout << ret_strtok << endl;
		//		if (1 == index)
		//		{
		//			strncpy_s(test_value, sizeof(ret_value), ret_strtok, (strlen(ret_strtok) + 1));
		//			//cout << test_value << endl;
		//		}

		//		//p = strtok(NULL, d);
		//		ret_strtok = strtok_s(NULL, separator, &buf);
		//		index += 1;
		//	}
		//	free(data);
		//}
		//else
		//{
		char* data;
		int len = ret_GND.length();
		data = (char*)malloc((len + 1) * sizeof(char));
		ret_GND.copy(data, len, 0);

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
		//}
	}
	else if (1 == Pin_Flag)
	{
		string ret_VBUS = TestCommand("[2]pin test(VBUS)\r\n");
		//cout << ret_GND << endl;
		//int ret_find = ret_VBUS.find("PASS");
		//if (ret_find == string::npos)
		//	return -11;
		//else
		//{
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
		//}
	}
	else
	{
		//��������
		return -2;
	}

	return 0;
}


/// <summary>
/// TypeC������������״̬�����������γ������������ĸ����ܽŶ��ô�������״̬
/// </summary>
/// <returns></returns>
_declspec(dllexport) int comEarphoneSuspend()
{
	string ret = TestCommand("[1]earphone disconnect()\r\n");

	return 0;
}

/// <summary>
/// TypeC�����������
/// �����������ܽ�����Ϊ����״̬�������������TypeC������
/// </summary>
/// <returns>0��Pass; ��0��ʧ��</returns>
_declspec(dllexport) int comEarphoneInsertTestTop()
{
	string ret = TestCommand("[]earphone insert(top)\r\n");

	return 0;
}


/// <summary>
/// TypeC�����������
/// �����������ܽ�����Ϊ����״̬�������������TypeC������
/// </summary>
/// <returns>0��Pass; ��0��ʧ��</returns>
_declspec(dllexport) int comEarphoneInsertTestBot()
{
	string ret = TestCommand("[]earphone insert(bot)\r\n");

	return 0;
}


/// <summary>
/// TypeC������������
/// </summary>
/// <param name="pressTime">�����������²����ְ�ס��ʱ�䣬��λ������</param>
/// <returns>0��Pass; ��0��ʧ��</returns>
_declspec(dllexport) int comEarphonePressKeyTest(int pressTime)
{
	char cmd[100]{};
	//const char* linkrate = linkRate.c_str();

	sprintf_s(cmd, 100, "[]earphone presstime(%d)\r\n", pressTime);
	//cout << cmd << endl;

	const char* real_cmd = cmd;
	string ret = TestCommand(real_cmd);

	return 0;
}