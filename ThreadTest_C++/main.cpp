#include	<stdio.h>
#include	<thread>
#include	<mutex>
#include	<windows.h>

std::mutex mux;
int n = 0;
unsigned int __stdcall Thread(){
	for(int i = 0;i < 100;i++)
	{
		std::lock_guard<std::mutex> lock(mux);
		printf("Thread %d\n",n++);
	}
	return 0;
}

void main(void){
	auto th = std::thread([] { Thread(); });
	for(int i = 0;i < 100;i++)
	{
		std::lock_guard<std::mutex> lock(mux);
		printf("Main %d\n",n++);
		Sleep(1);
	}
	th.join();
	return;
}