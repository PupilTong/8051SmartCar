#include <Pid.h>
int pre2Err; //ǰ�������
int preErr;//ǰһ�����


unsigned char pid(int Err,unsigned char U){
	int pErr,dErr,dU;
	pErr = Err - preErr;   //����������ʽ���
	dErr = Err - 2 * preErr + pre2Err;  //΢��������ʽ���
	dU = Pconst * pErr+ Dconst * dErr + Iconst * Err;  //����������
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