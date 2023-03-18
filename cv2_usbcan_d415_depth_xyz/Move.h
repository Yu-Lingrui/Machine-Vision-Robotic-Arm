#pragma once

#include"ControlCAN.h"
#include<vcruntime_string.h>
#include<Windows.h>
#include<iostream>

//****************************************************************************************************//
typedef unsigned char uint8;
typedef struct shoulder_ctr
{
	int curr_angle;			//��ǰ�Ƕ�(-359~359)
	int left_angle;			//���޽Ƕ�(��û������ʱ����Ϊ-360)
	int right_angle;		//�Ҽ��޽Ƕ�(��û���Ҽ���ʱ����Ϊ360)
	int set_angle;			//�ؽڽṹ�帴λ�Ƕ�
}SHOULDER_ctr, * _SHOULDER_ctr;

//�ؽڽṹ����λ�͸�λȫ�ֱ���
SHOULDER_ctr shoulder1 = { 0, -60, 60, 0 };	//����ת(�ؽ�8)
SHOULDER_ctr shoulder2 = { 0, -75, 90, -67 };	//��ڶ�(�ؽ�1)
SHOULDER_ctr shoulder3 = { 0, 60, 160, 130 };	//��1(�ؽ�2)
SHOULDER_ctr shoulder4 = { 0, -80, 80, -67 };	//��2(�ؽ�3)
SHOULDER_ctr shoulder5 = { 0, 6, 90, 30 };	//צ�ź�(�ؽ�5)

//****************************************************************************************************//
const unsigned char CRC8Table[256] = {
	0, 94, 188, 226, 97, 63, 221, 131, 194, 156, 126, 32, 163, 253, 31, 65,
	157, 195, 33, 127, 252, 162, 64, 30, 95, 1, 227, 189, 62, 96, 130, 220,
	35, 125, 159, 193, 66, 28, 254, 160, 225, 191, 93, 3, 128, 222, 60, 98,
	190, 224, 2, 92, 223, 129, 99, 61, 124, 34, 192, 158, 29, 67, 161, 255,
	70, 24, 250, 164, 39, 121, 155, 197, 132, 218, 56, 102, 229, 187, 89, 7,
	219, 133, 103, 57, 186, 228, 6, 88, 25, 71, 165, 251, 120, 38, 196, 154,
	101, 59, 217, 135, 4, 90, 184, 230, 167, 249, 27, 69, 198, 152, 122, 36,
	248, 166, 68, 26, 153, 199, 37, 123, 58, 100, 134, 216, 91, 5, 231, 185,
	140, 210, 48, 110, 237, 179, 81, 15, 78, 16, 242, 172, 47, 113, 147, 205,
	17, 79, 173, 243, 112, 46, 204, 146, 211, 141, 111, 49, 178, 236, 14, 80,
	175, 241, 19, 77, 206, 144, 114, 44, 109, 51, 209, 143, 12, 82, 176, 238,
	50, 108, 142, 208, 83, 13, 239, 177, 240, 174, 76, 18, 145, 207, 45, 115,
	202, 148, 118, 40, 171, 245, 23, 73, 8, 86, 180, 234, 105, 55, 213, 139,
	87, 9, 235, 181, 54, 104, 138, 212, 149, 203, 41, 119, 244, 170, 72, 22,
	233, 183, 85, 11, 136, 214, 52, 106, 43, 117, 151, 201, 74, 20, 246, 168,
	116, 42, 200, 150, 21, 75, 169, 247, 182, 232, 10, 84, 215, 137, 107, 53
};

UCHAR GetCRC8(unsigned char* ptr, unsigned int len)
{
	unsigned char crc8 = 0;
	for (; len > 0; len--)
	{
		crc8 = CRC8Table[crc8 ^ *ptr];
		ptr++;
	}
	return(crc8);
}

//unsigned char CHECKCRC8(unsigned char* ptr, unsigned int len)
//{
//	unsigned char crc8 = 0;
//	for (; len > 0; len--)
//	{
//		crc8 = CRC8Table[crc8 ^ *ptr];
//		ptr++;
//	}
//	if (crc8)//�����Ϊ��,�д���
//	{
//		return 0;
//	}
//	return 1;
//}

void Move(int id, char dir, int times, int nDeviceType = 3, int nDeviceInd = 0, int nCANIndint = 0,
	_SHOULDER_ctr shoulder = &shoulder1)
{
	unsigned char cmd[6] = { 'A', 'M', dir, 0x32, 0x04 , 0 }; // 'A','M', dir, 50, 4 , 0;
	cmd[5] = GetCRC8(cmd, 5);
	std::cout << cmd << std::endl;
	/*if (CHECKCRC8(cmd, 6))
	{
		std::cout << "CRC��ȷ" << std::endl;
	}*/
	VCI_CAN_OBJ frameinfo;
	//VCI_CAN_OBJ frame;
	frameinfo.DataLen = 6;
	memcpy(&frameinfo.Data, cmd, 6);
	frameinfo.RemoteFlag = 0;
	frameinfo.ExternFlag = 0;
	frameinfo.ID = id;
	frameinfo.SendType = 0;

	for (int i = 1; i <= times; i++)
	{
		//VCI_Transmit(nDeviceType, nDeviceInd, nCANIndint, &frameinfo, 1);
		if (VCI_Transmit(nDeviceType, nDeviceInd, nCANIndint, &frameinfo, 1) == 1)
		{
			std::cout << "д��ɹ�" << std::endl;
		}
		else
		{
			std::cout << "д��ʧ��" << std::endl;
		}
		/********************************************************************************************/
		//if (VCI_Receive(nDeviceType, nDeviceInd, nCANIndint, &frame, 1, 1))// ���̵߳��ñ��������:-1!(���ǲ���!)
		//{
		//	std::cout << "��ȡ�ɹ�" << std::endl;
		//	unsigned char* pdata = &frame.Data[0];
		//	if ((frame.DataLen >= 5) && (pdata[0] == 'G') && (pdata[1] == 'S'))//����ģʽ
		//	{
		//		short temp_value = 0;
		//		int m_CurrPos;
		//		/*int NodeID = frame.ID - 0x20;
		//		std::cout << "�ؽ�ID�� " << NodeID << std::endl;*/
		//		temp_value = pdata[2] + (pdata[3] << 8);
		//		m_CurrPos = (int)temp_value / 64;
		//		//std::cout << "�Ƕȣ� " << m_CurrPos << std::endl;
		//		shoulder->curr_angle = m_CurrPos;	 //�Ƕ���Ϣ
		//	}
		//	else
		//	{
		//		std::cout << "��ȡʧ��" << std::endl;
		//	}
		//}
		/********************************************************************************************/
		//�Զ��滮ʱ������λ����ֹͣ
		if ((shoulder->curr_angle <= shoulder->left_angle) || (shoulder->curr_angle >= shoulder->right_angle))
		{
			break;
		}
		Sleep(200);
	}

}


//void move(int id, char dir, int times, int nDeviceType = 3, int nDeviceInd = 0, int nCANIndint = 0)
//{
//	unsigned char cmd[6] = { 'A', 'M', dir, 0x64, 0x04 , 0 }; // 'A','M', dir, 100, 4 , 0;
//	cmd[5] = GetCRC8(cmd, 5);
//	std::cout << cmd << std::endl;
//	VCI_CAN_OBJ frameinfo;
//	frameinfo.DataLen = 6;
//	memcpy(&frameinfo.Data, cmd, 6);
//	frameinfo.RemoteFlag = 0;
//	frameinfo.ExternFlag = 0;
//	frameinfo.ID = id;
//	frameinfo.SendType = 0;
//
//	for (int i = 1; i <= times; i++)
//	{
//		if (VCI_Transmit(nDeviceType, nDeviceInd, nCANIndint, &frameinfo, 1) == 1)
//		{
//			std::cout << "д��ɹ�" << std::endl;
//		}
//		else
//		{
//			std::cout << "д��ʧ��" << std::endl;
//		}
//		Sleep(200);
//	}
//  
//}
//�����߳�ѭ��һֱ��ѯ�����ؽڽǶ�,������߳�ֻ��˳��ִ�л��岻ͬ������� 
void Rcv(int nDeviceType = 3, int nDeviceInd = 0, int nCANIndint = 0)
{	
	/*******************************************************************************/
	unsigned char _cmd[6] = { 'A', 'M', '+', 0x01, 0x04 , 0}; // ��ѯ�Ƕ�
	_cmd[5] = GetCRC8(_cmd, 5);
	VCI_CAN_OBJ frame;
	VCI_CAN_OBJ Frame;
	frame.DataLen = 6;
	memcpy(&frame.Data, _cmd, 6);
	frame.RemoteFlag = 0;
	frame.ExternFlag = 0;
	frame.ID = 8;
	frame.SendType = 0;
	unsigned char* _pdata = &Frame.Data[0];
	VCI_Transmit(nDeviceType, nDeviceInd, nCANIndint, &frame, 1);
	VCI_Receive(nDeviceType, nDeviceInd, nCANIndint, &Frame, 1, 10);
	shoulder1.curr_angle = (int)(_pdata[2] + (_pdata[3] << 8)) / 64;
	Sleep(20);
	frame.ID = 1;
	VCI_Transmit(nDeviceType, nDeviceInd, nCANIndint, &frame, 1);
	VCI_Receive(nDeviceType, nDeviceInd, nCANIndint, &Frame, 1, 10);
	shoulder2.curr_angle = (int)(_pdata[2] + (_pdata[3] << 8)) / 64;
	Sleep(20);
	frame.ID = 2;
	VCI_Transmit(nDeviceType, nDeviceInd, nCANIndint, &frame, 1);
	VCI_Receive(nDeviceType, nDeviceInd, nCANIndint, &Frame, 1, 10);
	shoulder3.curr_angle = (int)(_pdata[2] + (_pdata[3] << 8)) / 64;
	Sleep(20);
	frame.ID = 3;
	VCI_Transmit(nDeviceType, nDeviceInd, nCANIndint, &frame, 1);
	VCI_Receive(nDeviceType, nDeviceInd, nCANIndint, &Frame, 1, 10);
	shoulder4.curr_angle = (int)(_pdata[2] + (_pdata[3] << 8)) / 64;
	Sleep(20);
	frame.ID = 5;
	VCI_Transmit(nDeviceType, nDeviceInd, nCANIndint, &frame, 1);
	VCI_Receive(nDeviceType, nDeviceInd, nCANIndint, &Frame, 1, 10);
	shoulder5.curr_angle = (int)(_pdata[2] + (_pdata[3] << 8)) / 64;
	/*********************************************************************************/
	while (true)
	{	
		VCI_CAN_OBJ _frame;
		int i;
		int len = 0;
		len = VCI_Receive(nDeviceType, nDeviceInd, nCANIndint, &_frame, 1, 10); // VCI_Receive(3, 0, 0, frame, 50, 200)
		if (len <= 0)
		{
			std::cout << "��ȡʧ��" << std::endl;
		}										 
		else
		{
			std::cout << "��ȡ�ɹ�" << std::endl;
			/*for (i = 0; i < len; i++)
			{*/
				unsigned char* pdata = &_frame.Data[0];
				if ((_frame.DataLen >= 5) && (pdata[0] == 'G') && (pdata[1] == 'S'))//����ģʽ
				{
					short temp_value = 0;
					int m_CurrPos;
					int NodeID = _frame.ID - 0x20;
					temp_value = pdata[2] + (pdata[3] << 8);
					m_CurrPos = (int)temp_value / 64;
					if (NodeID ==8)
					{
						shoulder1.curr_angle = m_CurrPos;
					}
					else if (NodeID == 1)
					{
						shoulder2.curr_angle = m_CurrPos;
					}
					else if (NodeID == 2)
					{
						shoulder3.curr_angle = m_CurrPos;
					}
					else if (NodeID == 3)
					{
						shoulder4.curr_angle = m_CurrPos;
					}
					else if (NodeID == 5)
					{
						shoulder5.curr_angle = m_CurrPos;
					}
					else
					{
						std::cout << "ID����" << std::endl;
					}

				}

			/*}*/
		}
	}
}


