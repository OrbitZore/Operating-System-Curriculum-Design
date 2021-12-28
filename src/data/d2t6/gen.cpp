#include "cardinal.hpp"
#include <omp.h>
mt19937 mt{random_device{}()};
template<class T=int> using rintT=uniform_int_distribution<T>;
template<class T=double> using rfloatT=uniform_real_distribution<T>;
using ull=unsigned long long;

string prefix="";
int T;
void genin(int T,ostream& os){
	auto r=rintT(-100000000,100000000);
	os << r(mt) << " " << r(mt) << endl;
}

int main(int argc,char *argv[]){
	omp_set_num_threads(8);
	T=10;
	#pragma omp parallel for
	for (int i=0;i<T;i++){
		string ifname="data/"+prefix+to_string(i)+".in";
		string ofname="data/"+prefix+to_string(i)+".ans";
		{
			ofstream of(ifname);
			genin(i,of);
		}
		cerr<<ifname+" done\n";
	}
}
