#include <stdio.h> 
#include <string.h>
#include <pthread.h> 
#include <semaphore.h> 
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>

typedef struct Node Node;

typedef struct Thread Thread;

struct Thread{ 
int id;   
int function_code;    
bool is_active;
};

typedef struct Node Node;

struct Node{ 
int function_code;    
Node *next;   /* ptr to next node the in list */
};

#define NUM_OF_THREADS 10

//Array which holds all the 10 threads in struct form
struct Thread threadArray[NUM_OF_THREADS];

//forder to keep track of function f1(), f2() & f3() execution order
struct Node fun_exe_order;

//declaring a & b in global context
int a, b, a_initial_val, b_initial_val, can_run_command = 0;
pthread_t threads[NUM_OF_THREADS];
Node *head_node = NULL;

//mutex to lock & unlock critical section execution
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

// global condition variable for can_run
pthread_cond_t can_run   = PTHREAD_COND_INITIALIZER;

// global condition variable for thread 0
pthread_cond_t thread_zero   = PTHREAD_COND_INITIALIZER;
// global condition variable for thread 1
pthread_cond_t thread_one   = PTHREAD_COND_INITIALIZER;
// global condition variable for thread 2
pthread_cond_t thread_two   = PTHREAD_COND_INITIALIZER;
// global condition variable for thread 3
pthread_cond_t thread_three   = PTHREAD_COND_INITIALIZER;
// global condition variable for thread 4
pthread_cond_t thread_four   = PTHREAD_COND_INITIALIZER;
// global condition variable for thread 5
pthread_cond_t thread_five   = PTHREAD_COND_INITIALIZER;
// global condition variable for thread 6
pthread_cond_t thread_six   = PTHREAD_COND_INITIALIZER;
// global condition variable for thread 7
pthread_cond_t thread_seven   = PTHREAD_COND_INITIALIZER;
// global condition variable for thread 8
pthread_cond_t thread_eight   = PTHREAD_COND_INITIALIZER;
// global condition variable for thread 9
pthread_cond_t thread_nine   = PTHREAD_COND_INITIALIZER;

void f1 (int x) {a = a + 1; sleep(x); b = b * 1.5;}
void f2 (int x) {b = b + 1; sleep(x); a = a * 1.5;}
void f3 (int x) {a = a * 0.5; sleep(x); b = b * 0.5;}

//Executed function is added into linked list
void update_forder(int func_code){
if(head_node == NULL){
 head_node = (Node *) malloc(sizeof(Node));
}
Node *current = head_node;
while (current->next != NULL) {
     current = current->next;
  }
    /* now we can add a new variable */
    current->next = (Node *) malloc(sizeof(Node));
    current->next->function_code = func_code;
    current->next->next = NULL;
}

void update_thread(Thread thread_data)
{
thread_data.is_active = false;
thread_data.function_code = 0;
}


void wait_on_thread_cond_var(int thread_id){
switch(thread_id){
	case 0:
		pthread_cond_wait(&thread_zero, &lock);
                break;
            case 1:
               pthread_cond_wait(&thread_one, &lock);
                break;
            case 2:
              pthread_cond_wait(&thread_two, &lock);
                break;
	case 3:
               pthread_cond_wait(&thread_three, &lock);
                break;
	case 4:
              pthread_cond_wait(&thread_four, &lock);
                break;
	case 5:
              pthread_cond_wait(&thread_five, &lock);
               break;
	case 6:
              pthread_cond_wait(&thread_six, &lock);
               break;
	case 7:
               pthread_cond_wait(&thread_seven, &lock);
                break;
	case 8:
               pthread_cond_wait(&thread_eight, &lock);
                break;
	case 9:
              pthread_cond_wait(&thread_nine, &lock);
		break;
}
}
void* threadWork(void* thread_param){

	if(can_run_command <= 0){
		printf("looping thread = %ld\n", pthread_self());
	}
	//int arrStart = *((int *) arrayStart);
	
	//Acquires lock
	pthread_mutex_lock(&lock);
printf("acquired lock");
	//waiting for can_run signal
	pthread_cond_wait(&can_run, &lock); 
	printf("waiting on cond var");

	Thread thread_data = *((Thread *) thread_param);
	int func_code = thread_data.function_code;
	printf("func_code %d ", func_code);
	printf("thread No %d ", thread_data.id);		
	
	//waiting for signal on thread's condition variable
	wait_on_thread_cond_var(thread_data.id);
	 
	int rand_sleep_sec = (rand() % 5) + 1; 
	printf("random number = %d\n", rand_sleep_sec);	
	
 	if(func_code == 1){
		f1(rand_sleep_sec);
	}else if(func_code == 2){
		f2(rand_sleep_sec);
	}else if(func_code == 3){
		f3(rand_sleep_sec);
	}
	
	update_forder(func_code);
	update_thread(thread_data);
	//releases the lock
	pthread_mutex_unlock(&lock);	

}
//create a struct element for a thread
Thread* create_thread_data(int threadId){
Thread *new_thread = (Thread *) malloc(sizeof(Thread));
new_thread->id = threadId;
new_thread->is_active = false;
return new_thread;
}
 
void createThreads(){

	printf("thread creating ");
	Thread *thread_node;
	
	for(int threadCount = 0; threadCount < NUM_OF_THREADS; threadCount++){
	thread_node = create_thread_data(threadCount);
		
		//store all the thread data in an array
		threadArray[threadCount] = *thread_node;			
		
	 	//creating threads and make them to run 'threadWork()'
	pthread_create(&threads[threadCount], NULL, &threadWork, thread_node);
			
	}
	
}

int get_node_func(Node *node) {
    int func_code = 0;
    if (node != NULL) {
        printf("%d\n", node->function_code);
        func_code = node->function_code;
    }
return func_code;
}

Thread* get_available_thread(){
Thread* thread_to_return = NULL; 
for(int index=0; index < NUM_OF_THREADS; index++){
	if(! threadArray[index].is_active){
		thread_to_return = &threadArray[index];
		break;
	}
}
return thread_to_return;	
}

void join_threads(){
for(int threadCount = 0; threadCount < NUM_OF_THREADS; threadCount++)		{	pthread_join(threads[threadCount], NULL);

}
}
void execute_forder_function(){
Node* current_node = head_node;
int function_code = 0;
while(current_node != NULL){
	function_code = get_node_func(current_node);
	if(function_code == 1){
		f1(0);
	}else if(function_code == 2){
		f2(0);
	}else if(function_code == 3){
		f3(0);
	}
	current_node = current_node->next;
	}	
	
}

void signal_thread_cond_var(int thread_id){
switch(thread_id){
	case 0:
		printf("signaling thread 0 with status %d ",pthread_cond_signal(&thread_zero));
                break;
            case 1:
               printf("signaling thread 1 with status %d ",pthread_cond_signal(&thread_one));
                break;
            case 2:
               printf("signaling thread 0 with status %d ",pthread_cond_signal(&thread_two));
                break;
	case 3:
                 pthread_cond_signal(&thread_three);
                break;
	case 4:
                pthread_cond_signal(&thread_four);
                break;
	case 5:
               pthread_cond_signal(&thread_five);
               break;
	case 6:
               pthread_cond_signal(&thread_six);
               break;
	case 7:
               pthread_cond_signal(&thread_seven);
                break;
	case 8:
               pthread_cond_signal(&thread_eight);
                break;
	case 9:
               pthread_cond_signal(&thread_nine);
		break;
}
}

/* executes the command from input file */
void execute_command(char command_name, int command_val){
int slepp_sec;
char command_var;

	if(command_name == 'a'){
	printf("a command_val %d", command_val);
printf("a can_run_command %d",can_run_command);
		if(can_run_command > 0 && command_val > 0){
			printf("gonna call f()");

		//check for available thread
		Thread *thread_to_work = get_available_thread();
		if(thread_to_work == NULL){
		printf("Command %c %d ignored as thread not available",command_name, command_val );
		
		}else{			
			//broadcasting signal to all threads which are waiting 				for can_run condition
	printf("got thread to work %d", thread_to_work->id);
	printf("broadcasting signal %d", pthread_cond_broadcast(&can_run));
		
			//send signal to the waiting thread 
			//to execute critical section
			thread_to_work->is_active = true;
			thread_to_work->function_code = command_val;

			//send signal to waiting thread 
			//which is waiting on thread specific conditional var
			signal_thread_cond_var(thread_to_work->id);
			
		}
		}
	}else if(command_name == 'r'){
		printf("r here ------");
		can_run_command = 1;
printf("r can_run_command %d ------",can_run_command);
	}else if(command_name == 'u'){
		printf("u here");
		can_run_command = 0;
	}else if(command_name == 's'){
		printf("s here");
		if(command_val > 0 && command_val < 30){
			sleep(command_val);
		}else{
	printf("sleep command ignored as sleep sec is %d ",command_val);
		}
	}else if(command_name == 't'){
		printf("t here");
		join_threads();
		printf(" a & b value executed by threads %d %d", a, b );
		//re-initialize a & b
		a = a_initial_val;
		b = b_initial_val;
		execute_forder_function();
           	printf(" a & b value executed in forder order %d %d", a, b );
           	//break;
	}

}

int main(){

char fileName[25] = "";
printf("Enter command file");
scanf("%s",&fileName);

//first node in forder
 head_node = (struct Node*)malloc(sizeof(struct Node)); 
 
//read file
FILE *file  = fopen(fileName, "r"); 

 if (file == NULL) {
        printf("Error: Invalid file name");
        exit(1);
   }

char line[128];
int lineCount = 0;
char command_name;
int command_val;

    
//processes the file line by line

	//retrieves a and b value
	if(lineCount == 0){
	 if(fscanf(file, "%d %d ",&a, &b) == 2){
		
	printf("%d ",a);
	printf("%d ",b);
	a_initial_val = a;
	b_initial_val = b;

	printf("%d ",a_initial_val);
	printf("%d ",b_initial_val);

	createThreads();
	lineCount++;
	}
	}
	if(lineCount > 0){
	//retrieves remaining commands
	int word_count = fscanf(file, "%c %d ",&command_name, &command_val); 
	while( word_count == 2 ||  word_count == 1){
		
		//printf("command_name  %c ",command_name);
		//printf("command_val  %d ", command_val);
		
		execute_command(command_name, command_val);
		word_count = fscanf(file, "%c %d ",&command_name, &command_val); 

	}
	        
     }
    	fclose(file);
    return 0;
}



