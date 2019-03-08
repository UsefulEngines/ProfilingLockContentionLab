// lockContention.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <windows.h> 

using namespace std;

//#define SYNCHRONIZATION_MUTEX 1
#define SYNCHRONIZATION_CRITICAL_SECTION 1

//Header declarations
long WINAPI executeThread(long lParam);
long primes_num();

#ifdef   SYNCHRONIZATION_MUTEX
	volatile HANDLE hMutex;	
#endif
#ifdef   SYNCHRONIZATION_CRITICAL_SECTION
	CRITICAL_SECTION g_CriticalSection;
#endif

CONST int MAX_THREADS = 4;
CONST int PRIME_RANGE_BASE = 10000;

int main(void)
{
  HANDLE hThread[MAX_THREADS];
  DWORD dwID[MAX_THREADS];
  DWORD dwRetVal = 0;
  int param[MAX_THREADS];

  std::cout << "Program start with id: " << GetCurrentThreadId() << std::endl;

  #ifdef   SYNCHRONIZATION_MUTEX
  	hMutex = CreateMutex(NULL,FALSE,NULL);
  #endif
  #ifdef   SYNCHRONIZATION_CRITICAL_SECTION
  	InitializeCriticalSection(&g_CriticalSection);
  #endif

  //release the threads.
  for(int i = 0; i< MAX_THREADS;i++){
	param[i]=i+1;
  	hThread[i] = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)executeThread,&param[i],0,&dwID[i]);
    std::cout << "Thread " <<  i+1  << " with id " << dwID[i] <<" created." << std::endl;

  }
 
  //wait for all threads to complete before continuing
  dwRetVal = WaitForMultipleObjects(MAX_THREADS, hThread, TRUE, INFINITE);
  
  //Display a message: Wait state has finished
  std::cout << "All threads have completed." << std::endl << std::endl;
 
  //close handles
  for(int i = 0; i< MAX_THREADS;i++)
  	CloseHandle(hThread[i]);

  system("pause");

  //end the main function
  return 0;
}

long WINAPI executeThread(long lParam)
{
  int threadNum = *((int*)lParam);
 
  primes_num();

  std::cout << "Thread " << threadNum << " finished calculating prime numbers." << std::endl;
  return 0;
}

long primes_num()
{
  int isprime;
    
  for( int i = 0 ; i <= 5 ; i++)
  {
  	 #ifdef   SYNCHRONIZATION_MUTEX
	 	WaitForSingleObject(hMutex,INFINITE);
	 #endif
     #ifdef   SYNCHRONIZATION_CRITICAL_SECTION
	 	EnterCriticalSection(&g_CriticalSection);
	 #endif
	 for(int i=2;i<PRIME_RANGE_BASE;i++)
	 {
		isprime  = 1;
		for( int j = 2 ; j <= i ; j++)
	     {
	        if( i == j)
	           continue;
	        else if( i % j == 0)
	        	isprime = 0;
	     }
		 if(isprime){
	    	// This number is prime
		 }
	 }	
	 #ifdef   SYNCHRONIZATION_MUTEX
	 	ReleaseMutex(hMutex);
	 #endif
	 #ifdef   SYNCHRONIZATION_CRITICAL_SECTION
	 	LeaveCriticalSection(&g_CriticalSection);
	 #endif
  }

  return 0;
}

