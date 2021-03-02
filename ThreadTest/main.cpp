#include	<stdio.h>
#include	<process.h>
#include	<windows.h>

unsigned int __stdcall Thread(void* pData){
	for(int i = 0;i < 100;i++)
	{
		printf("Thread %d\n",i);
	}
	_endthreadex(NULL);
	return 0;
}

void main(void){
	HANDLE hThread1 = (HANDLE)_beginthreadex(NULL,0,Thread,"Thread1",0,NULL);
	for(int i = 0;i < 100;i++)
	{
		printf("Main %d\n",i);
	}
	WaitForSingleObject(hThread1, INFINITE);
	CloseHandle(hThread1);
	return;
}