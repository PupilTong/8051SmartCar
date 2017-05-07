#include <Pwm.h>
void InitilizePwmChannel(unsigned char Channel);
unsigned char TransValue;
//void ChangeValue() interrupt
void InitilizePwm(){
	unsigned char i;
	P_SW2 |= 0x80;//访问扩展sfr
	//设置周期
	PWMCH = (PWMT>>8);
	PWMCL = PWMT ;
//Initilize transverse  PWM3:P2.1 
	InitilizePwmChannel(3);
	//设置第一次翻转值
	PWM3T1H= 0x00;
	PWM3T1L= T1ValueL;
	
//Initilize Left driving wheel Pwm  PWM4 :P2.2
	//P2.2强推挽
	InitilizePwmChannel(4);
	//设置第一次翻转值
	PWM4T1H= 0x00;
	PWM4T1L= T1ValueL;

//Initilize Right driving wheel Pwm PWM5 : P2.3
	//P2.2强推挽
	InitilizePwmChannel(5);
	//设置第一次翻转值
	PWM5T1H= 0x00;
	PWM5T1L= T1ValueL;
	
	PWMCKS = 0x0d;//系统时钟,13分频
	/*AUXR |= 0x04;		//定时器2时钟1T模式
	T2L = (255-13);		//设置定时2初值
	T2H = 0xFF;		//设置定时2初值,24分频
	AUXR |= 0x10;		//定时器2开始计时
	PWMCKS = 0x10;*///定时器2溢出
	PWMCR |= 0x80; //启动PWM模块
	P_SW2 &= 0x7f;//返回正常状态,不访问扩展sfr
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
	//设置为强推挽
	P2M0 |= 0x01<<Channel;
	P2M1 &= 0x01<<Channel;
	#if MosCloseLevel==0 //为低时关闭，那么初始电平为低
		PWMCFG &= 0x01<<Channel;
	#else //否则初始电平为高
		PWMCFG |= 0x01<<Channel;
	#endif
	PWMCR |= 0x01<<Channel;//输出从GPIO改变为PWM
}
void SetTran(unsigned char Value){//PWM3
	unsigned int CalcedValue ;
	P_SW2 |= 0x80;//访问扩展sfr
	//计算出要放入的值
	#if MosCloseLevel==1 //两次翻转中为低电平,计算高电平时间
		Value = 0xff - Value;
	#endif
	CalcedValue = (unsigned int)(((float)Value)/255.0*(float)((PWMT - 1) - T1ValueL)); 
	CalcedValue += T1ValueL + 1;
	//放入第二次翻转寄存器
	PWM3T2H = (CalcedValue>>8)&0xff;
	PWM3T2L = CalcedValue & 0xff;
	P_SW2 &= 0x7f;//返回正常状态,不访问扩展sfr
}
void SetLeftWheel(unsigned char Value){
	unsigned int CalcedValue ;
	P_SW2 |= 0x80;//访问扩展sfr
	//计算出要放入的值
	#if MosCloseLevel==1 //两次翻转中为低电平,计算高电平时间
		Value = 0xff - Value;
	#endif
	CalcedValue = (unsigned int)(((float)Value)/255.0*(float)((PWMT - 1) - T1ValueL)); 
	CalcedValue += T1ValueL + 1;
	//放入第二次翻转寄存器
	PWM4T2H = (CalcedValue>>8)&0xff;
	PWM4T2L = CalcedValue & 0xff;
	P_SW2 &= 0x7f;//返回正常状态,不访问扩展sfr
}
void SetRightWheel(unsigned char Value){
	unsigned int CalcedValue ;
	P_SW2 |= 0x80;//访问扩展sfr
	//计算出要放入的值
	#if MosCloseLevel==1 //两次翻转中为低电平,计算高电平时间
		Value = 0xff - Value;
	#endif
	CalcedValue = (unsigned int)(((float)Value)/255.0*(float)((PWMT - 1) - T1ValueL)); 
	CalcedValue += T1ValueL + 1;
	//放入第二次翻转寄存器
	PWM5T2H = (CalcedValue>>8)&0xff;
	PWM5T2L = CalcedValue & 0xff;
	P_SW2 &= 0x7f;//返回正常状态,不访问扩展sfr
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