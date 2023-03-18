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


	//初始化Socket环境
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		//printf("WASStartup failed !\n");
		std::cout << "WASStartup failed !" << std::endl;
		return 1;
	}


	//创建用于监听的Socket
	sServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sServer == INVALID_SOCKET)
	{
		//printf("socket failed !\n");
		std::cout << "socket failed !" << std::endl;
		return -1;
	}


	//设置服务器Socket地址
	SOCKADDR_IN addrServ;
	addrServ.sin_family = AF_INET;
	addrServ.sin_port = htons(6666);
	//addrServ.sin_addr.S_un.S_addr = htons(INADDR_ANY);


	//绑定Sockets Server 到本地地址
	retVal = ::bind(sServer, (const struct sockaddr*)&addrServ, sizeof(SOCKADDR_IN));
	if (SOCKET_ERROR == retVal)
	{
		std::cout << "bind failed !" << std::endl;
		closesocket(sServer);
		WSACleanup();
		return -1;
	}


	//在Socket Server上进行监听
	retVal = listen(sServer, 1);
	if (SOCKET_ERROR == retVal)
	{
		std::cout << "listen failed !" << std::endl;
		closesocket(sServer);
		WSACleanup();
		return -1;
	}




	//接受来自客户端的请求
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




	//在服务器与客户端之间发送和接收数据

	int flag = 0;
	//循环接受客户端的数据，直接客户端发送quit命令后退出
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
		//获取当前系统时间
		SYSTEMTIME st;
		GetLocalTime(&st);
		char sDataTime[30];
		sprintf(sDataTime, "%4d-%2d-%2d %2d:%2d:%2d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
		//打印输出信息
		std::cout << sDataTime << "Recv from Client [" << inet_ntoa(addrClient.sin_addr) << ":";
		std::cout << addrClient.sin_port << "] : " << buf << std::endl;


		////如果客户端发送“quit”字符串，则服务器退出
		//if (strcmp(buf, "quit") == 0)
		//{
		//	retVal = send(sClient, "quit", strlen("quit"), 0);
		//	break;
		//}
		//否则向客户端发送回显字符串
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


	//释放资源
	closesocket(sServer);
	closesocket(sClient);
	WSACleanup();
	return 0;
}
