
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

int dining_time;
int philosopher_count;

//chopstick in struct format.
struct Chopstick{    
  bool is_busy;    
};

struct Chopstick chopsticks[];

//mutex to lock & unlock critical section execution
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t chopstick_lock = PTHREAD_MUTEX_INITIALIZER;

// global condition variable for can_run
pthread_cond_t thread_run_cond   = PTHREAD_COND_INITIALIZER;

// global condition variable for can_run
pthread_cond_t chopstick_cond   = PTHREAD_COND_INITIALIZER;

void get_chopstick(int threadID,int chopstick_id){
int chopstick_1_id = threadID; 
int chopstick_2_id = (threadID + 1) % philosopher_count;

	//locking chopsticks
 	pthread_mutex_lock(&chopstick_lock);
	//checking for chopstick availability
	if(chopsticks[chopstick_1_id].is_busy 
	|| chopsticks[chopstick_2_id].is_busy){
		//As chopstick not available, philosopher is waiting
		printf("Philosopher %d is waiting as,", threadID);
		printf("Chopstick %d and %d are not availabe.. \n",chopstick_1_id, chopstick_2_id);
	 	pthread_cond_wait(&chopstick_cond, &chopstick_lock);
	}
	
	printf("Philosopher %d is picking up Chopstick %d and %d.. \n",threadID,chopstick_1_id, chopstick_2_id);
	chopsticks[threadID].is_busy = true;
	chopsticks[(threadID + 1) % philosopher_count].is_busy = true;

	printf("Philosopher %d is busy in having food..\n",threadID);
	int sleep_sec = (rand() % 3) + 1;
	sleep(sleep_sec);
	pthread_mutex_unlock(&chopstick_lock);
}

void release_chopstick(int threadID){
	int chopstick_1_id = threadID; 
	int chopstick_2_id = (threadID + 1) % philosopher_count;
	printf("Philosopher %d is releasing Chopsticks %d and %d.. \n",threadID,chopstick_1_id, chopstick_2_id);
	chopsticks[chopstick_1_id].is_busy = false;
	chopsticks[chopstick_2_id].is_busy = false;
	pthread_cond_broadcast(&chopstick_cond);	
}

void* threadWork(void* arg){

char thread_name[20];

int threadID = *((int *) arg);
snprintf(thread_name,20, "Philosopher_%d", threadID);
int chopstick_id = threadID;

pthread_mutex_lock(&lock);

/*printf("Philosopher %s is waiting to get message from main thread \n", thread_name); */

pthread_cond_wait(&thread_run_cond, &lock);

printf("Philosopher %s received message from main thread \n", thread_name);
	
int rand_sleep_no = (rand() % 999) + 1;
struct timespec ts;

ts.tv_sec = 1;
ts.tv_nsec = 10000 * rand_sleep_no;
printf(" philosopher %d going for a nano sleep \n",threadID);
nanosleep (&ts, NULL);

get_chopstick(threadID, chopstick_id);

release_chopstick(threadID);
pthread_mutex_unlock(&lock);

}

int main(){

printf("Enter number of philosopher going to have dinner..\n");
scanf("%d",&philosopher_count);

pthread_t threads[philosopher_count];
chopsticks[philosopher_count];

//creating threads and make them to run 
for(int threadCount = 0; threadCount < philosopher_count; threadCount++){
	pthread_attr_t threadAttr;
	pthread_attr_init(&threadAttr);	
		
	int *arg = malloc(sizeof(*arg));
	*arg = threadCount;	

	//creating threads and make them to run 'threadWork()'
	pthread_create(&threads[threadCount], &threadAttr, &threadWork,arg);

} 

printf("Enter time of dining in seconds for all the philosoper.. \n");
scanf("%d",&dining_time);

pthread_cond_broadcast(&thread_run_cond);
sleep(dining_time);
printf("Main thread done with sleep..\n");

//End of thread execution

for(int threadCount = 0; threadCount < philosopher_count; threadCount++){
	pthread_join(threads[threadCount], NULL);
	printf("Main thread is ending/joining the philosopher %d..\n", threadCount);
}

return 0;
}