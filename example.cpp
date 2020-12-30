#include "fastio.cpp"

#include <vector>
#include <algorithm>
using namespace std;

int main(){
	Scanner<> scan;
	Printer<> print;
	double x;
	scan(x);
	print("x = ",x,'\n');
	print.flush();
	int n,m;
	scan(n,m);
	vector<int> v(n);
	scan(v);
	reverse(v.begin(),v.end());
	print(v,'\n',m);
	return 0;
}

