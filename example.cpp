#include "fastio.cpp"

#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <algorithm>

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
	//close(STDIN_FILENO);
	//close(STDOUT_FILENO);

	int pfds[2];
	pipe(pfds);
	SPEED_TEST(100000,print(-12342134),print('\n');print.flush())

}
