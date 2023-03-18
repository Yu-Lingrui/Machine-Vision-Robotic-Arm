#include<iostream>
#include<thread>
#include<math.h>
#include"Move.h"

using namespace std;

////****************************************************************************************************//
//typedef unsigned char uint8;
//typedef struct shoulder_ctr
//{
//	int curr_angle;			//当前角度(-359~359)
//	int left_angle;			//左极限角度(当没有左极限时设置为-360)
//	int right_angle;		//右极限角度(当没有右极限时设置为360)
//	int set_angle;			//关节结构体复位角度
//}SHOULDER_ctr, * _SHOULDER_ctr;
//
////关节结构体限位和复位全局变量
//SHOULDER_ctr shoulder1 = { 0, -178 * 64, 178 * 64, 0 };	//肩旋转(关节8)
//SHOULDER_ctr shoulder2 = { 0, -88 * 64, 90 * 64, -67 * 64 };	//肩摆动(关节1)
//SHOULDER_ctr shoulder3 = { 0, -60 * 64, 170 * 64, 130 * 64 };	//肘1(关节2)
//SHOULDER_ctr shoulder4 = { 0, -90 * 64, 90 * 64, -67 * 64 };	//肘2(关节3)
//SHOULDER_ctr shoulder5 = { 0, 6, 180, 30 };	//爪张合(关节5)
////****************************************************************************************************//
void run(char dir, SHOULDER_ctr shoulder, int ID)
{
	for (size_t i = 100; i > 0; i--)
	{
		if (dir == '+')
		{
			Move(ID, dir, 1, 3, 0, 0, &shoulder);
			if (shoulder.curr_angle - shoulder.set_angle >= 0)
			{
				break;
			}
		}
		else
		{
			Move(ID, dir, 1, 3, 0, 0, &shoulder);
			if (shoulder.curr_angle - shoulder.set_angle <= 0)
			{
				break;
			}
		}

	}

}


void s_ctr_should_mode(int ID)
{
	switch (ID)
	{
	case 8:
		//Move(8, '+', 1, 3, 0, 0, &shoulder1);
		if (shoulder1.curr_angle < shoulder1.set_angle)
		{
			run('+', shoulder1, 8);
		}
		else
		{
			run('-', shoulder1, 8);
		}
		break;

	case 1:
		//Move(1, '+', 1, 3, 0, 0, &shoulder2);
		if (shoulder2.curr_angle < shoulder2.set_angle)
		{
			run('+', shoulder2, 1);
		}
		else
		{
			run('-', shoulder2, 1);
		}
		break;

	case 2:
		//Move(2, '+', 1, 3, 0, 0, &shoulder3);
		if (shoulder3.curr_angle < shoulder3.set_angle)
		{
			run('+', shoulder3, 2);
		}
		else
		{
			run('-', shoulder3, 2);
		}
		break;

	case 3:
		//Move(3, '+', 1, 3, 0, 0, &shoulder4);
		if (shoulder4.curr_angle < shoulder4.set_angle)
		{
			run('+', shoulder4, 3);
		}
		else
		{
			run('-', shoulder4, 3);
		}
		break;

	case 5:
		//Move(5, '+', 1, 3, 0, 0, &shoulder5);
		if (shoulder5.curr_angle < shoulder5.set_angle)
		{
			run('+', shoulder5, 5);
		}
		else
		{
			run('-', shoulder5, 5);
		}
		break;

	default:
		break;
	}
}


void reset()
{
	thread b8(s_ctr_should_mode, 8);
	//b8.join();
	thread b1(s_ctr_should_mode, 1);
	//b1.join();
	thread b2(s_ctr_should_mode, 2);
	//b2.join();
	thread b3(s_ctr_should_mode, 3);
	//b3.join();
	thread b5(s_ctr_should_mode, 5);
	b8.join();
	b1.join();
	b2.join();
	b3.join();
	b5.join();
}