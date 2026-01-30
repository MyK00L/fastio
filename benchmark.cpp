// #define WINDOWS

#define NDEBUG

#include "fastio.cpp"
#include "fastio_float.cpp"

#include <array>
#include <cstdio>
#include <functional>
#include <string>
#include <vector>
#include <x86intrin.h>

using namespace std;

struct Rng {
	uint64_t x;
	template<typename T> T next() {
		uint64_t z = (x += 0x9e3779b97f4a7c15);
		z = (z^(z>>30))*0xbf58476d1ce4e5b9;
		z = (z^(z>>27))*0x94d049bb133111eb;
		return T(z^(z>>31));
	}
	Rng(uint64_t seed) : x(seed) {}
} rng(42);
template<> char Rng::next<char>() {
	uint64_t x = next<uint64_t>();
	return 'a'+x%26;
}
template<> string Rng::next<string>() {
	size_t siz = 1+(next<uint64_t>()%42);
	string s(siz, 'a');
	for(auto &c : s) c = next<char>();
	return s;
}

Printer eprint(DEF_ERR);

template<typename T> void test_print(const vector<T> &v, FILE *file) {
	Printer print(ft_from(file));
	print(v);
	print.flush();
}

template<typename T> void test_scan(const vector<T> &v, FILE *file) {
	Scanner scan(ft_from(file));
	auto s = v[0];
	for(const auto &x : v) {
		scan(s);
		if(x != s) [[unlikely]]
			__builtin_trap();
	}
}

#define bench(test, out) \
	uint64_t out = 0; \
	do { \
		out = __rdtsc(); \
		test; \
		out = __rdtsc()-out; \
	} while(0)

template<typename T> array<uint64_t, 2> bench_pair(const vector<T> &v) {
	FILE *tmp = tmpfile();
	bench(test_print(v, tmp), tw);
	rewind(tmp);
	bench(test_scan(v, tmp), tr);
	fclose(tmp);
	return {tr, tw};
}

template<typename T> array<uint64_t, 2> test_int_small() {
	vector<T> v;
	v.reserve(13001);
	for(T i = -6500; i<=6500; ++i) v.push_back(i);
	return bench_pair(v);
}

template<typename T, const size_t N> array<uint64_t, 2> bench_random() {
	vector<T> v(N);
	for(auto &x : v) x = rng.next<T>();
	return bench_pair(v);
}

array<uint64_t, 2> bench_avg(function<array<uint64_t, 2>()> f, const size_t num) {
	array<uint64_t, 2> ans = {num-1, num-1};
	for(size_t i = 0; i<num; ++i) {
		array<uint64_t, 2> t = f();
		ans[0] += t[0];
		ans[1] += t[1];
	}
	ans[0] /= num;
	ans[1] /= num;
	return ans;
}

int main() {
	{
		auto [tr, tw] = bench_avg(bench_random<string, 3000>, 2048);
		eprint("u64: tr=", tr, " tw=", tw, '\n');
	}
	{
		auto [tr, tw] = bench_avg(test_int_small<int16_t>, 2048);
		eprint("sml: tr=", tr, " tw=", tw, '\n');
	}
	return 0;
}
