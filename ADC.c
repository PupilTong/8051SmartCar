#include <ADC.h>
unsigned char ADCThreadId;
unsigned char ADCBusyFlag = 0;
void ADCInterruptProcesser() interrupt 5 {
	ADC_CONTR &= !ADC_FLAG;         //���ADC�жϱ�־
	isr_set_ready(ADCThreadId);//����ת������ź���
}

void InitilizeADC(){
	ADC_RES = 0;                    //�������Ĵ���
	ADC_CONTR = ADC_POWER | ADC_SPEEDH;
	os_wait(K_TMO,2,0);//�ȴ�2����
	PADC = 0;//���ȼ���
	EADC = 1;//ʹ��ADC�ж�;
}
unsigned char GetADCResult(unsigned char Channel){
	while(ADCBusyFlag);//Loop until NotBusy
	ADCBusyFlag = 1;
	ADCThreadId = os_running_task_id();
	ADC_RES = 0;                    //�������Ĵ���
	if(Channel==9){
		P1ASF=0x00;//ѡ��ͨ��9
    ADC_CONTR = ADC_POWER | ADC_SPEEDLL | 0 | ADC_START;
	}
	else{ //Channel<9
		P1ASF=0x01<<Channel;//������
		ADC_CONTR = ADC_POWER | ADC_SPEEDLL | ADC_START | Channel;//ѡ��ͨ��������
	}
	os_wait(K_SIG,2,0);//�ȴ�ת�����
	Channel = ADC_RES;//��ʱ����
	ADCBusyFlag = 0;
	return Channel;
}
unsigned char GetVccReference(){
	return GetADCResult(9);
}
