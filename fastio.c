#include <unistd.h>
//#include <stdio.h>

#define NBIN 8000000
#define NBOUT 20

unsigned char input[NBIN];
unsigned inputi;

unsigned char output[NBOUT];
unsigned outputi;

//if values are already 0, you can delete first line
#define get_uns(x){\
	x=0;\
	while(input[inputi]<=32)inputi++;\
	while(input[inputi]>32)x=x*10+input[inputi++]-'0';\
}

#define get_int(x){\
	x=0;\
	unsigned char sign=0;\
	while(input[inputi]<=32)inputi++;\
	if(input[inputi]=='-'){sign=1;inputi++;}\
	while(input[inputi]>='0')x=x*10+input[inputi++]-'0';\
	if(sign)x=-x;\
}

#define get_char(x){\
	x=input[inputi++];\
}


//can't print 0, modifies value (thus x has to be lvalue)
#define put_uns(x){\
	unsigned char _put_b[22];\
	char _put_t=0;\
	while(x){\
		_put_b[_put_t++]=x%10+'0';\
		x/=10;\
	}\
	while(--_put_t>=0)output[outputi++]=_put_b[_put_t];\
}

#define put_int(x){\
	if(x<0){\
		output[outputi++]='-';\
		x=-x;\
	}\
	put_uns(x)\
}

#define put_char(x){\
	output[outputi++]=x;\
}

void put_str(char *x){
	while(*x)output[outputi++]=*(x++);
}

int main(){
	read(0,input,NBIN);
	//fread_unlocked(input,1,NBIN,stdin);

	unsigned x;
	get_uns(x);
	if(x==0)put_str("x=0\n");
	else {
		put_str("x=");
		put_uns(x);
		put_char('\n');
	}

	write(1,output,outputi);
	//fwrite_unlocked(output,1,outputi,stdout);
	return 0;
}

