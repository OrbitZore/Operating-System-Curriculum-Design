#include <bits/stdc++.h>

using namespace std;
#define VMEM_N 320

random_device rd;

mt19937 mt(rd());

vector<int> gen(){//生成指令序列
	auto rall=uniform_int_distribution(0,VMEM_N-1);
	vector<int> w;
	w.push_back(rall(mt));
	for (int j=0;w.size()<320;){
		w.push_back([&](){
			if (j==0||j==2){
				return w.back()+1;
			}else if (j==1&&0<w.back()-1){
				return uniform_int_distribution(0,w.back()-1)(mt);
			}else if (j==3&&w.back()+1<VMEM_N-1){
				return uniform_int_distribution(w.back()+1,VMEM_N-1)(mt);
			}else return rall(mt);
		}()%VMEM_N);
		if (++j>=4) j=0;
	}
	return w;
}
namespace memscheduler{
	constexpr int n=4;
	struct base_memscheduler{//基本抽象页面置换调度器
		int clocks;//当前时钟
		array<int,n> pages;//内存中的页面
		vector<int> v;//指令序列
		base_memscheduler(const vector<int>& v):v(v){}
		virtual ~base_memscheduler(){}
		virtual int choosePage()=0;//选中要置换的一页
		virtual void usePage(int index){}//使用内存中的一页
		virtual void loadPage(int index,int page){//加载一页
			pages[index]=page;
		}
		int deal(){//处理指令序列
			clocks=0;
			int cnt=0;//加载页面计数
			pages.fill(-1);
			while (v.size()){
				cnt+=[&](){
				auto addr=v.front();v.erase(v.begin());//读取指令
				for (int i=0;i<n;i++)//检查当前内存中有无指令对应的页
					if (addr/10==pages[i]){
						usePage(i);
						return false;//无需加载
					}
				for (int i=0;i<n;i++)//置换掉内存中无效页面
					if (i==-1){
						loadPage(i,addr/10);
						usePage(i);
						return true;//加载一个页面
					}
				int index=choosePage();//选中即将置换掉的页
				loadPage(index,addr/10);//加载页
				usePage(index);//使用页
				return true;//加载一个页面
				}();
				clocks++;
			}
			return cnt;//返回计数
		}
	};
	struct OPT_memscheduler:public base_memscheduler{//最佳置换算法(OPT)
		using base_memscheduler::base_memscheduler;
		int choosePage(){
			array<int,n> w;//w[i]表示内存中第i个页所指向的虚拟地址页最近使用是第几次
			for (int i=0;i<n;i++)
				w[i]=find_if(v.begin(),v.end(),[&](auto& j){
					return j/10==pages[i];
				})-v.begin();
			return max_element(w.begin(),w.end())-w.begin();//取最近最晚使用的页面
		}
	};
	struct FIFO_memscheduler:public base_memscheduler{//先进先出置换算法(FIFO)
		using base_memscheduler::base_memscheduler;
		array<int,n> pageClk;//pageClk表示内存中页面上一次调入的时间戳
		void loadPage(int index,int page){
			pages[index]=page;
			pageClk[index]=clocks;
		}
		int choosePage(){
			return min_element(pageClk.begin(),pageClk.end())-pageClk.begin();//取最早调入的页面
		}
	};
	struct LRU_memscheduler:public base_memscheduler{//最近最久未用置换算法(LRU)
		using base_memscheduler::base_memscheduler;
		array<int,n> pageClk;//pageClk表示内存中页面上一次调入的时间戳
		void usePage(int index){
			pageClk[index]=clocks;
		}
		int choosePage(){
			return min_element(pageClk.begin(),pageClk.end())-pageClk.begin();//取内存中最近最久未使用的页
		}
	};
	struct CLK_memscheduler:public base_memscheduler{//时钟算法(Clock)
		using base_memscheduler::base_memscheduler;
		array<unsigned char,n> buffer{};int i=0;//初始化全部置0
		void usePage(int index){
			buffer[index]=1;//刚使用置1
		}
		void loadPage(int index,int page){
			buffer[index]=1;//加载置1
			pages[index]=page;
		}
		int choosePage(){
			while (exchange(buffer[i],0)) {//旋转
				if (++i==n) i=0;
			}
			return i;
		}
	};
	struct CLK2_memscheduler:public base_memscheduler{//时钟算法(Clock)
		using base_memscheduler::base_memscheduler;
		array<unsigned char,n> buffer{};int i=0;//初始化全部置0
		void usePage(int index){
			buffer[index]=1;//刚使用置1
		}
		void loadPage(int index,int page){
			buffer[index]=1;//加载置1
			pages[index]=page;
		}
		int choosePage(){
			if (buffer[i]==0) return i;
			int j=i+1;
			for (;i!=j;j=j+1==n?0:j+1){
				if (buffer[j]==0) return j;
			}
			if (i==j) buffer.fill(0);
			return i;
		}
	};
}
using namespace memscheduler;
vector<int> mkdata(){
	vector<unique_ptr<base_memscheduler>> l;
	auto data=gen();
	l.emplace_back(new OPT_memscheduler(data));
	l.emplace_back(new FIFO_memscheduler(data));
	l.emplace_back(new LRU_memscheduler(data));
	l.emplace_back(new CLK_memscheduler(data));
	l.emplace_back(new CLK2_memscheduler(data));
    vector<int> v(l.size());
    for (int i=0;i<l.size();i++){
		v[i]=l[i]->deal();
	}
    return v;
}
int main(){
	vector<pair<int,string>> l(5,pair<int,string>{0,""});
	l[0].second=("OPT");
	l[1].second=("FIFO");
	l[2].second=("LRU");
	l[3].second=("CLK");
	l[4].second=("CLK2");
    for (int i=0;i<1000;i++){
        auto v=mkdata();
        for (int j=0;j<v.size();j++)
            l[j].first+=v[j];
    }
	for (auto& [schi,name]:l){
        cout<<schi<<" "<<name<<endl;
	}
}