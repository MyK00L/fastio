#include "fastio.cpp"

#include <vector>
#include <array>
#include <cassert>
#include <cstdio>
#include <string>
#include <functional>

using namespace std;

struct Rng {
	uint64_t x;
	template<typename T>
	T next() {
		uint64_t z = (x += 0x9e3779b97f4a7c15);
		z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
		z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
		return T(z ^ (z >> 31));
	}
	template<> char next<char>() {
		uint64_t x = next<uint64_t>();
		return 'a'+x%26;
	}
	template<> string next<string>() {
		size_t siz = next<uint64_t>()%100000;
		string s(siz,'a');
		for(auto &c: s) c = next<char>();
		return s;
	}
	Rng(uint64_t seed): x(seed) {}
} rng(42);

Printer eprint(2);

template<typename T>
void test_print(const vector<T>& v, FILE* file) {
	Printer print(fileno(file));
	print(v);
	print.flush();
}

template<typename T>
void test_scan(const vector<T>& v, FILE* file) {
	Scanner scan(fileno(file));
	auto s = v[0];
	for(const auto& x:v) {
		scan(s);
		assert(x==s);
	}
}

#define bench(test, out) \
	uint64_t out=0;\
	do {\
		out=__rdtsc();\
		test;\
		out=__rdtsc()-out;\
	} while(0)

template<typename T>
array<uint64_t,2> bench_pair(const vector<T>& v) {
	FILE* tmp = tmpfile();
	bench(test_print(v,tmp),tw);
	rewind(tmp);
	bench(test_scan(v,tmp),tr);
	fclose(tmp);
	return {tr,tw};
}

void test_int_small() {
	vector<int> v;
	v.reserve(200001);
	for(int i=-100000; i<=100000; ++i) v.push_back(i);
	auto [tr,tw] = bench_pair(v);
	eprint("tr=",tr," tw=",tw,'\n');
}

template<typename T, const size_t N>
array<uint64_t,2> bench_random() {
	vector<T> v(N);
	for(auto &x:v) x=rng.next<T>();
	return bench_pair(v);
}

array<uint64_t,2> bench_avg(function<array<uint64_t,2>()> f, const size_t num) {
	array<uint64_t,2> ans = {num-1,num-1};
	for(size_t i=0; i<num; ++i) {
		array<uint64_t,2> t = f();
		ans[0]+=t[0];
		ans[1]+=t[1];
	}
	ans[0]/=num;
	ans[1]/=num;
	return ans;
}

int main() {
	auto [tr,tw] = bench_avg(bench_random<int64_t,100000>, 20);
	eprint("tr=",tr," tw=",tw,'\n');
	return 0;
}

