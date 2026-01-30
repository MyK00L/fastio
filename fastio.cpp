#include <algorithm>
#include <cstdint>
#include <cstring>
#include <string>

using namespace std;

#ifdef WINDOWS
#include <cstdio>
#define FT FILE *
#define FIO_READ(fd, buf, count) fread(buf, 1, count, fd)
#define FIO_WRITE(fd, buf, count) fwrite(buf, 1, count, fd)
#define DEF_IN stdin
#define DEF_OUT stdout
#define DEF_ERR stderr
__attribute__((unused)) static inline FILE *ft_from(FILE *fp) { return fp; }
#else
#include <unistd.h>
#define FT const int
#define FIO_READ(fd, buf, count) read(fd, buf, count)
#define FIO_WRITE(fd, buf, count) write(fd, buf, count)
#define DEF_IN STDIN_FILENO
#define DEF_OUT STDOUT_FILENO
#define DEF_ERR STDERR_FILENO
__attribute__((unused)) static inline int ft_from(FILE *fp) { return fileno(fp); }
__attribute__((unused)) static inline int ft_from(int fd) { return fd; }
#endif

template<size_t BUFSIZE = 1<<16> class Scanner {
  private:
	char buf[BUFSIZE];
	char *it;
	char *ed;
	FT fd;
	Scanner(const Scanner &) = delete;
	Scanner &operator=(const Scanner &) = delete;
	static inline bool has_space8(const uint64_t x) noexcept {
		constexpr uint64_t _96 = 0x5f5f5f5f5f5f5f5f;
		constexpr uint64_t _128 = 0x8080808080808080;
		return (~(x+_96))&_128;
	}
	static inline bool has_space4(const uint32_t x) noexcept {
		constexpr uint32_t _96 = 0x5f5f5f5f;
		constexpr uint32_t _128 = 0x80808080;
		return (~(x+_96))&_128;
	}
	bool has_whitespace() const noexcept {
		char *i = it;
		while(ed-i>=8) {
			uint64_t a = 0;
			std::memcpy(&a, i, sizeof(a));
			if(has_space8(a)) return 1;
			i += 8;
		}
		while(ed-i>=4) {
			uint32_t a = 0;
			std::memcpy(&a, i, sizeof(a));
			if(has_space4(a)) return 1;
			i += 4;
		}
		while(i<ed)
			if(*(i++)<=32) return 1;
		return 0;
	}
#ifdef FAST_FIFSS
	// faster fifss, does not work with interactive problems, in the shell, or with a lot of whitespace
	void fifss() noexcept {
		constexpr size_t x = 32;
		if(it+x>ed) {
			it = copy(it, ed, buf);
			ed = it+FIO_READ(fd, it, BUFSIZE+buf-it);
			it = buf;
		}
		while(*it<=32) ++it;
	}
#else
	void fifss() noexcept {
		while(it<ed && *it<=32) ++it;
		if(!has_whitespace()) {
			it = copy(it, ed, buf);
			ed = it+FIO_READ(fd, it, BUFSIZE+buf-it);
			it = buf;
		}
		while(*it<=32) ++it;
	}
#endif
	void _flush() noexcept {
		ed = buf+FIO_READ(fd, buf, BUFSIZE);
		it = buf;
	}
	inline void ss() noexcept {
		while(it != ed && *it<=32) ++it;
		if(it==ed) {
			_flush();
			ss();
		}
	}
	void scan(char &x) noexcept {
		ss();
		x = *(it++);
	}
	void scan(string &x) noexcept {
		x.clear();
		ss();
		do {
			if(it==ed) _flush();
			char *itbg = it;
			while(ed-it>=8) {
				uint64_t a;
				std::memcpy(&a, it, sizeof(a));
				if(has_space8(a)) break;
				it += 8;
			}
			while(it != ed && *it>32) ++it;
			x.append(itbg, it);
		} while(it==ed);
	}
	static inline void trick8(uint64_t &x) noexcept {
		// x = ((x&0x0f000f000f000f00)>>8)+((x&0x000f000f000f000f)*10); // 1 less imul, but slower(?)...
		x = (x&0x0F0F0F0F0F0F0F0F)*2561>>8;
		x = (x&0x00FF00FF00FF00FF)*6553601>>16;
		x = (x&0x0000FFFF0000FFFF)*42949672960001>>32;
	}
	static inline void trick4(uint32_t &x) noexcept {
		x = (x&0x0F0F0F0F)*2561>>8;
		x = (x&0x00FF00FF)*6553601>>16;
	}
	template<typename T> inline void scan_u_after_fifss(T &x) noexcept {
		x = 0;
		if(ed-it>=8) {
			uint64_t a = 0;
			std::memcpy(&a, it, sizeof(a));
			if(!has_space8(a)) {
				trick8(a);
				x = T(a);
				it += 8;
				if(ed-it>=8) {
					std::memcpy(&a, it, sizeof(a));
					if(!has_space8(a)) {
						trick8(a);
						x = T(x*T(100000000)+T(a));
						it += 8;
					}
				}
			}
		}
		if(ed-it>=4) {
			uint32_t a = 0;
			std::memcpy(&a, it, sizeof(a));
			if(!has_space4(a)) {
				trick4(a);
				x = T(x*T(10000)+T(a));
				it += 4;
			}
		}
		while(*it>32) { x = T(x*10+(*it++&0xf)); }
	}
	template<typename T> void scan_u(T &x) noexcept {
		fifss();
		scan_u_after_fifss(x);
	}
	template<typename T> void scan_i(T &x) noexcept {
		fifss();
		bool sign = 0;
		if(*it=='-') {
			sign = 1;
			++it;
		}
		scan_u_after_fifss(x);
		if(sign) x = T(-x);
	}

	void scan(int8_t &x) noexcept { scan_i(x); }
	void scan(int16_t &x) noexcept { scan_i(x); }
	void scan(int32_t &x) noexcept { scan_i(x); }
	void scan(int64_t &x) noexcept { scan_i(x); }
	void scan(long long &x) noexcept { scan_i(x); }
	void scan(uint8_t &x) noexcept { scan_u(x); }
	void scan(uint16_t &x) noexcept { scan_u(x); }
	void scan(uint32_t &x) noexcept { scan_u(x); }
	void scan(uint64_t &x) noexcept { scan_u(x); }
	void scan(unsigned long long &x) noexcept { scan_u(x); }

	template<typename T> inline void scan_u_strict_no0_after_fifss(T &x) noexcept;
	template<typename T> void scan_f(T &x) noexcept;
	void scan(float &x) noexcept;
	void scan(double &x) noexcept;
	void scan(long double &x) noexcept;

	template<typename T> void scan(T &x) noexcept {
		for(auto &i : x) scan(i);
	}

  public:
	~Scanner() noexcept {}
	Scanner(FT _fd = DEF_IN) noexcept : it(0), ed(0), fd(_fd) {}
	void operator()() noexcept {}
	template<typename T, typename... R> void operator()(T &a, R &...rest) noexcept {
		scan(a);
		operator()(rest...);
	}
};

template<size_t BUFSIZE = 1<<16> class Printer {
  private:
	char buf[BUFSIZE];
	char *it;
	FT fd;
	void fif(const size_t x) noexcept {
		if(size_t(BUFSIZE+buf-it)<x) flush();
	}
	void print(const char x) noexcept {
		fif(1);
		*(it++) = x;
	}
	void print(char *const x) noexcept {
		size_t s = strlen(x);
		if(s>BUFSIZE/2) {
			flush();
			FIO_WRITE(fd, x, s);
		} else {
			fif(s);
			copy(x, x+s, it);
			it += s;
		}
	}
	void print(const char *const x) noexcept { print((char *)x); }
	void print(const string &x) noexcept {
		if(x.size()>BUFSIZE/2) {
			flush();
			FIO_WRITE(fd, x.data(), x.size());
		} else {
			fif(x.size());
			copy(x.begin(), x.end(), it);
			it += x.size();
		}
	}
	template<typename T> static uint8_t ndigits(T x) noexcept {
		if(!x) return 1;
		uint8_t ans = 0;
		while(x>=10000000) {
			ans += 8;
			x /= 100000000;
		}
		if(x>=1000) {
			ans += 4;
			x /= 10000;
		}
		if(x>=10) {
			ans += 2;
			x /= 100;
		}
		if(x) ans += 1;
		return ans;
	}
	template<typename T> void print_u_afterfif(T x, const uint8_t nd0) noexcept {
		static constexpr char i2a[201] =
			"0001020304050607080910111213141516171819202122232425262728293031323334353637383940414243444546474849505152"
			"5354555657585960616263646566676869707172737475767778798081828384858687888990919293949596979899";
		uint8_t nd = nd0;
		while(x>=100000000) {
			uint32_t _0 = x%100000000;
			x /= 100000000;
			uint32_t _1 = _0%10000;
			_0 /= 10000;
			uint32_t _00_2 = (_0/100)*2;
			uint32_t _01_2 = (_0%100)*2;
			uint32_t _10_2 = (_1/100)*2;
			uint32_t _11_2 = (_1%100)*2;
			memcpy(it+nd-8, i2a+_00_2, 2);
			memcpy(it+nd-6, i2a+_01_2, 2);
			memcpy(it+nd-4, i2a+_10_2, 2);
			memcpy(it+nd-2, i2a+_11_2, 2);
			nd -= 8;
		}
		if(x>=10000) {
			uint32_t _0 = x%10000;
			x /= 10000;
			uint32_t _1 = _0%100;
			_0 /= 100;
			memcpy(it+nd-4, i2a+_0*2, 2);
			memcpy(it+nd-2, i2a+_1*2, 2);
			nd -= 4;
		}
		if(x>=100) {
			uint32_t _0 = x%100;
			x /= 100;
			memcpy(it+nd-2, i2a+_0*2, 2);
			nd -= 2;
		}
		if(x>=10) memcpy(it+nd-2, i2a+x*2, 2);
		else
			it[nd-1] = '0'+x;
		it += nd0;
	}
	template<typename T> void print_u(T x) noexcept {
		fif(22);
		print_u_afterfif(x, ndigits(x));
	}
	template<typename T> void print_i(T x) noexcept {
		fif(22);
		if(x<0) {
			*(it++) = '-';
			x = -x;
		}
		print_u_afterfif(x, ndigits(x));
	}
	void print(int8_t x) noexcept { print_i(x); }
	void print(int16_t x) noexcept { print_i(x); }
	void print(int32_t x) noexcept { print_i(x); }
	void print(int64_t x) noexcept { print_i(x); }
	void print(long long x) noexcept { print_i(x); }
	void print(uint8_t x) noexcept { print_u(x); }
	void print(uint16_t x) noexcept { print_u(x); }
	void print(uint32_t x) noexcept { print_u(x); }
	void print(uint64_t x) noexcept { print_u(x); }
	void print(unsigned long long x) noexcept { print_u(x); }

	void print_f(const double x) noexcept;
	void print(const float x) noexcept;
	void print(const double x) noexcept;
	void print(const long double x) noexcept;

	template<typename T> void print(const T &x) noexcept {
		for(auto &i : x) {
			print(i);
			print(' ');
		}
	}

  public:
	~Printer() noexcept { flush(); }
	Printer(FT _fd = DEF_OUT) noexcept : it(buf), fd(_fd) {}
	void flush() noexcept {
		std::ignore = FIO_WRITE(fd, buf, it-buf);
		it = buf;
	}
	void operator()() noexcept {}
	template<typename T, typename... R> void operator()(T &&a, R &&...rest) noexcept {
		print(a);
		operator()(rest...);
	}
};
