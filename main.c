/******************************************************************************/
/*                                                                            */
/*                     RTX_EX2.C:  A RTX-51 Application                       */
/*                                                                            */
/******************************************************************************/

#include <rtx51tny.h>                 /* RTX-51 tiny functions & defines      */
#include <STC15F2K60S2.H>
#include <System.h>


void ADCServices () _task_ 2  {
 unsigned char a,b;
 a=40;
  while (1)  { 
		//P20=~P20;
    /*os_wait (K_TMO, 500, 0); 
		SetTrans(LeftMax);
    os_wait (K_TMO, 500, 0); 
		SetTrans(RightMax);
		P2=~(a<<4);
		if(b==0){
			a++;
			if(a==41){
				b=1;
			}
		}
		else{
			a--;
			if(a==30){
				b=0;
			}
		}*/
		//SerialWrite("hello \n \0");
  }
}


void job2 () _task_ 3  {
 unsigned char a,b;
  while (1)  {
    os_wait (K_TMO, 2, 0);           /* wait for timeout: 10 ticks           */
		//P22=~P22;
  }
}

void job3 () _task_ 1  {
 unsigned char NL,NR;//近处两个
 unsigned char FL,FR;//远处两个
 unsigned char NResult,FResult,Result;
 unsigned char BoolL;
  while (1)  {
    os_wait (K_TMO, 3, 0);  
		NL = GetADCResult(0);
		NR = GetADCResult(1);  
		FL = GetADCResult(2);
		FR = GetADCResult(3);
		if(NL > 0x3f && NR > 0x3f){//近处界限未超出
			if(NL > NR){
				NResult = NL - NR;
				BoolL = 0;
			}
			else{
				NResult = NR - NL;//在左
				BoolL = 1;
			}
			if(NResult <= 0x03){
				NResult = 0x00;
			}
			else if(NResult <= 0x07){
				NResult = 0x01;
			}
			else if(NResult <= 0x0f){
				NResult = 0x02;
			}
			else if(NResult <= 0x7f){
				NResult = 0x03;
			}
			else if(NResult >= 0x80){
				NResult = 0x04;
			}
		}
		else{//超出
			NResult = NR/2+NL/2;
			if(NL > NR){;
				BoolL = 0;
			}
			else{//在左
				BoolL = 1;
			}
			
			if(NResult >= 0x40){
				NResult = 0x05;
			}
			else{
				NResult = 0x06;
			}
		}
		//----------------------------------近传感器解算完毕
		FResult = FR>FL?FL:FR;
		if(FResult <= 0x0B){
			Result = 0x0;
		}
		else if(FResult <= 0x30){
			Result = 0x25;
		}
		else if(FResult >= 0x40){
			Result = 0x40;
		}
		else if(FResult >= 0x50){
			Result = 0x55;
		}
		FResult = Speed - Result;
		//--------------------------------------远传感器解算完毕
		if(BoolL){
			SetTrans(38-NResult);
		}
		else{
			SetTrans(38+NResult);
		}
		SetLeftWheel(FResult);
		SetRightWheel(FResult);
		P2=~FResult;
		//Result=(unsigned char)((float)GetADCResult(0)/255.0*(float)GetVccReference()*3);;
  }
}
