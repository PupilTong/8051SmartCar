#include <Serial.h>
unsigned char ReceiveBufferCount = 0;
unsigned char SerialThreadId;
unsigned char BusyFlag = 0;
void InitSerial(void(*CallBack)(char*)){		//115200bps@11.0592MHz
	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x01;		//����1ѡ��ʱ��2Ϊ�����ʷ�����
	AUXR |= 0x04;		//��ʱ��2ʱ��ΪFosc,��1T
	P_SW1 &=0x3F;   //Ĭ��λ��
	T2L = 0xE8;		//�趨��ʱ��ֵ
	T2H = 0xFF;		//�趨��ʱ��ֵ
	AUXR |= 0x10;		//������ʱ��2
	ES=1;//ʹ�ܴ����ж�
	SerialReceiveCallBack = CallBack;
}
void SerialInterruptProcesser() interrupt 4 {
	unsigned char temp;
	if(RI){//�����ж�
		ReceiveBuffer[ReceiveBufferCount] = SBUF;//���뻺����
		if(ReceiveBuffer[ReceiveBufferCount] == '\0' || ReceiveBufferCount == 62){//��������������յ�������־
			ReceiveBuffer[63]='\0';
			ReceiveBufferCount = 0;//������0
			RI = 0;
			isr_set_ready(SerialThreadId);
		}
		RI = 0;
	}
	if(TI){//��������ж�
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