#ifndef _PWM_H_
#define _PWM_H_

#include <System.h>

#define MosCloseLevel 0
#define PWMT 18431 //PWM周期20ms
#define T1ValueL 0xff //PWM第一次翻转的设定值, 0-0xff

#define LeftMax 41
#define RightMax 31

#define Speed 0x65

void InitilizePwm();
void SetTrans(unsigned char Value);
void SetLeftWheel(unsigned char Value);
void SetRightWheel(unsigned char Value);
#endif