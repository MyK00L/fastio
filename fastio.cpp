#include <unistd.h>
#include <fcntl.h>
#include <cstdint>
#include <cstring>
#include <cstdio>
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
		// FILE* fd;
		Scanner(const Scanner&) = delete;
		Scanner& operator=(const Scanner&) = delete;
		void fifss(const size_t x) {
			// modify this if you want it to work with
			// a lot of whitespace or with interactive problems
			if(it+x>ed) {
				it=copy(it,ed,buf);
				ed=it+read(fd,it,BUFSIZE+buf-it);
				it=buf;
			}
			while(*it<=32){++it;}
		}
		void _flush() {
			ed=buf+read(fd,buf,BUFSIZE);
			// ed=buf+fread(buf,1,BUFSIZE,fd);
			it=buf;
		}
		void ss(){
			while(it!=ed && *it<=32) ++it;
			if(it==ed) {
				_flush();
				ss();
			}
		}
		void scan(char& x){ss();x=*(it++);}
		void scan(string& x){
			x.clear();
			ss();
			do {
				if(it==ed) _flush();
				char * itbg=it;
				while(it!=ed&&*it>32)++it;
				x.append(itbg,it);
			} while(it==ed);
		}
		template<typename T> void scan_u(T& x){
			fifss(32);
			x=0;
			do{
				x=T(x*10+*(it++)-'0');
			} while(*it>32);
		}
		template<typename T> void scan_i(T& x){
			fifss(32);
			x=0;
			bool sign=0;
			if(*it=='-') {
				sign=1;
				++it;
			}
			do{
				x=T(x*10+*(it++)-'0');
			} while(*it>32);
			if(sign)x=T(-x);
		}
		template<typename T> void scan_f(T& x){
			fifss(64);
			x=0;
			bool sign=0;
			if(*it=='-'){
				sign=1;
				++it;
			}
			while(*it>32 && *it!='.'){
				x=x*T(10.0)+T(*(it++)-'0');
			}
			if(*it=='.'){
				++it;
				T e=T(0.1);
				while(*it>32){
					x=x+e*T(*(it++)-'0');
					e=e*T(0.1);
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
		// Scanner(FILE* _fd=stdin):it(0),ed(0),fd(_fd){}
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
		// FILE* fd;
		void fif(const size_t x){
			if(size_t(BUFSIZE+buf-it)<x)flush();
		}
		void print(const char x){fif(1);*(it++)=x;}
		void print(char* const x){
			size_t s = strlen(x);
			if(s>BUFSIZE/2){
				flush();
				write(fd,x,s);
				// fwrite(x,1,s,fd);
			} else {
				fif(s);
				copy(x,x+s,it);
				it+=s;
			}
		}
		void print(const char* const x){print((char*)x);}
		void print(const string& x){
			if(x.size()>BUFSIZE/2){
				flush();
				write(fd,x.data(),x.size());
				// fwrite(x.data(),1,x.size(),fd);
			} else {
				fif(x.size());
				copy(x.begin(),x.end(),it);
				it+=x.size();
			}
		}
		static constexpr size_t siz = 24;
		char b[siz];
		template<typename T> void print_u(T x){
			uint8_t i=siz;
			do {
				b[--i]=char(x%10+'0');
				x=T(x/10);
			}while(x);
			fif(siz-i);
			copy(b+i,b+siz,it);
			it+=siz-i;
		}
		template<typename T> void print_i(T x){
			size_t i=siz;
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
		// Printer(FILE* _fd=stdout):it(buf),fd(_fd){}
		void flush(){
			write(fd,buf,it-buf);
			// fwrite(buf,1,it-buf,fd);
			it=buf;
		}
		void operator()(){}
		template<typename T, typename...R>
			void operator()(T&& a,R&&...rest){
				print(a);
				operator()(rest...);
			}
};

