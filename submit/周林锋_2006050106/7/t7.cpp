#include <bits/stdc++.h>

using namespace std;
#define V_MAX 200
namespace scheduler{
	struct base_scheduler{
		deque<int> v;
		base_scheduler(vector<int> _v):
			v(_v.begin(),_v.end()){
			}
		virtual ~base_scheduler(){}
		virtual pair<int,deque<int>::iterator> take(int pos)=0;
		int deal(int pos){
			int step=0,cnt=0;
			while (v.size()){
				auto p=take(pos);
				pos+=p.first>pos?1:-1;
				step++;
				if (p.second!=v.end()&&*p.second==pos) {
					v.erase(p.second);
					cnt+=exchange(step,0);
				}
			}
			return cnt;
		}
	};
	struct FIFO_scheduler:public base_scheduler{
		using base_scheduler::base_scheduler;
		virtual pair<int,deque<int>::iterator> take(int pos){
			return {v.front(),v.begin()};
		}
	};
	
	struct SSTF_scheduler:public base_scheduler{
		using base_scheduler::base_scheduler;
		virtual pair<int,deque<int>::iterator> take(int pos){
			int minn=0x3f3f3f3f,mini=0;
			for (size_t i=0;i<v.size();i++)
				if (abs(v[i]-pos)<minn){
					minn=abs(v[i]-pos);
					mini=i;
				}
			return {v[mini],v.begin()+mini};
		}
	};
	
	struct SCAN_scheduler:public base_scheduler{
		using base_scheduler::base_scheduler;
		bool f=false;
		virtual pair<int,deque<int>::iterator> take(int pos){
			deque<int>::iterator w=v.end();
			for (auto i=v.begin();i<v.end();i++)
				if (f==(*i>pos))
					if (w==v.end()||(f==(*i<*w))) w=i;
					
			if (w!=v.end()) {
				return {*w,w};
			}else{
				f=!f;
				return take(pos);
			}
		}
	};
	
	struct C_SCAN_scheduler:public base_scheduler{
		using base_scheduler::base_scheduler;
		bool f=false;
		virtual pair<int,deque<int>::iterator> take(int pos){
			if (!f){
				deque<int>::iterator w=v.end();
				for (auto i=v.begin();i<v.end();i++)
					if (f==(*i>pos))
						if (w==v.end()||(f==(*i<*w))) w=i;
						
				if (w!=v.end()) {
					return {*w,w};
				}else {
					f=true;
					return take(pos);
				}
			}else{
				deque<int>::iterator w=v.end();
				for (auto i=v.begin();i<v.end();i++)
					if (w==v.end()||(*i>*w)) w=i;
				if (pos!=*w)
					return {*w,w};
				else{
					f=false;
					return take(pos);
				}
			}
		}
	};
}
using namespace scheduler;

int main(){
	int n,pos;
	cin>>n>>pos;pos=200-pos;
	vector<int> v(n);
	for (auto &i:v) {cin>>i;i=200-i;}
	vector<pair<unique_ptr<base_scheduler>,string>> l;
	l.emplace_back(new FIFO_scheduler(v),"FIFO");
	l.emplace_back(new SSTF_scheduler(v),"SSTF");
	l.emplace_back(new SCAN_scheduler(v),"SCAN");
	l.emplace_back(new C_SCAN_scheduler(v),"C_SCAN");
	for (auto& [schi,name]:l)
		cout << fixed <<  setprecision(1) << schi->deal(pos)/(double)n << " " << name << endl;
}

