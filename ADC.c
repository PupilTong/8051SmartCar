#include <ADC.h>
unsigned char ADCThreadId;
unsigned char ADCBusyFlag = 0;
void ADCInterruptProcesser() interrupt 5 {
	ADC_CONTR &= !ADC_FLAG;         //清除ADC中断标志
	isr_set_ready(ADCThreadId);//发送转换完成信号量
}

void InitilizeADC(){
	ADC_RES = 0;                    //清除结果寄存器
	ADC_CONTR = ADC_POWER | ADC_SPEEDH;
	os_wait(K_TMO,2,0);//等待2毫秒
	PADC = 0;//优先级低
	EADC = 1;//使能ADC中断;
}
unsigned char GetADCResult(unsigned char Channel){
	while(ADCBusyFlag);//Loop until NotBusy
	ADCBusyFlag = 1;
	ADCThreadId = os_running_task_id();
	ADC_RES = 0;                    //清除结果寄存器
	if(Channel==9){
		P1ASF=0x00;//选择通道9
    ADC_CONTR = ADC_POWER | ADC_SPEEDLL | 0 | ADC_START;
	}
	else{ //Channel<9
		P1ASF=0x01<<Channel;//打开引脚
		ADC_CONTR = ADC_POWER | ADC_SPEEDLL | ADC_START | Channel;//选择通道并启动
	}
	os_wait(K_SIG,2,0);//等待转换完成
	Channel = ADC_RES;//临时保存
	ADCBusyFlag = 0;
	return Channel;
}
unsigned char GetVccReference(){
	return GetADCResult(9);
}
