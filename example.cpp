#include "fastio.cpp"
#include "fastio_float.cpp"

#include <unistd.h>
#include <vector>

using namespace std;

Printer<> print;
Scanner<> scan;

template<typename T> void tt() {
	T t;
	scan(t);
	print(t);
}

int main() {
	size_t n;
	scan(n);
	if(n == 0) {
		tt<int8_t>();
		tt<char>();
		tt<uint64_t>();
		tt<string>();
		tt<float>();
		tt<vector<vector<long>>>();
	}
	int64_t sum = 0;
	for(size_t i = 0; i < n; ++i) {
		int64_t x;
		scan(x);
		if(x == 0) {
			print(sum, '\n');
			print.flush();
		}
		sum += x;
	}
	print(sum, '\n');
	return 0;
}
