#include <Pid.h>
int pre2Err; //前两拍误差
int preErr;//前一拍误差


unsigned char pid(int Err,unsigned char U){
	int pErr,dErr,dU;
	pErr = Err - preErr;   //比例项增量式误差
	dErr = Err - 2 * preErr + pre2Err;  //微分项增量式误差
	dU = Pconst * pErr+ Dconst * dErr + Iconst * Err;  //控制量增量
	pre2Err = preErr;
	preErr = Err;
	
	dU += (int)U;
	if(dU<0){
		return 0x00;
	}
	if(dU>255){
		return 0xff;
	}
	return (unsigned char)(dU & 0xff);
}