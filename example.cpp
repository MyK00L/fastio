#include "fastio.cpp"

#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <map>

using namespace std;

long gettime() {
	timespec t;
	clock_gettime(0, &t);
	return t.tv_sec * (long)1e9 + t.tv_nsec;
}

#define SPEED_TEST(n, fn, end)\
{\
	size_t i = n;\
	long __time = gettime();\
	while (i--){fn;}\
	end;\
	__time = gettime() - __time;\
	cerr<<to_string((double)__time / n)<<"\t"<<#fn<<endl;\
}

Printer<> print;
Scanner<> scan;

int main() {
	vector<double> m(16);
	for(int i=0; i<16; ++i)m[i]=double(i)+0.012345;
	print("yes\n",m,'\n');
	return 0;
}

