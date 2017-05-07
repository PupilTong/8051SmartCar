#include <System.h>

void rectest(char * Call){
	P27=~P27;
}
SystemServices () _task_ 0  {     
	P1M0 = 0x00;
  P1M1 = 0x00;
  P2M0 = 0x00;
  P2M1 = 0x00;     
	P3M0 = 0x00;
	P3M1 = 0x00;
	//InitSerial(&rectest);
	InitilizePwm();
	InitilizeADC();
  //os_create_task (1);                 /* start task 1                         */
  //os_create_task (2);                 /* start task 2                         */
  os_create_task (1);                 /* start task 3                         */

  while (1)  {                        /* endless loop                         */
    os_wait (K_SIG,1,0);            /* wait SIG            */
		SerialReceiveCallBack(ReceiveBuffer);
  }
}