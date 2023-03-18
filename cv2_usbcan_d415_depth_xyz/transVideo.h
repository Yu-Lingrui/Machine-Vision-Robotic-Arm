//#include "opencv2/opencv.hpp"
//#include <WinSock2.h>
//#include <Windows.h>
//#pragma comment (lib, "ws2_32.lib")  //加载 ws2_32.dll
//#pragma warning(disable : 4996)
//
//using namespace cv;
//using namespace std;
//
////待传输图像默认大小为 640*480，可修改
//#define IMG_WIDTH 640	// 需传输图像的宽
//#define IMG_HEIGHT 480	// 需传输图像的高
////默认格式为CV_8UC3
//#define BUFFER_SIZE IMG_WIDTH*IMG_HEIGHT*3/32 
//
//extern Mat timage;
//struct sentbuf {
//	char buf[BUFFER_SIZE];
//	int flag;
//};
//sentbuf datadata;
//void sendMat(SOCKET sockClient, Mat image);
//
//int trans_video() {
//	WSADATA wsaData;
//	::WSAStartup(MAKEWORD(2, 0), &wsaData);
//	//创建套接字
//	SOCKET servSock = ::socket(AF_INET, SOCK_STREAM, 0);
//
//	//绑定套接字
//	sockaddr_in sockAddr;
//	sockAddr.sin_family = AF_INET;
//	sockAddr.sin_addr.S_un.S_addr = inet_addr("192.168.1.103");
//	sockAddr.sin_port = htons(1234);
//	::bind(servSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
//	//进入监听状态
//	listen(servSock, 5);
//	//接收客户端请求
//	SOCKADDR clntAddr;
//	int nSize = sizeof(SOCKADDR);
//	SOCKET clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);
//	std::cout << "linked\n";
//	//cap >> frame; // get a new frame from camera
//	sendMat(clntSock, timage);
//	// the camera will be deinitialized automatically in VideoCapture destructor
//
//	//关闭套接字
//	closesocket(clntSock);
//	closesocket(servSock);
//
//	//终止 DLL 的使用
//	WSACleanup();
//
//	return 0;
//}
//
//void sendMat(SOCKET sockClient, Mat image) {
//	for (int k = 0; k < 32; k++) {
//		int num1 = IMG_HEIGHT / 32 * k;
//		for (int i = 0; i < IMG_HEIGHT / 32; i++) {
//			int num2 = i * IMG_WIDTH * 3;
//			uchar* ucdata = image.ptr<uchar>(i + num1);
//			for (int j = 0; j < IMG_WIDTH * 3; j++) {
//				datadata.buf[num2 + j] = ucdata[j];
//			}
//		}
//		if (k == 31)
//			datadata.flag = 2;
//		else
//			datadata.flag = 1;
//		send(sockClient, (char*)(&datadata), sizeof(datadata), 0);
//	}
//}