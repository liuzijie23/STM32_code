#include "math.h"
#include "pid.h"
#include "stm32f10x.h"



/*
typedef struct
{
	float Offset;	  //����ƫ����
	float CurPos;
	float PrevPos;
	float CurAcc;
	float PrevSpeed;

	volatile float SetXPos;	  //�趨λ��
	volatile float SetYPos;	  //�趨λ��
	volatile float SetSpeed;  //�趨�ٶ�
	
	volatile float CurXPos;	  //��ǰλ��
	volatile float CurYPos;	  //��ǰλ��
	volatile float CurSpeed;  //��ǰ�ٶ�ʸ��

	volatile int32_t  PWM;	      //PWM
	volatile uint8_t  ShootFlag;
	volatile uint8_t  AdjustFlag;
	volatile uint8_t  ErrFlag;

	volatile uint32_t SetMaxPos;	  //����趨���λ��
	volatile uint32_t SetMaxPower;	  //����趨�������
	volatile int32_t  SetMaxSpeed;	  //����趨����ٶ�
		
}M1TypeDef,M2TypeDef;

*/

typedef struct
{
	float  SetPoint; 	//  �趨Ŀ�� Desired Value 
	double  SumError;		//	����ۼ� 
		
	float  Proportion;      //  �������� Proportional Const 
	float  Integral;        //  ���ֳ��� Integral Const
	float  Derivative;      //  ΢�ֳ��� Derivative Const

	float LastError;     //  Error[-1]
	float PrevError;     //  Error[-2]

}PIDTypdDef;

//extern M1TypeDef  M1;
//extern M2TypeDef  M2;

PIDTypdDef M1PID;
PIDTypdDef M2PID;
/*------------------------------------------
 ��������:��ʼ��M1PID�ṹ�����
 ����˵��:			
------------------------------------------*/
void PIDM1_Init(void)
{
    M1PID.LastError  = 0;			//Error[-1]
    M1PID.PrevError  = 0;			//Error[-2]
	M1PID.Proportion = 0.1;			//�������� Proportional Const
    M1PID.Integral   = 0.1;			//���ֳ��� Integral Const
    M1PID.Derivative = 0.1;			//΢�ֳ��� Derivative Const
    M1PID.SetPoint   = 0;
	M1PID.SumError   = 0;
}
/*------------------------------------------
 ��������:��ʼ��M2PID�ṹ�����
 ����˵��:			
------------------------------------------*/
void PIDM2_Init(void)
{
    M2PID.LastError  = 0;			//Error[-1]
    M2PID.PrevError  = 0;			//Error[-2]
	M2PID.Proportion = 10;			//�������� Proportional Const
    M2PID.Integral   = 100;			//���ֳ��� Integral Const
    M2PID.Derivative = 5;			//΢�ֳ��� Derivative Const
    M2PID.SetPoint   = 0;
	M2PID.SumError   = 0;
}
/*------------------------------------------
 ��������:����M1PID����ֵ
 ����˵��:			
------------------------------------------*/
void PIDM1_SetPoint(float setpoint)
{	
	M1PID.SetPoint = setpoint;	
}
/*------------------------------------------
 ��������:����M2����ֵ
 ����˵��:			
------------------------------------------*/
void PIDM2_SetPoint(float setpoint)
{	
	M2PID.SetPoint = setpoint;	
}
/*------------------------------------------
 ��������:����M1PID����ϵ��
 ����˵��:������			
------------------------------------------*/
void PIDM1_SetKp(float dKpp)
{	
	M1PID.Proportion = dKpp;	
}
/*------------------------------------------
 ��������:����M2����ϵ��
 ����˵��:������			
------------------------------------------*/
void PIDM2_SetKp(float dKpp)
{	
	M2PID.Proportion = dKpp;	
}
/*------------------------------------------
 ��������:����M1PID����ϵ��
 ����˵��:������			
------------------------------------------*/
void PIDM1_SetKi(float dKii)
{	
	M1PID.Integral = dKii;	
}
/*------------------------------------------
 ��������:����M2����ϵ��
 ����˵��:������			
------------------------------------------*/
void PIDM2_SetKi(float dKii)
{	
	M2PID.Integral = dKii;	
}
/*------------------------------------------
 ��������:����M1PID΢��ϵ��
 ����˵��:������			
------------------------------------------*/
void PIDM1_SetKd(float dKdd)
{	
	M1PID.Derivative = dKdd;
}
/*------------------------------------------
 ��������:����M2΢��ϵ��
 ����˵��:������			
------------------------------------------*/
void PIDM2_SetKd(float dKdd)
{	
	M2PID.Derivative = dKdd;
}
/*------------------------------------------
 ��������:���1λ��ʽPID����
 ����˵��:		
------------------------------------------*/
int32_t PIDM1_PosLocCalc(float NextPoint)
{
    register float  iError,dError;

	iError = M1PID.SetPoint - NextPoint;        // ƫ��
	M1PID.SumError += iError;				    // ����
	if(M1PID.SumError > 2300.0)					//�����޷�2300
		M1PID.SumError = 2300.0;
	else if(M1PID.SumError < -2300.0)
		M1PID.SumError = -2300.0;	
	dError = iError - M1PID.LastError; 			// ��ǰ΢��
	M1PID.LastError = iError;
	
	return(int32_t)(  M1PID.Proportion * iError           	// ������
          		    + M1PID.Integral   * M1PID.SumError 		// ������
          		    + M1PID.Derivative * dError);
}

/*------------------------------------------
 ��������:���2λ��ʽPID����
 ����˵��:			
------------------------------------------*/
int32_t PIDM2_PosLocCalc(float NextPoint)
{
	register float  iError,dError;

	iError = M2PID.SetPoint - NextPoint;        // ƫ��
	M2PID.SumError += iError;
	if(M2PID.SumError > 2300.0)					//�����޷�
		M2PID.SumError = 2300.0;
	else if(M2PID.SumError < -2300.0)
		M2PID.SumError = -2300.0;
	dError = iError - M2PID.LastError; 			// ��ǰ΢��
	M2PID.LastError = iError;
	
	return(int32_t)(  M2PID.Proportion * iError           	// ������
          		    + M2PID.Integral   * M2PID.SumError 		// ������
          		    + M2PID.Derivative * dError);
}
