#pragma once

#include<iostream>
#include<thread>
#include<math.h>
#include"Move.h"

using namespace std;
extern float pa, pb, pc;

#define a1 2
#define a2 62
#define a3 12
#define angle M_PI/6

void jxb_move() {

	float X = 100 * pa, Y = 100 * pb, Z = 100 * pc, q, p, turn_angle;
	//float X = pa, Y = pb, Z = pc, q, p, turn_angle;
	cout << "X  " << X << " Y  " << Y << " Z  " << Z << endl;

	//X = X + 5;
	//Z = Z+2
	//Y = Y - 10;
	//Z = Z - 10;
	/*X = X + 4.5;
	Y = Y - 13;
	Z = Z - 13;*/
	int t = 0;
	q = 15 + abs(Z * sqrt(3) / 2) + abs(Y / 2);
	cout << "q = " << q << endl;
	p = abs(X / q);
	cout << "p = " << p << endl;
	turn_angle = atan(p) * 180 / M_PI / 90 * 38;
	t = turn_angle;
	cout << "t = " << t << endl;
	int flag = 1;
	// init 转肩 -> X；
	if (X >= -5)
	{
		if (t < 0) t = 0;
		//t = t - 1;
		Move(8, '+', t);//0x2b
		flag = 0;
	}
	else
	{
		//t = t - 1;
		Move(8, '-', t);//0x2d
	}
	//cout << "t = " << t << endl;

	float X2 = X, turnangle, n;
	n = abs(X2 / q);
	turnangle = atan(n);
	if (X >= 0)
	{
		turnangle = -turnangle;
	}
	float xt, yt, zt, xt2, yt2, zt2, xt3, yt3, zt3;
	xt = X;
	yt = Y * cos(-angle) - Z * sin(-angle);
	zt = Y * sin(-angle) + Z * cos(-angle);
	xt2 = xt * cos(turnangle) + zt * sin(turnangle);
	yt2 = yt;
	zt2 = -xt * sin(turnangle) + zt * cos(turnangle);
	xt3 = xt2;
	yt3 = yt2 * cos(angle) - zt2 * sin(angle);
	zt3 = zt2 * sin(angle) + zt2 * cos(angle);
	cout << "xt " << xt3 << " yt " << yt3 << " zt " << zt3 << endl;

	//P b;
	//b = getXYZ();
	//float x0 = 100 * b.a, y0 = 100 * b.b, z0 = 100 * b.c;

	float x0 = xt3, y0 = yt3 * 1.1, z0 = zt3 * 0.87;
	cout << "x0  " << x0 << " y0  " << y0 << " z0  " << z0 << endl;
	float c;
	float x = 15 + z0 * sqrt(3) / 2 - y0 / 2, y = 34 - z0 / 2 - y0 * sqrt(3) / 2;
	cout << "x  " << x << " y  " << y << endl;

	float angle1, angle2, cos_angle2, angle3, cos_angle3, angle4, angle5, angle6, theta1, theta2, theta3;
	float OA;
	OA = sqrt((x - a3) * (x - a3) + y * y);
	angle1 = atan(y / (x - a3));
	cos_angle2 = (OA * OA + a1 * a1 - a2 * a2) / (2 * a1 * OA);
	angle2 = acos(cos_angle2);
	theta1 = M_PI * 5 / 6 - angle1 - angle2;
	theta1 = theta1 * 180 / M_PI;

	cos_angle3 = (a1 * a1 + a2 * a2 - OA * OA) / (2 * a1 * a2);
	angle3 = acos(cos_angle3);
	theta2 = angle3 - M_PI / 6;
	theta2 = theta2 * 180 / M_PI;

	angle5 = M_PI - angle2 - angle3;
	angle6 = atan((x - a3) / y);
	if (y >= 0) angle4 = M_PI * 3 / 2 - angle5 - angle6;
	else angle4 = -angle5 - angle6 + M_PI / 2;
	theta3 = M_PI * 4 / 3 - angle4;
	theta3 = theta3 * 180 / M_PI * 13 / 90;

	/* //原代码
	if (flag == 0)
	{
	x = x * 0.95;
	y = y * 0.8;
	}
	else
	{
	x = x * 0.95;
	y = y * 0.8;

	}
	cout << "x  " << x << " y  " << y << endl;
	float beta, cos_beta, alpha, cos_alpha, gama, cos_gama, theta1, theta2;
	c = sqrt(a2 * a2 + a3 * a3 - 2 * a2 * a3 * cos(M_PI - angle));
	cos_beta = (x * x + y * y + a1 * a1 - c * c) / (2 * a1 * sqrt(x * x + y * y));
	beta = acos(cos_beta);
	if (y >= 0)
	{
	theta1 = atan(x / y) - beta;
	}
	else theta1 = M_PI + atan(x / y) - beta;
	theta1 = theta1 * 180 / M_PI + 60;
	cos_alpha = (a1 * a1 + c * c - x * x - y * y) / (2 * a1 * c);
	alpha = acos(cos_alpha);
	cos_gama = (a2 * a2 + c * c - a3 * a3) / (2 * a2 * c);
	gama = acos(cos_gama);
	theta2 = M_PI - alpha - gama;
	theta2 = 150 - theta2 * 180 / M_PI;
	*/


	int n1, n2, n3;
	n1 = theta1;
	n2 = theta2;
	n3 = theta3;
	if (flag == 0)
	{
		n1 = n1 * 0.82;
		n2 = n2 * 0.85;
		n3 = n3 * 0.6;
	}
	else
	{
		n1 = n1 * 0.82;
		n2 = n2 * 0.85;
		n3 = n3 * 0.6;
	}
	cout << "n1 " << n1 << endl;
	cout << "n2 " << n2 << endl;
	cout << "n3 " << n3 << endl;

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
	//张合 -> XYZ 同步 //1;
	thread m2(Move, 2, '-', n2+17, 3, 0, 0, &shoulder3);
	thread m3(Move, 1, '+', n1, 3, 0, 0, &shoulder2);
	thread m7(Move, 3, '-', n3, 3, 0, 0, &shoulder4);
	thread m5(Move, 5, '+', 25, 3, 0, 0, &shoulder5);
  	m2.join();
	m3.join();
	m7.join();
	m5.join();
	//-抓 逆时针
	//Move(5, '+', 18);	
	//Move(2, '-', n2+2);
	//Move(1, '+', n1+1);

	//Move(5, '+', 25);
	Move(5, '-', 32);

	//Move(1, '-', n1+2);
	//Move(2, '+', n2+2);
	//微调
	if (n1 % 2 == 1)
	{
		thread m4(Move, 1, '-', n1 / 2 + 1, 3, 0, 0, &shoulder2);
		m4.join();
	}
	else
	{
		thread m4(Move, 1, '-', n1 / 2, 3, 0, 0, &shoulder2);
		m4.join();
	}
	//返回
	thread m6(Move, 1, '-', n1 / 2, 3, 0, 0, &shoulder2);
	thread m9(Move, 2, '+', n2 + 17, 3, 0, 0, &shoulder3);
	thread m8(Move, 3, '-', n3, 3, 0, 0, &shoulder4);
	m6.join();
	m9.join();
	m8.join();
	//Move(1, '+', 1);

	//Move(8, '-', 1);
	//复位
	if (flag == 0)
	{
		Move(8, '-', t);
	}
	else Move(8, '+', t);
	

	//m4.join();
	//m5.join();
}
