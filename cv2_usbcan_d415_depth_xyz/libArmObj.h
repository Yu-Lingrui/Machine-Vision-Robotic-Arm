//#pragma once
//// libArmObj.cpp : 定义 DLL 应用程序的导出函数。
//
//#include <stdio.h>
//#include <malloc.h>
//#include <math.h>
//#include <string.h>
//
//
//#define pi (3.1415926535897932384626433832795)
//
//#define CONST_HEIGHT_0 (70);      // 垂直长度参数
//#define CONST_HEIGHT_1 (630);     // 520
//#define CONST_HEIGHT_2 (500);     // 600
//#define CONST_HEIGHT_3 (69.5);
//#define CONST_HEIGHT_4 (400-69.5);
//
//#define CONST_DISTANCE_0 (107.5); // 水平长度参数
//#define CONST_DISTANCE_1 (82.5);
//#define CONST_DISTANCE_2 (-103.5);
//#define CONST_DISTANCE_3 (-15);
//#define CONST_DISTANCE_4 (0);
//
//#define CONST_LOW_BOUND_0 (-180); //角度范围
//#define CONST_HIGH_BOUND_0 (180);
//#define CONST_LOW_BOUND_1 (-180);
//#define CONST_HIGH_BOUND_1 (180);
//#define CONST_LOW_BOUND_2 (-180);
//#define CONST_HIGH_BOUND_2 (180);
//#define CONST_LOW_BOUND_3 (-180);
//#define CONST_HIGH_BOUND_3 (180);
//#define CONST_LOW_BOUND_4 (-180);
//#define CONST_HIGH_BOUND_4 (180);
//
//typedef struct _AngleRange
//{
//	double lowBound;
//	double highBound;
//}AngleRange;
//
//
//typedef struct _HSArm
//{
//	double height0;
//	double height1;
//	double height2;
//	double height3;
//	double height4;
//
//	double distance0;
//	double distance1;
//	double distance2;
//	double distance3;
//	double distance4;
//
//	AngleRange angle0;
//	AngleRange angle1;
//	AngleRange angle2;
//	AngleRange angle3;
//	AngleRange angle4;
//	AngleRange angle5;
//}HSArm;
//
//
//HSArm *createArmObj()
//{
//	HSArm *armObj = (HSArm *)malloc(sizeof(HSArm));
//
//	if (armObj==NULL)
//		return NULL;
//
//
//	armObj->height0 = CONST_HEIGHT_0;
//	armObj->height1 = CONST_HEIGHT_1;
//	armObj->height2 = CONST_HEIGHT_2;
//	armObj->height3 = CONST_HEIGHT_3;
//	armObj->height4 = CONST_HEIGHT_4;
//
//	armObj->distance0 = CONST_DISTANCE_0;
//	armObj->distance1 = CONST_DISTANCE_1;
//	armObj->distance2 = CONST_DISTANCE_2;
//	armObj->distance3 = CONST_DISTANCE_3;
//	armObj->distance4 = CONST_DISTANCE_4;
//
//	armObj->angle0.lowBound = CONST_LOW_BOUND_0;
//	armObj->angle0.highBound = CONST_HIGH_BOUND_0;
//	armObj->angle1.lowBound = CONST_LOW_BOUND_1;
//	armObj->angle1.highBound = CONST_HIGH_BOUND_1;
//	armObj->angle2.lowBound = CONST_LOW_BOUND_2;
//	armObj->angle2.highBound = CONST_HIGH_BOUND_2;
//	armObj->angle3.lowBound = CONST_LOW_BOUND_3;
//	armObj->angle3.highBound = CONST_HIGH_BOUND_3;
//	armObj->angle4.lowBound = CONST_LOW_BOUND_4;
//	armObj->angle4.highBound = CONST_HIGH_BOUND_4;
//
//
//	return armObj;
//}
//
//
//void releaseArmObj(HSArm *armObj)
//{
//	if (armObj)
//		free(armObj);
//}
//
//
//int getArmAngleWithPositon(HSArm *armObj, int q2anglePara, double px, double py, double pz, double qx, double qz, double *q0s, double *q1s, double *q2s, double *q3s, double *q4s, double *qy)
//{
//	double h0 = armObj->height0;
//	double h1 = armObj->height1;
//	double h2 = armObj->height2;
//	double h3 = armObj->height3;
//	double h4 = armObj->height4;
//
//	double l0 = armObj->distance0;
//	double l1 = armObj->distance1;
//	double l2 = armObj->distance2;
//	double l3 = armObj->distance3;
//	double l4 = armObj->distance4;
//
//	if((!q0s) || (!q1s) || (!q2s) || (!q3s) || (!q4s) || (!qy))
//	{
//		return -30;
//	}
//	
//	double lengthArmExt=pow(px*px+py*py+pz*pz,0.5);
//	double maxRange=h0+h1+h2+h3+h4+l0+l1+l2+l3+l4;
//	double heightLenth=h0+h1+h2+h3+h4;
//	double levelLength=l0+l1+l2+l3+l4;
//	if (lengthArmExt>maxRange)
//		return -32;
//
//	
//	double Rmatrix[3][3];
//	double nx, ox, ax, ny, oy, ay, nz, oz, az;
//	double q0, q1 ,q2 ,q3, q4, q123; //q123=q1+q2+q3
//
//	double LengPara=- levelLength*levelLength + px*px+py*py; //该值应当大于0，levelLength为机械臂水平面能到达的最小值
//	if (pow(px*px+py*py,0.5)<levelLength && pow(px*px+py*py,0.5)>=levelLength-0.000001)
//		LengPara=0;
//	else if (pow(px*px+py*py,0.5)<levelLength-0.000001)  // 机械臂末端的xy应当在机械臂空间的水平面最小圆约束外侧
//		return -33;
//
//	if (pow(px*px+py*py+pz*pz,0.5)>pow(levelLength*levelLength+heightLenth*heightLenth+0.000001, 0.5)) // 机械臂末端应当小与机械臂空间能够到达的最大值
//		return -34;
//
//
//	//*q4s=qx;
//	q4=qx/180*pi;
//
//	q123=qz/180*pi; //q1+q2+q3
//
//
//	double q0tem;
//
//
//	if (q123>=0) //通过判断，解决求解角度符号和象限问题
//	{
//
//		q0=asin(-(levelLength*px - py*pow(LengPara,0.5))/(px*px+py*py)); //逆运动学通过左乘求解
//		q0tem=q0;
//		if (px<-levelLength && py>0) //通过判断，解决求解角度符号和象限问题
//		{
//			q0=pi-q0;
//			q0tem=-q0tem;
//		}
//		else if (px<levelLength && py<=0)
//		{
//			q0=-pi-q0;
//			q0tem=-q0tem;
//		}
//		
//		if (q0>pi)  //通过判断，解决求解角度符号和象限问题
//			q0=q0-2*pi;
//		else if (q0<-pi)
//			q0=q0+2*pi;
//
//
//
//	}
//	else if (q123<0)
//	{
//		q0=-asin((levelLength*px + py*pow(LengPara,0.5))/(px*px+py*py));
//		q0tem=q0;
//		if (px>-levelLength && py<0)
//		{
//			q0=pi-q0;
//			q0tem=-q0tem;
//		}
//		else if (px>levelLength && py>=0)
//		{
//			q0=-pi-q0;
//			q0tem=-q0tem;
//		}
//		
//		if (q0>pi)
//			q0=q0-2*pi;
//		else if (q0<-pi)
//			q0=q0+2*pi;
//
//
//
//	}
//
//
///*
//	if (q123>=0)
//	{
//
//		q0=asin(-(levelLength*px - py*pow(LengPara,0.5))/(px*px+py*py));
//
//		q0degreeTem=q0*180/pi;
//
//		if (px<-levelLength && py>0)
//			q0degreeTem=180-q0degreeTem;
//		else if (px<levelLength && py<=0)
//			q0degreeTem=-180-q0degreeTem;
//		
//		if (q0degreeTem>180)
//			q0degreeTem=q0degreeTem-360;
//		else if (q0degreeTem<-180)
//			q0degreeTem=q0degreeTem+360;
//	}
//	else if (q123<0)
//	{
//		q0=-asin((levelLength*px + py*pow(LengPara,0.5))/(px*px+py*py));
//
//		q0degreeTem=q0*180/pi;
//
//		if (px>-levelLength && py<0)
//			q0degreeTem=180-q0degreeTem;
//		else if (px>levelLength && py>=0)
//			q0degreeTem=-180-q0degreeTem;
//		
//		if (q0degreeTem>180)
//			q0degreeTem=q0degreeTem-360;
//		else if (q0degreeTem<-180)
//			q0degreeTem=q0degreeTem+360;
//
//
//
//	}
//
//
//*/
//
//
//
//
//	//*q0s=q0*180/pi;
//
//
///*
//	q0=atan(py/pxe);
//
//	if (px<0 && py>0)
//		q0=q0+pi;
//	else if (px<0 && py<=0)
//		q0=q0-pi;
//
//	*q0s=q0*180/pi;
//*/
//
//
//
//
//	*qy=q0*180/pi;
//
//	double qzrad=qz/180*pi;  // q123
//	double qyrad=*qy/180*pi;  // q0
//	double qxrad=qx/180*pi;  // q4
//
//	Rmatrix[0][0] = -sin(qxrad)*sin(qyrad) - cos(qxrad)*cos(qyrad)*sin(qzrad - pi/2);  //逆运动学左乘得到矩阵的中间变量
//	Rmatrix[0][1] = cos(qyrad)*sin(qxrad)*sin(qzrad - pi/2) - cos(qxrad)*sin(qyrad);
//	Rmatrix[0][2] = cos(qyrad)*cos(qzrad - pi/2);
//	Rmatrix[1][0] = cos(qyrad)*sin(qxrad) - cos(qxrad)*sin(qyrad)*sin(qzrad - pi/2);
//	Rmatrix[1][1] = cos(qxrad)*cos(qyrad) + sin(qxrad)*sin(qyrad)*sin(qzrad - pi/2);
//	Rmatrix[1][2] = cos(qzrad - pi/2)*sin(qyrad);
//	Rmatrix[2][0] = -cos(qxrad)*cos(qzrad - pi/2);
//	Rmatrix[2][1] = cos(qzrad - pi/2)*sin(qxrad);
//	Rmatrix[2][2] = -sin(qzrad - pi/2);
//
///*
//	for (int i=0; i<3; i++)
//	{
//		for (int j=0; j<3; j++)
//		{
//			if (abs(Rmatrix[i][j])<0.000001)
//			{
//				Rmatrix[i][j]=0;
//			}
//		}
//	}
//*/
//
//	nx=Rmatrix[0][0];
//	ox=Rmatrix[0][1];
//	ax=Rmatrix[0][2];
//	ny=Rmatrix[1][0];
//	oy=Rmatrix[1][1];
//	ay=Rmatrix[1][2];
//	nz=Rmatrix[2][0];
//	oz=Rmatrix[2][1];
//	az=Rmatrix[2][2];
//
//	/*
//	double axe;
//	if (abs(ax)<0.0000001)
//	{
//		axe=ax+0.0000001;
//	}
//	else
//	{
//		axe=ax;
//	}
//
//	double test1=ay/axe;
//	double test2 = atan(0.001);
//
//
//
//
//	//if (ax!=0)
//	//{
//		q0=atan(ay/axe);
//	//}
//
//
//	if (abs(ay)<0.000001 && abs(ax)<0.000001)
//	{
//		if (abs(ox)<0.000001)
//		{
//			q0=0; //0
//		}
//		else if (abs(1-ox)<0.000001)
//		{
//			q0=-pi/2+pi; //	-90
//		}
//		else if (abs(ox+1)<0.000001)
//		{
//			q0=pi/2-pi; //90
//		}
//	}
//
//*/
//	double t11=cos(q0);
//	double t22=sin(q0);
//
//	double cq123=az; //逆运动学求解 cos(q1+q2+q3)
//	//double sq123=ax*cos(q0) + ay*sin(q0);
//	double sq123=ax*cos(q0tem) + ay*sin(q0tem);  //逆运动学求解 sin(q1+q2+q3)
//
//
//
///*
//	q123=acos(az);
//
//
//	if (rotatePara==1)
//	{
//		if (ax<0)
//		{
//			q123=-q123;
//		}
//	}
//	else if (rotatePara==0)
//	{
//		if (ax>0)
//		{
//			q123=-q123;
//		}
//
//		if (ax<0 && ay<0 || ax>0 && ay>0)
//		{
//			q0=q0-pi;
//		}
//		else if (ax<0 && ay>0 || ax>0 && ay<0)
//		{
//			q0=q0+pi;    
//		}
//	}
//
//
//	*q0s=q0*180/pi;
//	double q123s=q123*180/pi;
//
//	double q4sin = asin(ny*cos(q0) - nx*sin(q0));
//	double q4cos = acos(oy*cos(q0) - ox*sin(q0));
//	q4=q4cos;
//	if (q4sin<0)
//	{
//		q4=-q4cos;
//	}
//
//	*q4s=q4*180/pi;
//	*/
//
//	double q0t=q0;
//	int	sflagq1=1;
//	int	sflagq2=1;
//
//	if (q0>pi/2)//通过判断，解决求解角度符号和象限问题
//	{
//		q0t=q0-pi;
//		sflagq1=-1;
//		sflagq2=-1;
//	}
//	else if (q0<-pi/2)
//	{
//		q0t=q0+pi;
//		sflagq1=-1;
//		sflagq2=-1;
//	}
//
//	double sq1234= cos(q4)*sq123 + cq123*sin(q4); //逆运动学左乘得到的变量 sin(q1+q2+q3+q4)
//	double sq123m4 = cos(q4)*sq123 - cq123*sin(q4);  // sin(q1+q2+q3-q4)
//
//	double cq1234 = cos(q4)*cq123 - sin(q4)*sq123;
//	double cq123m4=cos(q4)*cq123 + sin(q4)*sq123;
//	double para1= - (l4*sq1234)/2  - (l4*sq123m4)/2 + h3*cq123 + h4*cq123-(pz - h0);
//	double para2=(l4*cq1234)/2 + (l4*cq123m4)/2 + h3*sq123 + h4*sq123-(px*cos(q0t) + py*sin(q0t));
//
//	double pat1=- pow(h1, 2) + 2*h1*h2 - pow(h2, 2) + pow(para1, 2) + pow(para2, 2);
//	double pat2=pow(h1, 2) + 2*h1*h2 + pow(h2, 2) - pow(para1, 2) - pow(para2, 2);
//	
//	double errorPara=0.0000001;
//	
//	if (abs(pat1)<errorPara) //开方无法计算的情况
//		pat1=0;
//	if (abs(pat2)<errorPara)
//		pat2=0;
//
//	if (abs(para1)<errorPara)
//		para1=0;
//	if (abs(para2)<errorPara)
//		para2=0;
//
//	if (pat2<0 && pat2>-0.001) //解决pat2很小时，q123=0时的情况
//		pat2=0;
//	else if (pat2<-0.001)
//		return -31;
//
//	//double pat12=(pat1)*(pat2);
//	//double pat12s=pow(pat12, 0.5);
//	double prm0=2*h1*para2;  //逆运动学的中间变量，通过MATALB求解方程组得到q1tem等变量。prm0-7这些变量为方程组中的变量。最终计算得到q1 q2的值
//	double prm1=(pow(h1, 2)*pow((pat1)*(pat2), 0.5))/(pat1);
//	double prm2=(pow(h2, 2)*pow((pat1)*(pat2), 0.5))/(pat1);
//	double prm3=(pow(para1, 2)*pow((pat1)*(pat2), 0.5))/(pat1);
//	double prm4=(pow(para2, 2)*pow((pat1)*(pat2), 0.5))/(pat1);
//	double prm5=(2*h1*h2*pow((pat1)*(pat2), 0.5))/(pat1);
//	double prm6=(pow(h1, 2) - 2*h1*para1 - pow(h2, 2) + pow(para1, 2) + pow(para2, 2));
//	double prm7=pow((pat1)*(pat2), 0.5)/(pat1);
//
//	double q1tem[2], q2tem[2];
//	q1tem[0]=sflagq1*(-2*atan((prm0 - prm1 - prm2 + prm3 + prm4 + prm5)/prm6)); //MATLAB求解得到方程的解（方程由逆运动学得到）
//	q1tem[1]=sflagq1*(-2*atan((prm0 + prm1 + prm2 - prm3 - prm4 - prm5)/prm6));
//
//	q2tem[0]=sflagq2*(2*atan(prm7));
//	q2tem[1]=sflagq2*(-2*atan(prm7));
//
//
//	double t1, t2, t3, t4;
//	t1=q1tem[0]*180/pi;
//	t2=q1tem[1]*180/pi;
//	t3=q2tem[0]*180/pi;
//	t4=q2tem[1]*180/pi;
//
//
//	int q2angleComparsion;
//	if (q2tem[0]>=0) //通过判断，解决求解角度符号和象限问题
//	{
//		q2angleComparsion=1;
//	}
//	else if  (q2tem[0]<0)
//	{
//		q2angleComparsion=0;  
//	}
//
//	if (q2anglePara==q2angleComparsion) //通过判断，解决求解角度符号和象限问题
//	{
//		q2=q2tem[0];
//		q1=q1tem[0];
//	}
//	else if (q2anglePara!=q2angleComparsion)
//	{
//		q2=q2tem[1];
//		q1=q1tem[1];
//	}
//
//	q3=q123-q1-q2;
//
//	if (q3<-pi) //通过判断，解决求解角度符号和象限问题
//		q3=q3+2*pi;
//	else if (q3>pi)
//		q3=q3-2*pi;
//	
//	*q0s=q0*180/pi;
//	*q1s=q1*180/pi;
//	*q2s=q2*180/pi;
//	*q3s=q3*180/pi;
//	*q4s=q4*180/pi;
//
//	if ((*q0s<armObj->angle0.lowBound) || (*q0s>armObj->angle0.highBound))
//	{		
//		return -31;
//	}
//	if ((*q1s<armObj->angle1.lowBound) || (*q1s>armObj->angle1.highBound))
//	{		
//		return -32;
//	}
//	if ((*q2s<armObj->angle2.lowBound) || (*q2s>armObj->angle2.highBound))
//	{		
//		return -33;
//	}
//	if ((*q3s<armObj->angle3.lowBound) || (*q3s>armObj->angle3.highBound))
//	{		
//		return -34;
//	}
//	if ((*q4s<armObj->angle4.lowBound) || (*q4s>armObj->angle4.highBound))
//	{		
//		return -35;
//	}
//
//
//	return 0;
//}
