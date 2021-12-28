#include <iostream>
#include <cstdio>
#include <random>
#include <thread>
#include <atomic>
#include <semaphore.h>
using namespace std;
namespace z3475{
struct sem{
    sem_t a;
    sem(int v=0){sem_init(&a,0,v);}
	~sem(){sem_destroy(&a);}
	sem& operator++(){
		sem_post(&a);
		return *this;
	}
	sem& operator--(){
		sem_wait(&a);
		return *this;
	}
};

struct mutex{
	sem a{1};
	void lock(){--a;}
	void unlock(){++a;}
};

struct unique_mutex{
	mutex &m;
	unique_mutex(mutex &m):m(m){m.lock();}
	~unique_mutex(){m.unlock();}
};

random_device rd;
mt19937 mt(rd());

void random_sleepms(int l,int r){
	this_thread::sleep_for(uniform_int_distribution(l,r)(mt)*1ms);
}
vector<sem> forks;
vector<sem> pans;
void philosopher(){
	
}
using namespace z3475;
int main(){
	vector<jthread> vthread;
	file f;
	for (int i=0;i<5;i++)
		vthread.emplace_back(jthread(reader,ref(f)));
	for (int i=0;i<3;i++)
		vthread.emplace_back(jthread(writer,ref(f)));
}
