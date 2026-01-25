#include "fastio.cpp"

#include <vector>
#include <unistd.h>

using namespace std;

Printer<> print;
Scanner<> scan;

template<typename T>
void tt() {
	if(false) {
		T t;
		scan(t);
		print(t);
	}
}

int main() {
	tt<int8_t>();
	tt<char>();
	tt<uint64_t>();
	tt<string>();
	tt<vector<vector<long>>>();
	size_t n;
	scan(n);
	int64_t sum=0;
	for(size_t i=0; i<n; ++i) {
		int64_t x;
		scan(x);
		if(x==0) {
			print(sum,'\n');
			print.flush();
		}
		sum+=x;
	}
	print(sum,'\n');
	return 0;
}

