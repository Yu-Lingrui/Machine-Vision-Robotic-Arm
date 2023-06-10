#pragma once

#include<iostream>
#include<thread>
#include<math.h>
#include"Move.h"
#include"Reset.h"

#include"libArmObj.h"

using namespace std;
extern float pa, pb, pc;

// 关节长度
#define a1 52
#define a2 60
#define a3 28
#define angle M_PI / 6

void jxb_move() {

	float X = 100 * pa, Y = 100 * pb, Z = 100 * pc, q, p, turn_angle;
	//float X = pa, Y = pb, Z = pc, q, p, turn_angle;
	cout << "X  " << X << " Y  " << Y << " Z  " << Z << endl;

	/*d415自身X方向固定补偿修复*/  
	X = X + 20; // 15
	/*修复点击距离误差*/
	//Z = Z - 3;
	//Y = Y - 1;
	//D-H旋转坐标法:
	/******************************/
	//HSArm *armObj = createArmObj();
	//int q2anglePara = 1;
	//double qx = 0;
	//double qz = 0;
	//double qy = 0;
	//double q0s = 0;
	//double q1s = 0;
	//double q2s = 0;
	//double q3s = 0;
	//double q4s = 0;
	//getArmAngleWithPositon(armObj, q2anglePara, Z * 10, - (X * 10), (34 - Y) * 10, qx, qz, &q0s,&q1s, &q2s, &q3s, &q4s, &qy); //qy:q0;qx:q4;qz:q1
	//float theta1, theta2, theta3, theta4, theta5;
	//theta1 = q0s; 
	//theta2 = q1s;
	//theta3 = q2s;
	//theta4 = - q3s;
	//theta5 = q4s; // 旋转
	/******************************/
	int t = 0;
	q = 30 + abs(Z * sqrt(3) / 2) + abs(Y / 2);
	cout << "q = " << q << endl;
	p = abs(X / q);
	cout << "p = " << p << endl;
	turn_angle = atan(p) * 180 / M_PI / 90 * 38;
	//t = turn_angle * 0.8;
	t = turn_angle;
	cout << "t = " << t << endl;
	int flag = 1;
	// init 转肩 -> X；
	if (X < 0)
	{
		if (t < 0) t = 0;
		t = t - 1;
		Move(8, '-', t);//0x2b
		flag = 0; 
	}
	else
	{
		if (t < 0) t = 0;
		t = t * 2;
		Move(8, '+', t);//0x2d
	}
	//cout << "t = " << t << endl;

	float X2 = X, turnangle, n, m;
	//m = 30 + sqrt(Z * Z + Y * Y / 4);
	n = abs(30 * X2 / q);
	turnangle = atan(n / 30);
	if (X >= 0)
	{
		turnangle = -turnangle;
	}
	//旋转坐标切换左手坐标系+三次旋转角度转换(欧拉角)
	float xt, yt, zt, xt2, yt2, zt2, xt3, yt3, zt3;
	xt = X;//X
	yt = Y * cos(-angle) - Z * sin(-angle);
	zt = Y * sin(-angle) + Z * cos(-angle);
	xt2 = xt * cos(turnangle) + zt * sin(turnangle);
	yt2 = yt;//Y
	zt2 = -xt * sin(turnangle) + zt * cos(turnangle);
	xt3 = xt2;//X
	yt3 = yt2 * cos(angle) - zt2 * sin(angle);
	zt3 = zt2 * sin(angle) + zt2 * cos(angle);
	cout << "xt " << xt3 << " yt " << yt3 << " zt " << zt3 << endl;

	// 二次近距离获取坐标
	//P b;
	//b = getXYZ();
	//float x0 = 100 * b.a, y0 = 100 * b.b, z0 = 100 * b.c;

	// 平面映射
	float x0 = xt3, y0 = yt3 * 1.1, z0 = zt3 * 0.9;
	cout << "x0  " << x0 << " y0  " << y0 << " z0  " << z0 << endl;
	//float c;
	//float x = 15 + z0 * sqrt(3) / 2 - y0 / 2, y = 34 - z0 / 2 - y0 * sqrt(3) / 2;
	float x = z0 * sqrt(3) / 2 - y0 / 2, y = 34 - z0 / 2 - y0 * sqrt(3) / 2; // 根据初始复位位置调整
	cout << "x  " << x << " y  " << y << endl;

	// 修改后三自由度版本
	float angle1, angle2, cos_angle2, angle3, cos_angle3, angle4, angle5, angle6, theta1, theta2, theta3;
	float OA;
	OA = sqrt((x - a3) * (x - a3) + y * y);
	angle1 = atan(y / (x - a3));
	cos_angle2 = (OA * OA + a1 * a1 - a2 * a2) / (2 * a1 * OA);
	if (cos_angle2 < -1)
	{
		cos_angle2 = -1;
	}
	else if (cos_angle2 > 1)
	{
		cos_angle2 = 1;
	}
	angle2 = acos(cos_angle2);
	theta1 = M_PI * 5 / 6 - angle1 - angle2;
	/*theta1 = angle1 - angle2;*/
	//theta1 = angle1 + angle2 - M_PI/2; // 测试
	theta1 = theta1 * 180 / M_PI;
	theta1 -= 85;//减去初值 -62

	cos_angle3 = (a1 * a1 + a2 * a2 - OA * OA) / (2 * a1 * a2);
	if (cos_angle3 < -1)
	{
		cos_angle3 = -1;
	}
	else if (cos_angle3 > 1)
	{
		cos_angle3 = 1;
	}
	angle3 = acos(cos_angle3);
	/*theta2 = angle3 - M_PI / 6;*/
	theta2 = M_PI - angle3;//调节
	theta2 = theta2 * 180 / M_PI;
	/*theta2 = 156 - theta2 + 17;*/

	angle5 = M_PI - angle2 - angle3;
	angle6 = atan((x - a3) / y);
	if (y >= 0) angle4 = M_PI * 3 / 2 - angle5 - angle6;
	else angle4 = -angle5 - angle6 + M_PI / 2;
	theta3 = M_PI * 4 / 3 - angle4;
	/*theta3 = M_PI - angle4;*/
	/*theta3 = theta3 * 180 / M_PI;*/
	theta3 = theta3 * 180 / M_PI * 13 / 90;
	theta3 -= 30;//调节


	 
	/*if (flag == 0)
	{
	x = x * 0.95;
	y = y * 0.8;
	}
	else
	{
	x = x * 0.95;
	y = y * 0.8;

	}
	cout << "x  " << x << " y  " << y << endl;*/
	// 两自由度原始版本
	//float c;
	//float beta, cos_beta, alpha, cos_alpha, gama, cos_gama, theta1, theta2;
	//c = sqrt(a2 * a2 + a3 * a3 - 2 * a2 * a3 * cos(M_PI - angle));
	//cos_beta = (x * x + y * y + a1 * a1 - c * c) / (2 * a1 * sqrt(x * x + y * y));
	//beta = acos(cos_beta);
	//if (y >= 0)
	//{
	//theta1 = atan(x / y) - beta;
	//}
	//else theta1 = M_PI + atan(x / y) - beta;
	//theta1 = theta1 * 180 / M_PI + 60;
	////theta1 = theta1 * 180 / M_PI;
	//cos_alpha = (a1 * a1 + c * c - x * x - y * y) / (2 * a1 * c);
	//alpha = acos(cos_alpha);
	//cos_gama = (a2 * a2 + c * c - a3 * a3) / (2 * a2 * c);
	//gama = acos(cos_gama);
	//theta2 = M_PI - alpha - gama;
	////theta2 = 150 - theta2 * 180 / M_PI;
	//theta2 = theta2 * 180 / M_PI;
	

	int n1, n2, n3;
	n1 = theta1;
	n2 = theta2;
	n3 = theta3;
	//微调
	if (Z < 60 && Z > 50)
	{
		n1 -= 15; 
		n2 -= 35;
		n3 += 20;
	}
	else if (Z < 50)
	{
		n1 += 50; // 近距离可能到达不了必须修复
		n2 -= 20;
		//n3 += 20;
	}
	else
	{
		n1 -= 1;   // 远距离较准确，只需微调
		n2 -= 20;
		//n3 += 20;
	}

	cout << "n1 " << n1 << endl;
	cout << "n2 " << n2 << endl;
	cout << "n3 " << n3 << endl;

	/*cout << "theta1 " << theta1 << endl;
	cout << "theta2 " << theta2 << endl;
	cout << "theta3 " << theta3 << endl;
	cout << "theta4 " << theta4 << endl;*/

	/*****独立验证准确性,根据反馈实时跟踪循环迭代,解决多线程Can收发不灵敏感和重复误差的问题*****/
	int shoulder1_set_angle = shoulder1.set_angle;
	int shoulder4_set_angle = shoulder4.set_angle;
	int shoulder2_set_angle = shoulder2.set_angle;
	int shoulder3_set_angle = shoulder3.set_angle;
	shoulder2.set_angle = n1;
	shoulder3.set_angle = n2;
	shoulder4.set_angle = n3;
	shoulder1.set_angle = shoulder1.curr_angle;
	/*shoulder1.set_angle = theta1;
	shoulder2.set_angle = theta2;
	shoulder3.set_angle = theta3;
	shoulder4.set_angle = theta4;*/
	thread m0 = thread(reset);
	m0.join();
	/******************微调************************/
	//if (Z >= 80)
	//{
	//	n1 = n1; //0.82
	//	n2 = n2; //0.85;
	//	//n3 = n3 * 0.6; //0.6
	//}
	//else if (Z >= 40)
	//{
	//	n1 = n1; //0.82
	//	n2 = n2; //0.85;
	//	//n3 = n3 * 0.6; //0.6
	//}
	//else
	//{
	//	n1 = n1; 
	//	n2 = n2; 
	//	//n3 = n3 * 0.6; 
	//}
	//cout << "n1 " << n1 << endl;
	//cout << "n2 " << n2 << endl;
	//cout << "n3 " << n3 << endl;

	/*getchar();
	while (1)
	{
	if (_kbhit())
	{
	cout << "BEGIN2" << endl;
	break;
	}
	}*/

	//Move(int id关节号, char dir方向, int times次数, int nDeviceType = 3, int nDeviceInd = 0, int nCANIndint = 0)

	//Sleep(100);

	//Move(8, '+', 1);
	//n2 += 5;
	//n1 += 1;
	//张合 -> XYZ 同步 
	//thread m2(Move, 2, '-', n2 + 17, 3, 0, 0, &shoulder3);
	//thread m3(Move, 1, '+', n1, 3, 0, 0, &shoulder2);
	////thread m7(Move, 3, '-', n3, 3, 0, 0, &shoulder4);
	/*thread m5(Move, 5, '+', 8, 3, 0, 0, &shoulder5);*/
	//m2.join();
	//m3.join();
	////m7.join();
	/*m5.join();*/
	//-抓 逆时针
	/*Move(5, '+', 18	Move(2, '-', n2+2););	

	Move(1, '+', n1+1);*/

	//-抓取
	Move(5, '+', 5);
	Move(5, '-', 37);

	//Move(1, '-', n1+2);
	//Move(2, '+', n2+2);
	//返回
	/********************/
	shoulder1.set_angle = shoulder1_set_angle;
	shoulder2.set_angle = shoulder2_set_angle;
	shoulder3.set_angle = shoulder3_set_angle;
	shoulder4.set_angle = shoulder4_set_angle;
	shoulder5.set_angle = 13;
	thread m1 = thread(reset);
	m1.join();
	/*******************/
	//if (n1 % 2 == 1)
	//{
	//	thread m4(Move, 1, '-', n1 / 2 + 1, 3, 0, 0, &shoulder2);
	//	m4.join();
	//}
	//else
	//{
	//	thread m4(Move, 1, '-', n1 / 2, 3, 0, 0, &shoulder2);
	//	m4.join();
	//}
	//thread m6(Move, 1, '-', n1 / 2, 3, 0, 0, &shoulder2);
	//thread m9(Move, 2, '+', n2 + 17, 3, 0, 0, &shoulder3);
	////thread m8(Move, 3, '+', n3, 3, 0, 0, &shoulder4);
	//m6.join();
	//m9.join();
	//m8.join();
	//Move(1, '+', 1);

	//Move(8, '-', 1);
	//复位
	/*if (flag == 0)
	{
		Move(8, '+', t);
	}
	else Move(8, '-', t);*/
	
	cout << "抓取结束!" << endl;
	//Sleep(-1);
	//m4.join();
	//m5.join();
}
