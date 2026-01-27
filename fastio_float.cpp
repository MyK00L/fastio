
template<size_t BUFSIZE>
template<typename T>
void Scanner<BUFSIZE>::scan_f(T& x) noexcept {
	fifss();
	x=0;
	bool sign=0;
	if(*it=='-'){
		sign=1;
		++it;
	}
	while(*it>32 && *it!='.'){
		x=x*T(10.0)+T(*it++&0xf);
	}
	if(*it=='.'){
		++it;
		T e=T(0.1);
		while(*it>32){
			x=x+e*T(*it++&0xf);
			e=e*T(0.1);
		};
	}
	if(sign)x=-x;
}
template<size_t BUFSIZE>
void Scanner<BUFSIZE>::scan(float& x) noexcept {double d; scan_f(d); x=d;}
template<size_t BUFSIZE>
void Scanner<BUFSIZE>::scan(double& x) noexcept {scan_f(x);}
template<size_t BUFSIZE>
void Scanner<BUFSIZE>::scan(long double& x) noexcept {scan_f(x);}

template<size_t BUFSIZE>
void Printer<BUFSIZE>::print_f(const double x) noexcept {
	const uint64_t d = __builtin_bit_cast(uint64_t,x);
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
template<size_t BUFSIZE>
void Printer<BUFSIZE>::print(const float x) noexcept {print_f(x);}
template<size_t BUFSIZE>
void Printer<BUFSIZE>::print(const double x) noexcept {print_f(x);}
template<size_t BUFSIZE>
void Printer<BUFSIZE>::print(const long double x) noexcept {print_f(x);}

