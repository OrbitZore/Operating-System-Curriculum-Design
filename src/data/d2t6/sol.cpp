#include <bits/stdc++.h>
using namespace std;
array<int,2> lens;
auto &n=lens[0],&m=lens[1];
template<class T,size_t id>
struct vec:public vector<T>{
	constexpr static int& len=lens[id];
	vec():vector<T>(len){
		if constexpr (is_same_v<T,int>) fill(this->begin(),this->end(),0);
	}
};
using rvec=vec<int,1>;
struct bank{
	rvec resource,available;
	vec<rvec,0> claim,allocation;
	bank add(int id,const rvec& a){
		bank b=*this;
		for (int i=0;i<m;i++){
			b.available[i]-=a[i];
			b.allocation[id][i]+=a[i];
		}
		return b;
	}
	bool vaild(){return ((bank)*this)._vaild();}
	bool _vaild(){
		auto check=[&](int id){
			for (int i=0;i<m;i++)
				if (claim[id][i]-allocation[id][i]>available[i])
					return false;
			return true;
		};
		auto allzero=[&](auto &ma,int id){
			for (int i=0;i<m;i++)
				if (ma[id][i]!=0) return false;
			return true;
		};
		while ([&](){
			bool f=false;
			for (int i=0;i<n;i++)
				if (!allzero(allocation,i))
					if (check(i)){
						f=true;
						for (int j=0;j<m;j++){
							available[j]+=exchange(allocation[i][j],0);
						}
					}
			return f;
		}());
		for (int i=0;i<n;i++)
			if (!allzero(allocation,i)) return false;
		return true;
	}
};
int main(int argc,char *argv[]){
	cerr<<"Please Enter Process Count and Resource Count"<<endl;
	cin>>n>>m;
	bank b;
	cerr<<"Please Enter Resource Vector" << endl;
	for (int i=0;i<m;i++) cin>>b.resource[i];
	b.available=b.resource;
	cerr<<"Please Enter Claim Matrix" << endl;
	for (auto &i:b.claim) for (auto &j:i) cin>>j;
	while (cin){
		cerr<<"Please Enter Process ID"<<endl;
		int i;cin>>i;
		cerr<<"Please Enter Need Resource Vector"<<endl;
		rvec need;
		for (int i=0;i<m;i++)
			cin>>need[i];
		if (!cin) return 0;
		auto bn=b.add(i,need);
		if (bn.vaild()){
			cout << "Accept" << endl;
			b=bn;
		}else{
			cout << "Refuse" << endl;
		}
	}
}
