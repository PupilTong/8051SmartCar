#include <Pwm.h>
void InitilizePwmChannel(unsigned char Channel);
unsigned char TransValue;
//void ChangeValue() interrupt
void InitilizePwm(){
	unsigned char i;
	P_SW2 |= 0x80;//������չsfr
	//��������
	PWMCH = (PWMT>>8);
	PWMCL = PWMT ;
//Initilize transverse  PWM3:P2.1 
	InitilizePwmChannel(3);
	//���õ�һ�η�תֵ
	PWM3T1H= 0x00;
	PWM3T1L= T1ValueL;
	
//Initilize Left driving wheel Pwm  PWM4 :P2.2
	//P2.2ǿ����
	InitilizePwmChannel(4);
	//���õ�һ�η�תֵ
	PWM4T1H= 0x00;
	PWM4T1L= T1ValueL;

//Initilize Right driving wheel Pwm PWM5 : P2.3
	//P2.2ǿ����
	InitilizePwmChannel(5);
	//���õ�һ�η�תֵ
	PWM5T1H= 0x00;
	PWM5T1L= T1ValueL;
	
	PWMCKS = 0x0d;//ϵͳʱ��,13��Ƶ
	/*AUXR |= 0x04;		//��ʱ��2ʱ��1Tģʽ
	T2L = (255-13);		//���ö�ʱ2��ֵ
	T2H = 0xFF;		//���ö�ʱ2��ֵ,24��Ƶ
	AUXR |= 0x10;		//��ʱ��2��ʼ��ʱ
	PWMCKS = 0x10;*///��ʱ��2���
	PWMCR |= 0x80; //����PWMģ��
	P_SW2 &= 0x7f;//��������״̬,��������չsfr
	SetRightWheel(0x00);
	SetLeftWheel(0x00);
	TransValue=LeftMax;
	for( i = RightMax ; i <= LeftMax ; i++){
		SetTrans(i);
	}
	SetTrans((LeftMax+RightMax)/2);
}
void InitilizePwmChannel(unsigned char Channel){
	Channel -=2;
	//����Ϊǿ����
	P2M0 |= 0x01<<Channel;
	P2M1 &= 0x01<<Channel;
	#if MosCloseLevel==0 //Ϊ��ʱ�رգ���ô��ʼ��ƽΪ��
		PWMCFG &= 0x01<<Channel;
	#else //�����ʼ��ƽΪ��
		PWMCFG |= 0x01<<Channel;
	#endif
	PWMCR |= 0x01<<Channel;//�����GPIO�ı�ΪPWM
}
void SetTran(unsigned char Value){//PWM3
	unsigned int CalcedValue ;
	P_SW2 |= 0x80;//������չsfr
	//�����Ҫ�����ֵ
	#if MosCloseLevel==1 //���η�ת��Ϊ�͵�ƽ,����ߵ�ƽʱ��
		Value = 0xff - Value;
	#endif
	CalcedValue = (unsigned int)(((float)Value)/255.0*(float)((PWMT - 1) - T1ValueL)); 
	CalcedValue += T1ValueL + 1;
	//����ڶ��η�ת�Ĵ���
	PWM3T2H = (CalcedValue>>8)&0xff;
	PWM3T2L = CalcedValue & 0xff;
	P_SW2 &= 0x7f;//��������״̬,��������չsfr
}
void SetLeftWheel(unsigned char Value){
	unsigned int CalcedValue ;
	P_SW2 |= 0x80;//������չsfr
	//�����Ҫ�����ֵ
	#if MosCloseLevel==1 //���η�ת��Ϊ�͵�ƽ,����ߵ�ƽʱ��
		Value = 0xff - Value;
	#endif
	CalcedValue = (unsigned int)(((float)Value)/255.0*(float)((PWMT - 1) - T1ValueL)); 
	CalcedValue += T1ValueL + 1;
	//����ڶ��η�ת�Ĵ���
	PWM4T2H = (CalcedValue>>8)&0xff;
	PWM4T2L = CalcedValue & 0xff;
	P_SW2 &= 0x7f;//��������״̬,��������չsfr
}
void SetRightWheel(unsigned char Value){
	unsigned int CalcedValue ;
	P_SW2 |= 0x80;//������չsfr
	//�����Ҫ�����ֵ
	#if MosCloseLevel==1 //���η�ת��Ϊ�͵�ƽ,����ߵ�ƽʱ��
		Value = 0xff - Value;
	#endif
	CalcedValue = (unsigned int)(((float)Value)/255.0*(float)((PWMT - 1) - T1ValueL)); 
	CalcedValue += T1ValueL + 1;
	//����ڶ��η�ת�Ĵ���
	PWM5T2H = (CalcedValue>>8)&0xff;
	PWM5T2L = CalcedValue & 0xff;
	P_SW2 &= 0x7f;//��������״̬,��������չsfr
}
void SetTrans(unsigned char Value){
	unsigned char i,t;
	if(TransValue < Value){
		t = Value - TransValue;
		for(i=0 ; i < t ; i++){
			TransValue++;
			SetTran(TransValue);
			os_wait (K_TMO, 13, 0); 
		}
	}
	else{
		t = TransValue - Value;
		for(i=0 ; i < t ; i++){
			TransValue--;
			SetTran(TransValue);
			os_wait (K_TMO, 13, 0); 
		}
	}
	TransValue = Value;
}