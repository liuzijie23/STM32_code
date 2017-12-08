#include "math.h"
#include "pid.h"
#include "stm32f10x.h"



/*
typedef struct
{
	float Offset;	  //允许偏差量
	float CurPos;
	float PrevPos;
	float CurAcc;
	float PrevSpeed;

	volatile float SetXPos;	  //设定位置
	volatile float SetYPos;	  //设定位置
	volatile float SetSpeed;  //设定速度
	
	volatile float CurXPos;	  //当前位置
	volatile float CurYPos;	  //当前位置
	volatile float CurSpeed;  //当前速度矢量

	volatile int32_t  PWM;	      //PWM
	volatile uint8_t  ShootFlag;
	volatile uint8_t  AdjustFlag;
	volatile uint8_t  ErrFlag;

	volatile uint32_t SetMaxPos;	  //软件设定最大位置
	volatile uint32_t SetMaxPower;	  //软件设定最大力量
	volatile int32_t  SetMaxSpeed;	  //软件设定最大速度
		
}M1TypeDef,M2TypeDef;

*/

typedef struct
{
	float  SetPoint; 	//  设定目标 Desired Value 
	double  SumError;		//	误差累计 
		
	float  Proportion;      //  比例常数 Proportional Const 
	float  Integral;        //  积分常数 Integral Const
	float  Derivative;      //  微分常数 Derivative Const

	float LastError;     //  Error[-1]
	float PrevError;     //  Error[-2]

}PIDTypdDef;

//extern M1TypeDef  M1;
//extern M2TypeDef  M2;

PIDTypdDef M1PID;
PIDTypdDef M2PID;
/*------------------------------------------
 函数功能:初始化M1PID结构体参数
 函数说明:			
------------------------------------------*/
void PIDM1_Init(void)
{
    M1PID.LastError  = 0;			//Error[-1]
    M1PID.PrevError  = 0;			//Error[-2]
	M1PID.Proportion = 0.1;			//比例常数 Proportional Const
    M1PID.Integral   = 0.1;			//积分常数 Integral Const
    M1PID.Derivative = 0.1;			//微分常数 Derivative Const
    M1PID.SetPoint   = 0;
	M1PID.SumError   = 0;
}
/*------------------------------------------
 函数功能:初始化M2PID结构体参数
 函数说明:			
------------------------------------------*/
void PIDM2_Init(void)
{
    M2PID.LastError  = 0;			//Error[-1]
    M2PID.PrevError  = 0;			//Error[-2]
	M2PID.Proportion = 10;			//比例常数 Proportional Const
    M2PID.Integral   = 100;			//积分常数 Integral Const
    M2PID.Derivative = 5;			//微分常数 Derivative Const
    M2PID.SetPoint   = 0;
	M2PID.SumError   = 0;
}
/*------------------------------------------
 函数功能:设置M1PID期望值
 函数说明:			
------------------------------------------*/
void PIDM1_SetPoint(float setpoint)
{	
	M1PID.SetPoint = setpoint;	
}
/*------------------------------------------
 函数功能:设置M2期望值
 函数说明:			
------------------------------------------*/
void PIDM2_SetPoint(float setpoint)
{	
	M2PID.SetPoint = setpoint;	
}
/*------------------------------------------
 函数功能:设置M1PID比例系数
 函数说明:浮点型			
------------------------------------------*/
void PIDM1_SetKp(float dKpp)
{	
	M1PID.Proportion = dKpp;	
}
/*------------------------------------------
 函数功能:设置M2比例系数
 函数说明:浮点型			
------------------------------------------*/
void PIDM2_SetKp(float dKpp)
{	
	M2PID.Proportion = dKpp;	
}
/*------------------------------------------
 函数功能:设置M1PID积分系数
 函数说明:浮点型			
------------------------------------------*/
void PIDM1_SetKi(float dKii)
{	
	M1PID.Integral = dKii;	
}
/*------------------------------------------
 函数功能:设置M2积分系数
 函数说明:浮点型			
------------------------------------------*/
void PIDM2_SetKi(float dKii)
{	
	M2PID.Integral = dKii;	
}
/*------------------------------------------
 函数功能:设置M1PID微分系数
 函数说明:浮点型			
------------------------------------------*/
void PIDM1_SetKd(float dKdd)
{	
	M1PID.Derivative = dKdd;
}
/*------------------------------------------
 函数功能:设置M2微分系数
 函数说明:浮点型			
------------------------------------------*/
void PIDM2_SetKd(float dKdd)
{	
	M2PID.Derivative = dKdd;
}
/*------------------------------------------
 函数功能:电机1位置式PID计算
 函数说明:		
------------------------------------------*/
int32_t PIDM1_PosLocCalc(float NextPoint)
{
    register float  iError,dError;

	iError = M1PID.SetPoint - NextPoint;        // 偏差
	M1PID.SumError += iError;				    // 积分
	if(M1PID.SumError > 2300.0)					//积分限幅2300
		M1PID.SumError = 2300.0;
	else if(M1PID.SumError < -2300.0)
		M1PID.SumError = -2300.0;	
	dError = iError - M1PID.LastError; 			// 当前微分
	M1PID.LastError = iError;
	
	return(int32_t)(  M1PID.Proportion * iError           	// 比例项
          		    + M1PID.Integral   * M1PID.SumError 		// 积分项
          		    + M1PID.Derivative * dError);
}

/*------------------------------------------
 函数功能:电机2位置式PID计算
 函数说明:			
------------------------------------------*/
int32_t PIDM2_PosLocCalc(float NextPoint)
{
	register float  iError,dError;

	iError = M2PID.SetPoint - NextPoint;        // 偏差
	M2PID.SumError += iError;
	if(M2PID.SumError > 2300.0)					//积分限幅
		M2PID.SumError = 2300.0;
	else if(M2PID.SumError < -2300.0)
		M2PID.SumError = -2300.0;
	dError = iError - M2PID.LastError; 			// 当前微分
	M2PID.LastError = iError;
	
	return(int32_t)(  M2PID.Proportion * iError           	// 比例项
          		    + M2PID.Integral   * M2PID.SumError 		// 积分项
          		    + M2PID.Derivative * dError);
}
