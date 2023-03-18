#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <WinSock2.h>
#include <stdlib.h>

#include<string>

#pragma comment (lib,"WS2_32.lib")
#define BUF_SIZE 64

using namespace std;

extern int xcoor, ycoor;
bool FF=false;
int get_mouse_pixel()
{
	WSADATA wsd;
	SOCKET sServer;
	SOCKET sClient;
	int retVal;
	char buf[BUF_SIZE];


	//��ʼ��Socket����
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		//printf("WASStartup failed !\n");
		std::cout << "WASStartup failed !" << std::endl;
		return 1;
	}


	//�������ڼ�����Socket
	sServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sServer == INVALID_SOCKET)
	{
		//printf("socket failed !\n");
		std::cout << "socket failed !" << std::endl;
		return -1;
	}


	//���÷�����Socket��ַ
	SOCKADDR_IN addrServ;
	addrServ.sin_family = AF_INET;
	addrServ.sin_port = htons(6666);
	//addrServ.sin_addr.S_un.S_addr = htons(INADDR_ANY);


	//��Sockets Server �����ص�ַ
	retVal = ::bind(sServer, (const struct sockaddr*)&addrServ, sizeof(SOCKADDR_IN));
	if (SOCKET_ERROR == retVal)
	{
		std::cout << "bind failed !" << std::endl;
		closesocket(sServer);
		WSACleanup();
		return -1;
	}


	//��Socket Server�Ͻ��м���
	retVal = listen(sServer, 1);
	if (SOCKET_ERROR == retVal)
	{
		std::cout << "listen failed !" << std::endl;
		closesocket(sServer);
		WSACleanup();
		return -1;
	}




	//�������Կͻ��˵�����
	std::cout << "TCP Server Start..." << std::endl;
	sockaddr_in addrClient;
	int addrClientLen = sizeof(addrClient);
	sClient = accept(sServer, (sockaddr FAR*) & addrClient, &addrClientLen);
	if (INVALID_SOCKET == sClient)
	{
		std::cout << "accept failed !" << std::endl;
		closesocket(sServer);
		WSACleanup();
		return -1;
	}




	//�ڷ�������ͻ���֮�䷢�ͺͽ�������

	int flag = 0;
	//ѭ�����ܿͻ��˵����ݣ�ֱ�ӿͻ��˷���quit������˳�
	while (true)
	{
		memset(buf, 0, BUF_SIZE);
		retVal = recv(sClient, buf, BUF_SIZE, 0);
		if (SOCKET_ERROR == retVal)
		{
			std::cout << "recv failed !" << std::endl;
			closesocket(sServer);
			WSACleanup();
			return -1;
		}
		//��ȡ��ǰϵͳʱ��
		SYSTEMTIME st;
		GetLocalTime(&st);
		char sDataTime[30];
		sprintf(sDataTime, "%4d-%2d-%2d %2d:%2d:%2d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
		//��ӡ�����Ϣ
		std::cout << sDataTime << "Recv from Client [" << inet_ntoa(addrClient.sin_addr) << ":";
		std::cout << addrClient.sin_port << "] : " << buf << std::endl;


		////����ͻ��˷��͡�quit���ַ�������������˳�
		//if (strcmp(buf, "quit") == 0)
		//{
		//	retVal = send(sClient, "quit", strlen("quit"), 0);
		//	break;
		//}
		//������ͻ��˷��ͻ����ַ���
		//else
		{
			char msg[BUF_SIZE];
			sprintf(msg, "Message received - %s", buf);
			retVal = send(sClient, msg, strlen(msg), 0);
			if (SOCKET_ERROR == retVal)
			{
				std::cout << "send failed !" << std::endl;
				closesocket(sServer);
				closesocket(sClient);
				WSACleanup();
				return -1;
			}
			flag = 1;
			const char* sp = " ";
			char* p;
			p = strtok(buf, sp);
			string temp1 = p;
			xcoor = stoi(temp1);
			p = strtok(NULL, sp);
			string temp2 = p;
			ycoor = stoi(temp2);
		}
		FF = true;
		if (flag)
			break;

	}


	//�ͷ���Դ
	closesocket(sServer);
	closesocket(sClient);
	WSACleanup();
	return 0;
}
