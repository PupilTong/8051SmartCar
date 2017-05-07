#ifndef __SERIAL_H_
#define __SERIAL_H_
#include <STC15F2K60S2.H>
#include <rtx51tny.h>                 /* RTX-51 tiny functions & defines      */
#include <System.h>

void InitSerial(void(*CallBack)(char*));
static void(*SerialReceiveCallBack)(char*);
static unsigned char ReceiveBuffer[64];
void SerialWrite(char * String);
#endif