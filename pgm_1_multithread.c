#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<pthread.h>
#include<string.h>

//Since each number must be marked as ‘prime’ or non-prime number,Storing number in struct format.
struct number{    
int value;    
bool isPrime;    
}number;

//Declaring the numbers array as global variable. So that it can be accessed in thread function.
struct number numberStructArray[];

int dividedArraySize = 0, currentNumber, primeLimit;

void* threadWork(void* arrayStart){
	int subArrayIndex;
	int value;
	int arrStart = *((int *) arrayStart);
	int arrayEnd = arrStart + dividedArraySize-1;
	printf("running thread id = %ld\n", pthread_self()); 

	for(subArrayIndex = arrStart; subArrayIndex < arrayEnd; subArrayIndex++)
	{
	  value = numberStructArray[subArrayIndex].value;

	  	// set 'isprime = false' for all the multiples of the 			'currentNumber'
		if(value % currentNumber == 0)
		  {
			numberStructArray[subArrayIndex].isPrime = false;
		  }
	}
		
	pthread_exit(0);
}

void main()
{
	  int index, threadLimit; 

	  printf("\n Enter Prime numbers limit: \n");
	  scanf("%d", &primeLimit);
	  
	  printf("\n Enter number of threads: \n");
	  scanf("%d", &threadLimit);

	//Initializing array in main() which will hold 
	//the numbers in struct form	 
	 numberStructArray[primeLimit-1]; 
	  
	//generate and store numbers from 2 to N  
	  for(index = 2; index <= primeLimit; index++){
 		numberStructArray[index-2].value = index;
	 	numberStructArray[index-2].isPrime = true;
		}
	
	int divisorCount, i, threadCount;

	//iterate through all the numbers in the array 
	for(index = 0; index < primeLimit-1; index++)
	  {
	   divisorCount = 0;
	   currentNumber = numberStructArray[index].value;

	//if 'isPrime' is true, check for prime character
	  if(numberStructArray[index].isPrime)
	    {
			
		for (i = 2; i <= currentNumber/2; i++)
		{
		if(currentNumber % i == 0)
		    {
			divisorCount++;
			break;
		     }
		}
		//Set isPrime = false as it has divisorCount greater than 0
		if(divisorCount > 0 )
		{
		   numberStructArray[index].isPrime = false;
		} 
				
		pthread_t threads[threadLimit];
		pthread_attr_t threadAttr;
		
		int arrayStart = index+1, arrayEnd;
		
		dividedArraySize = ( (primeLimit-1) - index ) / 			threadLimit;	
	//creating threads and make them to run 'threadWork()'
	for(threadCount = 0; threadCount < threadLimit; threadCount++){
		pthread_attr_init(&threadAttr);			
			
		arrayEnd = arrayStart + dividedArraySize-1;
			
		int *arg = malloc(sizeof(*arg));
		*arg = arrayStart;
	pthread_create(&threads[threadCount], &threadAttr, &threadWork, arg);
	
		arrayStart = arrayEnd +1;
			
		}
	//End of thread execution
	for(threadCount = 0; threadCount < threadLimit; threadCount++){

		pthread_join(threads[threadCount], NULL);

		}
	    }
	  }
//Print final output
for(index = 0; index < primeLimit-1; index++)
  	{
	if(!numberStructArray[index].isPrime){
	 printf(" %d\t", numberStructArray[index].value);
	 printf(" %s\n", numberStructArray[index].isPrime ? "true" : "false");
	}
		
  	} 
}


