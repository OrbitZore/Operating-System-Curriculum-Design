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

struct file{
	int readers=0;
	mutex writeMutex;
	mutex readersMutex;
	void process_read(int i){
		printf("reader i:%d reading\n",i);
		random_sleepms(0,1000);
		printf("reader i:%d read done\n",i);
	}
	void process_write(int i){
		printf("writer i:%d writing\n",i);
		random_sleepms(0,1000);
		printf("writer i:%d write done\n",i);
	}
	void read(int i){
		{unique_mutex u(readersMutex);
			if (!readers++)
				writeMutex.lock();
		}
		process_read(i);
		{unique_mutex u(readersMutex);
			if (!--readers)
				writeMutex.unlock();
		}
	}
	void write(int i){
		unique_mutex u(writeMutex);
		process_write(i);
	}
};

void reader(file &f){
	static atomic<int> i=0;
	int w=i++;
	for (int i=0;i<4;i++){
		f.read(w);
		random_sleepms(0,1000);
	}
}
void writer(file &f){
	static atomic<int> i=0; 
	int w=i++;                 		
	for (int i=0;i<2;i++){
		f.write(w);
		random_sleepms(0,2000);
	}
}
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
