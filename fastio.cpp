#pragma GCC optimize("Ofast")
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <cstdio>
#include <cmath>
#include <string>
#include <algorithm>

using namespace std;

template<size_t BUFSIZE=1<<16>
class Scanner{
	private:
		char buf[BUFSIZE];
		char* it;
		char* ed;
		const int fd;
		Scanner(const Scanner&) = delete;
		Scanner& operator=(const Scanner&) = delete;
		void flush(){
			if(it==ed){
				ed=buf+read(fd,buf,BUFSIZE);
				it=buf;
			}
		}
		void ss(){
			flush();
			while(*it<=32){++it;flush();}
		}
		void scan(char& x){ss();x=*(it++);}
		void scan(string& x){
			do{
				ss();
				char * itbg=it;
				while(it!=ed&&*it>32)++it;
				x.append(itbg,it);
			} while(it==ed);
		}
		template<typename T> void scan_u(T& x){
			ss();
			x=0;
			do{
				x=T(x*10+*(it++)-'0');
				flush();
			} while(*it>32);
		}
		template<typename T> void scan_i(T& x){
			ss();
			x=0;
			bool sign=0;
			if(*it=='-'){
				sign=1;
				++it;
				flush();
			}
			do{
				x=T(x*10+*(it++)-'0');
				flush();
			} while(*it>32);
			if(sign)x=T(-x);
		}
		template<typename T> void scan_f(T& x){
			ss();
			x=0;
			bool sign=0;
			if(*it=='-'){
				sign=1;
				++it;
				flush();
			}
			while(*it>32 && *it!='.'){
				x=x*T(10.0)+T(*(it++)-'0');
				flush();
			}
			if(*it=='.'){
				++it;
				flush();
				T e=T(0.1);
				while(*it>32){
					x=x+e*T(*(it++)-'0');
					e=e*T(0.1);
					flush();
				};
			}
			if(sign)x=-x;
		}
		void scan(int8_t& x){scan_i(x);}
		void scan(int16_t& x){scan_i(x);}
		void scan(int32_t& x){scan_i(x);}
		void scan(int64_t& x){scan_i(x);}
		void scan(long long& x){scan_i(x);}
		void scan(uint8_t& x){scan_u(x);}
		void scan(uint16_t& x){scan_u(x);}
		void scan(uint32_t& x){scan_u(x);}
		void scan(uint64_t& x){scan_u(x);}
		void scan(unsigned long long& x){scan_u(x);}
		void scan(float& x){double d; scan_f(d); x=d;}
		void scan(double& x){scan_f(x);}
		void scan(long double& x){scan_f(x);}

		template<typename T> void scan(T& x){for(auto &i:x)scan(i);}
	public:
		~Scanner(){}
		Scanner(const int _fd=0):it(0),ed(0),fd(_fd){}
		void operator()(){}
		template<typename T, typename...R>
			void operator()(T& a,R&...rest){
				scan(a);
				operator()(rest...);
			}
};

template<size_t BUFSIZE=1<<16>
class Printer {
	private:
		char buf[BUFSIZE];
		char* it;
		const int fd;
		void fif(const size_t x){
			if(size_t(BUFSIZE+buf-it)<x)flush();
		}
		void print(const char x){fif(1);*(it++)=x;}
		void print(const char* const x){
			size_t s = strlen(x);
			if(s>BUFSIZE/2){
				flush();
				write(fd,x,s);
			} else {
				fif(s);
				copy(x,x+s,it);
				it+=s;
			}
		}
		void print(const string& x){
			if(x.size()>BUFSIZE/2){
				flush();
				write(fd,x.data(),x.size());
			} else {
				fif(x.size());
				copy(x.begin(),x.end(),it);
				it+=x.size();
			}
		}
		template<typename T> void print_u(T x){
			constexpr size_t siz = size_t(sizeof(T) * log10(256)) + 1;
			uint8_t i=siz;
			char b[siz];
			do {
				b[--i]=char(x%10+'0');
				x=T(x/10);
			}while(x);
			fif(siz-i);
			copy(b+i,b+siz,it);
			it+=siz-i;
		}
		template<typename T> void print_i(T x){
			constexpr size_t siz = size_t(sizeof(T) * log10(256)) + 2;
			size_t i=siz;
			char b[siz];
			const bool neg=(x<0);
			if(neg)x=-x;
			do {
				b[--i]=char(x%10+'0');
				x=T(x/10);
			}while(x);
			if(neg)b[--i]='-';
			fif(siz-i);
			copy(b+i,b+siz,it);
			it+=siz-i;
		}
		void print_f(const double x){
			const uint64_t d = *((uint64_t*)&x);
			const bool neg = d>>63;
			int32_t e = (d>>52)&((1ull<<11)-1);
			uint64_t m = d&((1ull<<52)-1);
			if(e){
				e-=1075;
				m+=(1ull<<52);
			} else e=-1074ll;
			int32_t re=e;
			if(e<0){
				while(e++){
					if(m&0xe000000000000000){
						m>>=1;
						re++;
					} else m*=5;
				}
			} else {
				while(e--){
					if(m&0x8000000000000000) m/=5;
					else {
						m<<=1;
						re--;
					}
				}
			}
			if(neg)print('-');
			print(m);
			print('e');
			print(re);
		}
		void print(int8_t x){print_i(x);}
		void print(int16_t x){print_i(x);}
		void print(int32_t x){print_i(x);}
		void print(int64_t x){print_i(x);}
		void print(long long x){print_i(x);}
		void print(uint8_t x){print_u(x);}
		void print(uint16_t x){print_u(x);}
		void print(uint32_t x){print_u(x);}
		void print(uint64_t x){print_u(x);}
		void print(unsigned long long x){print_u(x);}
		void print(const float x){print_f(x);}
		void print(const double x){print_f(x);}
		void print(const long double x){print_f(x);}

		template<typename T> void print(const T& x){
			for(auto &i:x){
				print(i);
				print(' ');
			}
		}
	public:
		~Printer(){flush();}
		Printer(const int _fd=1):it(buf),fd(_fd){}
		void flush(){
			write(fd,buf,it-buf);
			it=buf;
		}
		void operator()(){}
		template<typename T, typename...R>
			void operator()(T&& a,R&&...rest){
				print(a);
				operator()(rest...);
			}
};

