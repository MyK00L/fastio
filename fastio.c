//#include <unistd.h>
#include <stdio.h>

#define NBIN 100000
#define NBOUT 100000

unsigned char input[NBIN];
unsigned inputi;

unsigned char output[NBOUT];
unsigned outputi;

#define get_unsigned(x){\
	x=0;\
	while(input[inputi]<=32)inputi++;\
	while(input[inputi]>='0')x=x*10+input[inputi++]-'0';\
}
#define get_int(x){\
	x=0;\
	/*bool*/unsigned char sign=0;\
	while(input[inputi]<=32)inputi++;\
	if(input[inputi]=='-'){sign=1;inputi++;}\
	while(input[inputi]>='0')x=x*10+input[inputi++]-'0';\
	if(sign)x=-x;\
}
#define get_char(x){\
	x=input[inputi++];\
}

#define put_uns(x){\
	unsigned char b[22];\
	int t=0;\
	while(x){\
		b[t++]=x%10+'0';\
		x/=10;\
	}\
	while(--t>=0)output[outputi++]=b[t];\
}
#define put_int(x){\
	unsigned char b[22];\
	int t=0;\
	if(x<0){\
		x=-x;\
		output[outputi++]='-';\
	}\
	while(x){\
		b[t++]=x%10+'0';\
		x/=10;\
	}\
	while(--t>=0)output[outputi++]=b[t];\
}
#define put_char(x){\
	output[outputi++]=x;\
}
void put_str(char *x){
	while(*x)output[outputi++]=*(x++);
}


int main(){
	//read(0,input,NBIN);
	fread_unlocked(input,1,NBIN,stdin);

	int x;
	get_int(x);
	put_int(x);
	put_char('x');
	put_char('\n');
	put_str("YES\n");

	//write(1,output,outputi);
	fwrite_unlocked(output,1,outputi,stdout);
}

