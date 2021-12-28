# 实验4-Windows 线程的互斥与同步

## 实验目的

- 回顾操作系统进程、线程的有关概念，加深对Windows线程的理解。(2)了解互斥体对象，利用互斥与同步操作编写生产者
- 消费者问题的并发程序，加深对P(即semWait)、V(即semSignal)原语以及利用P、V原语进行进程间同步与互斥操作的理解。

## 实验内容

生产者消费者问题

1. 创建一个“Win32ConsolApplication”工程，然后拷贝清单5-1中的程序，编译成可执行文件。
2. 在"命令提示符"窗口运行1中生成的可执行文件，列出运行结果。
3. 仔细阅读源程序，找出创建线程的WINDOWSAPI函数，回答下列问题：线程的第一个执行函数是什么（从哪里开始执行）？它位于创建线程的API函数的第几个参数中？
4. 修改清单5-1中的程序，调整生产者线程和消费者线程的个数，使得消费者数目大与生产者，看看结果有何不同。察看运行结果，从中你可以得出什么结论？
5. 修改清单5-1中的程序，按程序注释中的说明修改信号量EmptySemaphore的初始化方法，看看结果有何不同。
6. 根据4的结果，并查看MSDN，回答下列问题
   1. CreateMutex中有几个参数，各代表什么含义。
   2. CreateSemaphore中有几个参数，各代表什么含义，信号量的初值在第几个参数中。
   3. 程序中P、V原语所对应的实际WindowsAPI函数是什么，写出这几条语句。
   4. CreateMutex能用CreateSemaphore替代吗？尝试修改程序5-1，将信号量Mutex完全用CreateSemaphore及相关函数实现。写出要修改的语句。

## 参考程序清单

```cpp
#include <windows.h>
#include <iostream>
const unsigned short SIZE_OF_BUFFER = 2;  //缓冲区长度
unsigned short ProductID = 0;             //产品号
unsigned short ConsumeID = 0;             //将被消耗的产品号
unsigned short in = 0;          //产品进缓冲区时的缓冲区下标
unsigned short out = 0;         //产品出缓冲区时的缓冲区下标
int buffer[SIZE_OF_BUFFER];     //缓冲区是个循环队列
bool p_ccontinue = true;        //控制程序结束
HANDLE Mutex;                   //用于线程间的互斥
HANDLE FullSemaphore;           //当缓冲区满时迫使生产者等待
HANDLE EmptySemaphore;          //当缓冲区空时迫使消费者等待
DWORD WINAPI Producer(LPVOID);  //生产者线程
DWORD WINAPI Consumer(LPVOID);  //消费者线程
int main() {
  //创建各个互斥信号
  //注意，互斥信号量和同步信号量的定义方法不同，互斥信号量调用的是 CreateMutex
  //函数，同
  步信号量调用的是 CreateSemaphore 函数，函数的返回值都是句柄。 Mutex =
      CreateMutex(NULL, FALSE, NULL);
  EmptySemaphore = CreateSemaphore(NULL, SIZE_OF_BUFFER, SIZE_OF_BUFFER, NULL);
  //将上句做如下修改，看看结果会怎样
  // EmptySemaphore = CreateSemaphore(NULL,0,SIZE_OF_BUFFER-1,NULL);
  FullSemaphore = CreateSemaphore(NULL, 0, SIZE_OF_BUFFER, NULL);
  //调整下面的数值，可以发现，当生产者个数多于消费者个数时，
  //生产速度快，生产者经常等待消费者；反之，消费者经常等待
  const unsigned short PRODUCERS_COUNT = 3;  //生产者的个数
  const unsigned short CONSUMERS_COUNT = 1;  //消费者的个数
  //总的线程数
  const unsigned short THREADS_COUNT = PRODUCERS_COUNT + CONSUMERS_COUNT;
  HANDLE hThreads[THREADS_COUNT];     //各线程的 handle
  DWORD producerID[PRODUCERS_COUNT];  //生产者线程的标识符
  DWORD consumerID[CONSUMERS_COUNT];  //消费者线程的标识符
  //创建生产者线程
  for (int i = 0; i < PRODUCERS_COUNT; ++i) {
    hThreads[i] = CreateThread(NULL, 0, Producer, NULL, 0, &producerID[i]);
    if (hThreads[i] == NULL) return -1;
  }
  //创建消费者线程
  for (i = 0; i < CONSUMERS_COUNT; ++i) {
    hThreads[PRODUCERS_COUNT + i] =
        CreateThread(NULL, 0, Consumer, NULL, 0, &consumerID[i]);
    if (hThreads[i] == NULL) return -1;
  }
  while (p_ccontinue) {
    if (getchar()) {  //按回车后终止程序运行
      p_ccontinue = false;
    }
  }
  return 0;
}
//生产一个产品。简单模拟了一下，仅输出新产品的 ID 号
void Produce() {
  std::cout << std::endl << "Producing " << ++ProductID << " ... ";
  std::cout << "Succeed" << std::endl;
}
//把新生产的产品放入缓冲区
void Append() {
  std::cerr << "Appending a product ... ";
  buffer[in] = ProductID;
  in = (in + 1) % SIZE_OF_BUFFER;
  std::cerr << "Succeed" << std::endl;
  //输出缓冲区当前的状态
  for (int i = 0; i < SIZE_OF_BUFFER; ++i) {
    std::cout << i << ": " << buffer[i];
    if (i == in) std::cout << " <-- 生产";
    if (i == out) std::cout << " <-- 消费";
    std::cout << std::endl;
  }
}
//从缓冲区中取出一个产品
void Take() {
  std::cerr << "Taking a product ... ";
  ConsumeID = buffer[out];
  buffer[out] = 0;
  out = (out + 1) % SIZE_OF_BUFFER;
  std::cerr << "Succeed" << std::endl;
  //输出缓冲区当前的状态
  for (int i = 0; i < SIZE_OF_BUFFER; ++i) {
    std::cout << i << ": " << buffer[i];
    if (i == in) std::cout << " <-- 生产";
    if (i == out) std::cout << " <-- 消费";
    std::cout << std::endl;
  }
}
//消耗一个产品
void Consume() {
  std::cout << "Consuming " << ConsumeID << " ... ";
  std::cout << "Succeed" << std::endl;
}
//生产者
DWORD WINAPI Producer(LPVOID lpPara) {
  while (p_ccontinue) {
    WaitForSingleObject(EmptySemaphore, INFINITE);  // p(empty);
    WaitForSingleObject(Mutex, INFINITE);           // p(mutex);
    Produce();
    Append();
    Sleep(1500);
    ReleaseMutex(Mutex);                       // V(mutex);
    ReleaseSemaphore(FullSemaphore, 1, NULL);  // V(full);
  }
  return 0;
}
//消费者
DWORD WINAPI Consumer(LPVOID lpPara) {
  while (p_ccontinue) {
    WaitForSingleObject(FullSemaphore, INFINITE);  // P(full);
    WaitForSingleObject(Mutex, INFINITE);          // P(mutex);
    Take();
    Consume();
    Sleep(1500);
    ReleaseMutex(Mutex);                        // V(mutex);
    ReleaseSemaphore(EmptySemaphore, 1, NULL);  // V(empty);
  }
  return 0;
}
```

## 实验结果与分析


## 小结和心得

