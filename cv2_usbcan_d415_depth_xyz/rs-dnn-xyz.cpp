// This example is derived from the ssd_mobilenet_object_detection opencv demo
// and adapted to be used with Intel RealSense Cameras
// Please see https://github.com/opencv/opencv/blob/master/LICENSE
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <WinSock2.h>
#include <stdlib.h>
//#include"socketpixel.h"

#include<string>
#include<cstdlib>
#include <opencv2/dnn.hpp>
#include <librealsense2/rs.hpp>
//#include <rs_frame.hpp>
#include <librealsense2/rsutil.h>
#include "cv-helpers.hpp"
#include<Windows.h>
#include<thread>
#define _USE_MATH_DEFINES
#include<math.h>
#include<conio.h>
#include<iostream>
//#include<fstream>
//#include <vector>

//#include"transVideo.h"
#include"JXB.h"
#include"Reset.h"


#pragma warning(disable : 4996)

using namespace cv;
using namespace std;
using namespace cv::dnn;

//待传输图像默认大小为 640*480，可修改
#define IMG_WIDTH 640	// 需传输图像的宽
#define IMG_HEIGHT 480	// 需传输图像的高
//默认格式为CV_8UC3


//#define BUFFER_SIZE IMG_WIDTH*IMG_HEIGHT*3/32 
//struct sentbuf {
//	char buf[BUFFER_SIZE];
//	int flag;
//};
//sentbuf datadata;
//void sendMat(SOCKET sockClient, Mat image1);
//vector<String> GetOutputsNames();

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma comment(lib,"ControlCAN.lib")
#pragma comment (lib,"WS2_32.lib")

//#define BUF_SIZE 64

//extern int xcoor, ycoor;
Mat timage;
//extern bool FF;
bool FF = true;
struct P
{
	float a;
	float b;
	float c;
};

const size_t inWidth = 416;
const size_t inHeight = 416;
const float WHRatio = inWidth / (float)inHeight;
const float inScaleFactor = 0.007843f;
const float meanVal = 127.5;
const char* classNames[] = { "background",
"aeroplane", "bicycle", "bird", "boat",
"bottle", "bus", "car", "cat", "chair",
"cow", "diningtable", "dog", "horse",
"motorbike", "person", "pottedplant",
"sheep", "sofa", "train", "tvmonitor" };
//yolo
// Net net;
//vector<string> classNames;
//string classesFile = "./yolo/coco.names";
//String modelConfig = "./yolo/yolov4.cfg";
//String modelWeights = "./yolo/yolov4.weights";
//加载类别名
//ifstream ifs(classesFile.c_str());

P a;
float pa, pb, pc;
//float pixel[2]; // 像素坐标



P getXYZ()
{
	using namespace rs2;

	Net net = readNetFromCaffe("MobileNetSSD_deploy.prototxt", "MobileNetSSD_deploy.caffemodel");
	//加载网络模型(yolo need x86 cv4.7)
	/*string line;
	while (getline(ifs, line)) classNames.push_back(line);
	net = readNetFromDarknet(modelConfig, modelWeights);
	net.setPreferableBackend(DNN_BACKEND_OPENCV);
	net.setPreferableTarget(DNN_TARGET_OPENCL);*/

	// Start streaming from Intel RealSense Camera
	pipeline pipe;
	auto config = pipe.start();
	auto profile = config.get_stream(RS2_STREAM_COLOR)
		.as<video_stream_profile>();
	rs2::align align_to(RS2_STREAM_COLOR);

	Size cropSize;
	if (profile.width() / (float)profile.height() > WHRatio)
	{
		cropSize = Size(static_cast<int>(profile.height() * WHRatio),
			profile.height());
	}
	else
	{
		cropSize = Size(profile.width(),
			static_cast<int>(profile.width() / WHRatio));
	}

	Rect crop(Point((profile.width() - cropSize.width) / 2,
		(profile.height() - cropSize.height) / 2),
		cropSize);

	

		//TCP
		//WSADATA wsaData;
		//::WSAStartup(MAKEWORD(2, 0), &wsaData);
		////创建套接字
		//SOCKET servSock = ::socket(AF_INET, SOCK_STREAM, 0);

		////绑定套接字
		//sockaddr_in sockAddr;
		//sockAddr.sin_family = AF_INET;
		//sockAddr.sin_addr.S_un.S_addr = inet_addr("192.168.1.37");
		//sockAddr.sin_port = htons(1234);
		//::bind(servSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
		////进入监听状态
		//listen(servSock, 5);
		////接收客户端请求
		//std::cout << "TCP Server Start..." << std::endl;
		//SOCKADDR clntAddr;
		//int nSize = sizeof(SOCKADDR);
		//SOCKET clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);
		//std::cout << WSAGetLastError();
		//std::cout << "linked\n";
	


	bool flag = false;
	float point[3] = {0, 0, 0};
	float points[3];

	const auto window_name = "Display Image";
	namedWindow(window_name, WINDOW_AUTOSIZE);

	while (getWindowProperty(window_name, WND_PROP_AUTOSIZE) >= 0)
	//while (true)
	{
		// Wait for the next set of frames
		auto data = pipe.wait_for_frames();
		// Make sure the frames are spatially aligned
		data = align_to.process(data);

		auto color_frame = data.get_color_frame();
		auto depth_frame = data.get_depth_frame();

		// If we only received new depth frame, 
		// but the color did not update, continue
		static int last_frame_number = 0;
		if (color_frame.get_frame_number() == last_frame_number) continue;
		last_frame_number = color_frame.get_frame_number();
		
		// Convert RealSense frame to OpenCV matrix:
		auto color_mat = frame_to_mat(color_frame);
		auto depth_mat = depth_frame_to_meters(depth_frame);

		Mat inputBlob = blobFromImage(color_mat, inScaleFactor,
			Size(inWidth, inHeight), meanVal, false); //Convert Mat to batch of images
		net.setInput(inputBlob, "data"); //set the network input
		//net.setInput(inputBlob); //set the network input
		Mat detection = net.forward("detection_out"); //compute output
		//Mat detection;
		//net.forward(detection, GetOutputsNames());

		Mat detectionMat(detection.size[2], detection.size[3], CV_32F, detection.ptr<float>());

		// Crop both color and depth frames
		color_mat = color_mat(crop);
		depth_mat = depth_mat(crop);

		// SSD detection
		float confidenceThreshold = 0.4f;
		for (int i = 0; i < detectionMat.rows; i++)
		{
			float confidence = detectionMat.at<float>(i, 2);

			if (confidence > confidenceThreshold)
			{
				size_t objectClass = (size_t)(detectionMat.at<float>(i, 1));

				int xLeftBottom = static_cast<int>(detectionMat.at<float>(i, 3) * color_mat.cols);
				int yLeftBottom = static_cast<int>(detectionMat.at<float>(i, 4) * color_mat.rows);
				int xRightTop = static_cast<int>(detectionMat.at<float>(i, 5) * color_mat.cols);
				int yRightTop = static_cast<int>(detectionMat.at<float>(i, 6) * color_mat.rows);

				Rect object((int)xLeftBottom, (int)yLeftBottom,
					(int)(xRightTop - xLeftBottom),
					(int)(yRightTop - yLeftBottom));

				object = object & Rect(0, 0, depth_mat.cols, depth_mat.rows);

				//Calculate mean depth inside the detection region
				//This is a very naive way to estimate objects depth
				//but it is intended to demonstrate how one might 
				//use depht data in general
				Scalar m = mean(depth_mat(object));

				std::ostringstream ss;
				ss << classNames[objectClass] << " ";
				ss << std::setprecision(2) << m[0] << " meters away";
				String conf(ss.str());

				rectangle(color_mat, object, Scalar(rand() % 256, rand() % 256, rand() % 256), 2);
				int baseLine = 0;
				Size labelSize = getTextSize(ss.str(), FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);

				auto center = (object.br() + object.tl()) * 0.5;

				if (classNames[objectClass] == "bottle")
				{	
					float countx = 0, county = 0, countz = 0;
					for (int i = xLeftBottom + static_cast<int>((xRightTop - xLeftBottom) / 3); 
					i < xLeftBottom + static_cast<int>((xRightTop - xLeftBottom) / 3 * 2); i++)
					{
						for (int j = yLeftBottom + static_cast<int>((yRightTop - yLeftBottom) / 3); 
						j < yLeftBottom + static_cast<int>((yRightTop - yLeftBottom) / 3 * 2); j++)
						{
							float pixel[2]; // 像素坐标
							//float point[3]; // 相机坐标(in 3D)

											// Copy pixels into the arrays (to match rsutil signatures)
							pixel[0] = i;
							pixel[1] = j;

							auto dist = depth_frame.get_distance(pixel[0], pixel[1]);

							rs2_intrinsics intr = depth_frame.get_profile().as<rs2::video_stream_profile>().get_intrinsics(); // Calibration data
							rs2_deproject_pixel_to_point(point, &intr, pixel, dist);

							countx = countx + point[0];
							county = county + point[1];
							countz = countz + point[2];
						}
					}
					point[0] = countx / static_cast<int>((xRightTop - xLeftBottom) * (yRightTop - yLeftBottom) / 9);
					point[1] = county / static_cast<int>((xRightTop - xLeftBottom) * (yRightTop - yLeftBottom) / 9);
					point[2] = countz / static_cast<int>((xRightTop - xLeftBottom) * (yRightTop - yLeftBottom) / 9);
					/*pixel[0] = center.x;
					pixel[1] = center.y;*/
				}
				center.x = center.x - labelSize.width / 2;

				rectangle(color_mat, Rect(Point(center.x, center.y - labelSize.height),
					Size(labelSize.width, labelSize.height + baseLine)),
					Scalar(255, 255, 255), FILLED); 
				putText(color_mat, ss.str(), center, FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 0));
			}
		}
		imshow(window_name, color_mat);
		//sendMat(clntSock, color_mat);
		
		if (FF) {
			//float pixel[2]; // 像素坐标
							// Copy pixels into the arrays (to match rsutil signatures)

			//pixel[0] = xcoor;
			//pixel[1] = ycoor;


			// Query the frame for distance
			// Note: this can be optimized
			// It is not recommended to issue an API call for each pixel
			// (since the compiler can't inline these)
			// However, in this example it is not one of the bottlenecks

			//auto dist = depth_frame.get_distance(pixel[0], pixel[1]);

			//rs2_intrinsics intr = depth_frame.get_profile().as<rs2::video_stream_profile>().get_intrinsics(); // Calibration data
			//rs2_deproject_pixel_to_point(Point, &intr, pixel, dist);

			if (point[0] != 0 && point[1] != 0 && point[2] != 0)
			{
				points[0] = point[0];
				points[1] = point[1];
				points[2] = point[2];
				std::cout << points[0] << "   " << points[1] << "   " << points[2] << std::endl;
			}
			//std::cout << point[0] << "   " << point[1] << "   " << point[2] << std::endl;

			/*p[loop][0] = point[0];
			p[loop][1] = point[1];
			p[loop][2] = point[2];*/
			//flag = true;
			//imshow(window_name, color_mat);
			//if (flag) break;
			if(waitKey(1) >= 0) break;
		}

	}
	//关闭套接字
	/*closesocket(clntSock);
	closesocket(servSock);*/

	//终止 DLL 的使用
	WSACleanup();

	/*	loop++;
	}
	std::cout << (p[1][0] + p[2][0] + p[3][0]) / 3 << "   " << (p[1][1] + p[2][1] + p[3][1]) / 3 << "   " << (p[1][2] + p[2][2] + p[3][2]) / 3 << std::endl;*/
	a.a = points[0];
	a.b = points[1];
	a.c = points[2];
	FF = false;
	return a;
}

int main()try
{
	int nDeviceType = 3;
	int nDeviceInd = 0;
	int nCANInd = 0;
	int nReserved = 0;
	DWORD dwRel;

	VCI_INIT_CONFIG init_config;
	//int index, cannum;
	int baud = 0;//默认为250kbps;
				 //250k,1000k,800k,500k,125k,100k,50k,20k,10k
	int timearray[9] = { 0x011c,0x0014,0x0016,0x001c,0x031c,0x041c,0x091c,0x181c,0x311c };

	init_config.AccCode = 0x00000000;//默认接收所有ID信息
	init_config.AccMask = 0xffffffff;
	init_config.Filter = 0;//默认双滤波方式(=1 单滤波)
	init_config.Mode = 0;//默认正常模式(=1 只听模式)
	init_config.Timing0 = (timearray[baud] >> 8) & 0xff;
	init_config.Timing1 = timearray[baud] & 0xff;

	/*while (1)
	{
		if (_kbhit())
		{
			cout << "BEGIN" << endl;
			break;
		}
	}*/

	//OpenDevice(nDeviceType, nDeviceInd, nReserved);
	if (VCI_OpenDevice(nDeviceType, nDeviceInd, 0) != STATUS_OK)
	{
		std::cout << "打开设备失败!" << std::endl;
	}
	else
	{
		std::cout << "打开设备成功" << std::endl;
	}
	//InitCAN(nDeviceType, nDeviceInd, nCANInd, &init_config);
	if (VCI_InitCAN(nDeviceType, nDeviceInd, nCANInd, &init_config) != STATUS_OK)
	{
		std::cout << "初始化CAN失败!" << std::endl; 
		VCI_CloseDevice(nDeviceType, nDeviceInd);
	}
	else
	{
		std::cout << "初始化CAN成功" << std::endl;
	}
	VCI_StartCAN(nDeviceType, nDeviceInd, nCANInd);
	VCI_ClearBuffer(nDeviceType, nDeviceInd, nCANInd);

	thread tt0 = thread(Rcv);
	std::cout << "开始复位！ " << std::endl;
	thread tt1 = thread(reset);
	tt1.join();
	std::cout << "开始识别！ " << std::endl;
	thread tt2=thread(getXYZ);
	tt2.join();   //wait；
	//thread tt2= thread(get_mouse_pixel);
	//tt2.join();
	pa = a.a;
	pb = a.b;
	pc = a.c;
	std::cout << "开始抓取！ " << std::endl;
	//thread tt4 = thread(getXYZ);
	thread tt3 = thread(jxb_move);
	//tt4.join();
	tt3.join();

	VCI_CloseDevice(nDeviceType, nDeviceInd);
	return 0;
}
catch (const rs2::error& e)
{
	std::cerr << "RealSense error calling " << e.get_failed_function() << "(" << e.get_failed_args() << "):\n    " << e.what() << std::endl;
	return EXIT_FAILURE;
}
catch (const std::exception& e)
{
	std::cerr << e.what() << std::endl;
	return EXIT_FAILURE;
}

//void sendMat(SOCKET sockClient, Mat image1) {
//	for (int k = 0; k < 32; k++) {
//		int num1 = IMG_HEIGHT / 32 * k;
//		for (int i = 0; i < IMG_HEIGHT / 32; i++) {
//			int num2 = i * IMG_WIDTH * 3;
//			uchar* ucdata = image1.ptr<uchar>(i + num1);
//			for (int j = 0; j < IMG_WIDTH * 3; j++) {
//				datadata.buf[num2 + j] = ucdata[j];
//			}
//		}
//		if (k == 31)
//			datadata.flag = 2;
//		else
//			datadata.flag = 1;
//		int r = send(sockClient, (char*)(&datadata), sizeof(datadata), 0);
//		//std::cout << WSAGetLastError();
//	}
//}

//获取网络输出层名称
// vector<String> GetOutputsNames()
// {
// 	static vector<String> names;
// 	if (names.empty())
// 	{
// 		//得到输出层索引号
// 		vector<int> outLayers = net.getUnconnectedOutLayers();
		
// 		//得到网络中所有层名称
// 		vector<String> layersNames = net.getLayerNames();
		
// 		//获取输出层名称
// 		names.resize(outLayers.size());
// 		for (int i = 0; i < outLayers.size(); ++i)
// 			names[i] = layersNames[outLayers[i] - 1];
// 	}
// 	return names;
// }


