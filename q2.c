
/*	Name : Fahad Qayyum
 * 	EECS login: fahadq
 * 	YU ID : 215287253
 *  Assignment # 2 Question 2
 * 	Course Code : EECS 3221
 * 	Course Name : Operating systems*/
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include "Queue.h"

void *longScheduler(void *args);  // thread representing long term scheduler
void * shortScheduler(void *args);  // thread representing short term scheduler
Queue *newQueue, *readyQueue;   // creating 2 queues 
bool button;			                //true for long term scheduler and false for short term
pthread_t longTid, shortTid;  // for long and short threads
pthread_mutex_t lock;					  // for locking and unlocking
pthread_cond_t check;            // for checkition checking


int main()
{
	int counter;
	button = true;
	srand(time(NULL)); 
  newQueue = makeQueue();
	readyQueue = makeQueue();

  //-------------Populating the NEW queue-------------------

	for(counter = 1; counter <= 100; counter++){
		int time = rand() % 30 + 1;
		Task *Task = makeTask(counter,time);
		enqueue(newQueue, Task);	
	}
	printf("\n");      
 
 //--------------Thread handling and manipulation------------
 
	pthread_mutex_init(&lock, NULL);  // defining mutex lock
	pthread_cond_init(&check, NULL);  // defining cond check
  pthread_mutex_lock(&lock);  // locking the thread 
	pthread_create(&longTid, NULL, longScheduler, NULL);  // creating long scheduler thread
	pthread_create(&shortTid, NULL, shortScheduler, NULL);  // creating short scheduler thread
	pthread_cond_signal(&check);  // checking signal
	pthread_mutex_unlock(&lock);  // mutex unlocking
	pthread_join(longTid, NULL);  // join for long thread    
	pthread_join(shortTid, NULL);  // join for short thread   
	pthread_mutex_destroy(&lock);  // destroying the lock
	pthread_cond_destroy(&check);  // destroying the check 
     
	return 0;
}

 //--------------Long Term Scheduler--------------

void *longScheduler(void *args)
{  
	printf("Long scheduler STARTED!\n");
	while(sizeOf(newQueue) > 0 || sizeOf(readyQueue) > 0){  // do until something in the queue
  pthread_mutex_lock(&lock);	  // getting locks
		if(!button){
			printf("Long scheduler PAUSED!\n");			
			pthread_cond_wait(&check, &lock);  // conditional wait
			printf("Long scheduler in ACTION!\n");
		}
  button = false;
	if(sizeOf(newQueue) > 0){  
		while(sizeOf(newQueue) > 0 && sizeOf(readyQueue) < 5){  // dequeue enqueue check
			Task *Task = dequeue(newQueue);
			enqueue(readyQueue,Task);
			printf("Task ID : %d and TIME : %d, is moved to READY queue\n", Task->pid, Task->time);	
		}	
	}
	pthread_cond_signal(&check);  // signalizing conditional
	pthread_mutex_unlock(&lock);  // unlocking mutex
	}
	printf("Long scheduler STOPPED!\n\n");
	printf("HURRAY! ALL PROCESSES TERMINATED SUCCESSFULLY!\n");  // mission accomplished
 
	return NULL;
}

 //--------------Short Term Scheduler--------------

void * shortScheduler(void *args)
{
  int counter;
	printf("Short scheduler STARTED!\n");
	while(true){
		pthread_mutex_lock(&lock);  // locking mutex
		if(button){
			pthread_cond_wait(&check, &lock);  // conditional wait
			printf("Short scheduler in Action!\n");	
		}
		button = true;
		for(counter = 0; sizeOf(readyQueue) > 0 && counter < 5; counter++){  // enqueing 5 processes in READY
			Task *Task = dequeue(readyQueue);
      printf("---------------------------------------\n");
      printf("Currently Executing:- \n");
			printf("Process ID : %d with TIME : %d\n", Task->pid, Task->time);
      printf("---------------------------------------\n");
			Task->time -= 2;  // decreasing time for every process by 2
			if(Task->time > 0){
				enqueue(readyQueue, Task);  // enqueing back again
				printf("Process ID: %d moved back!\n", Task->pid);	
			}else {
				printf("Process ID : %d TERMINATED!\n", Task->pid);
			}	
		}
		pthread_cond_signal(&check);  // signalizing the check
		pthread_mutex_unlock(&lock);  // releasing the locks
   if(sizeOf(readyQueue) == 0){
			printf("Short scheduler STOPPED!\n");  // stop if the queue gets empty
			break;	
		}
	}	
	return NULL;
}
