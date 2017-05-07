#include <Serial.h>
unsigned char ReceiveBufferCount = 0;
unsigned char SerialThreadId;
unsigned char BusyFlag = 0;
void InitSerial(void(*CallBack)(char*)){		//115200bps@11.0592MHz
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x01;		//串口1选择定时器2为波特率发生器
	AUXR |= 0x04;		//定时器2时钟为Fosc,即1T
	P_SW1 &=0x3F;   //默认位置
	T2L = 0xE8;		//设定定时初值
	T2H = 0xFF;		//设定定时初值
	AUXR |= 0x10;		//启动定时器2
	ES=1;//使能串口中断
	SerialReceiveCallBack = CallBack;
}
void SerialInterruptProcesser() interrupt 4 {
	unsigned char temp;
	if(RI){//接收中断
		ReceiveBuffer[ReceiveBufferCount] = SBUF;//存入缓冲区
		if(ReceiveBuffer[ReceiveBufferCount] == '\0' || ReceiveBufferCount == 62){//缓冲区溢出或者收到结束标志
			ReceiveBuffer[63]='\0';
			ReceiveBufferCount = 0;//计数置0
			RI = 0;
			isr_set_ready(SerialThreadId);
		}
		RI = 0;
	}
	if(TI){//发送完毕中断
		TI=0;
		isr_set_ready(SerialThreadId);
	}
}
void SerialWrite(char *String){
	int i;
	while(BusyFlag);// Wait until BUSYFLG=0
	BusyFlag = 1;//SetBusy
	SerialThreadId = os_running_task_id();
	for(i = 0; i < 64 && String[i] != '\0'; i++){
		SBUF=String[i];
    os_wait (K_SIG,1,0);            /* wait SIG            */
	}
	BusyFlag = 0;
}