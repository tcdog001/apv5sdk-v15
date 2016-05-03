#include"stdio.h"
#include"stdlib.h"
#include"time.h"

#include"ecc.h"
#include"string.h"


void print_string_array(char *name, void *_str,int len)
{
	int i;
	unsigned char *str = (unsigned char *)_str;

	if(name != NULL)
		printf("%s(%d) :\n", name, len);
	for(i=0;i<len;i++)
	{
		printf("%02X ",*str++);
		if((i+1)%16==0) printf("\n");
	}
	printf("\n");
}
	
int main()
{
	unsigned char datas[200] = {0,};
	int dataslen;
	unsigned char endata[200] = {0,};
	unsigned char dedata[200] = {0,};
	time_t  time,tmptime;
	double usetime;
	int tableLen;

	//==================test
	int status,len,i,len1;
	int testtime;
	unsigned char buff[200] = {0,};
	unsigned char PrivateKey[24] = {0,};
	int  KeyBitLen,LenOfPriKey;
	unsigned char PublicKey[49] = {0,};
	
	
	ECC_Init();

	//=======================
	sprintf ((char*)datas, "%s", "0123456789");//dataslen<=22
	//=====================
	KeyBitLen=192;
	testtime=100;
	
	
	dataslen=strlen((char*)datas);
	
do{
	//====================================
    	time=clock();
	for(i=0;i<testtime;i++)
		ecc192_genkey(PrivateKey,PublicKey);
	print_string_array("priv:", PrivateKey, 24);
	print_string_array("pub:", PublicKey, 49);
	tmptime=clock();
	usetime=(double)(tmptime-time)/CLOCKS_PER_SEC;
	printf("\n%d Generate_Key  times %f \n",testtime,usetime);

	//=================
	
#if 1	
	time=clock();
	for(i=0;i<testtime;i++)
	{
		len = ecc192_sign(PrivateKey,datas,dataslen,buff);
		status=ecc192_verify(PublicKey,datas,dataslen,buff, len);
		if(status <= 0)
		{
			printf("verify with public key failure\n");
			break;
		}
	}
	tmptime=clock();
	usetime=(double)(tmptime-time)/CLOCKS_PER_SEC;
	printf("\n%d Sign_With_Private_Key time %f ",i,usetime);
	time=clock();
	for(i=0;i<testtime;i++)
		status=ecc192_verify(PublicKey,datas,dataslen,buff, len);
	tmptime=clock();
	usetime=(double)(tmptime-time)/CLOCKS_PER_SEC;
	printf("\n%d Verify_With_Public_Key time %f ",i,usetime);
	if(status==1)
		printf("\nSucceed\n");
	else 
		printf("\nNot Succeed\n");
//==================
#endif	
	}while(0);
	getchar();
	return 0;
}
