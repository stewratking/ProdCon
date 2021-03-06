#include <string.h>
#include <pthread.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <mutex>
#include <math.h>

using namespace std;

//Functions
void setProgramSpecs();

bool isIntegerPrime(int toTest);
int getPrimeNumber();

void *producerFunction(void *ptr);
void *faultyProducerFunction(void *ptr);
void *consumerFunction(void *ptr);
void *fillBuffer(void *ptr);


//Global Variables
int** buffer;
int bufferLength;
const char** gargv;
int gargc;
int itemsPerProducer;
int numberOfProducers;
int numberOfFaulties;
int numberOfConsumers;
bool showDebug = false;
pthread_mutex_t random_mutex;


int main(int argc, char const *argv[]) {
  gargc = argc;
  gargv = argv;

  //get the flags and values from execution input
  setProgramSpecs();
  pthread_t producers[numberOfProducers];
  pthread_t faultiers[numberOfFaulties];
  pthread_t consumers[numberOfConsumers];

  //initialize buffer here

  cout << "Starting Threads..." << endl;
  printf("\n");

  // Create producer threads
  int jj;
  for (int jj = 0; jj < numberOfProducers; jj++) {
    pthread_create(&producers[jj], NULL, *producerFunction, new int(jj));
  }

  // Create faulty producer threads
  for (int jj = numberOfProducers; jj < numberOfFaulties + numberOfProducers; jj++) {
    pthread_create(&faultiers[jj], NULL, *faultyProducerFunction, new int(jj));
  }

  // Create consumers
  for(int jj = numberOfProducers + numberOfFaulties; jj < numberOfConsumers + numberOfFaulties + numberOfProducers; jj++) {
    pthread_create(&consumers[jj], NULL, *consumerFunction, new int(jj));
  }

  // Join with producer threads
  for(jj = 0; jj < numberOfProducers; jj++){
    pthread_join(producers[jj], NULL);
  }

  // Join with faulty producers
  for(jj = 0; jj < numberOfFaulties; jj++){
    pthread_join(faultiers[jj], NULL);
  }

  // Join with consumers
  for(jj = 0; jj < numberOfFaulties; jj++){
    pthread_join(consumers[jj], NULL);
  }

  return 0;
}

/*******************************************************************


*******************************************************************/

//initializes the buffer and simulation stats
void setProgramSpecs() {
  int ii;
  for (ii = 1; ii < gargc; ii++) {
    if (strcmp(gargv[ii], "-n") == 0) {
      itemsPerProducer = atoi(gargv[ii+1]);
      ii++;
    } else if (strcmp(gargv[ii], "-l") == 0) {
      bufferLength = atoi(gargv[ii+1]);
      ii++;
    } else if (strcmp(gargv[ii], "-p") == 0) {
      numberOfProducers = atoi(gargv[ii+1]);
      ii++;
    } else if (strcmp(gargv[ii], "-f") == 0) {
      numberOfFaulties = atoi(gargv[ii+1]);
      ii++;
    } else if (strcmp(gargv[ii], "-c") == 0) {
      numberOfConsumers = atoi(gargv[ii+1]);
      ii++;
    } else if (strcmp(gargv[ii],"-d") == 0) {
      showDebug = true;
    } else {
      ii++;
    }
  }
}

//*******************************************************************
//
//                      Producer Functions
//
//*******************************************************************/

void *producerFunction(void *ptr){
  int primeNumber = getPrimeNumber();

  // TODO: Add to buffer when possible

  // Exit thread
  pthread_exit(0);
}

void *faultyProducerFunction(void *ptr){
  int number = rand() % 999999;

  // TODO: Add to buffer when possible

  // Exit thread
  pthread_exit(0);
}

int getPrimeNumber(){
  // Returns a prime number
  bool done = false;
  int result = -1;

  while(!done){
    // Generate a random positive integer
    result = rand() % 999999;

    // If this int is prime, we are done
    done = isIntegerPrime(result);
  }

  return result;
}

//*******************************************************************
//
//                      Consumer Functions
//
//*******************************************************************/

void *consumerFunction(void *ptr){
  int newItem = -1;

  // TODO: Retreive item from buffer

  bool isPrime = isIntegerPrime(newItem);

  if(isPrime){
    // TODO: Output for case int is prime
  } else {
    // TODO: Output for case int is composite
  }

  pthread_exit(0);
}

void *fillBuffer(void *ptr) {
  int randomNumber;
  int id = *(int*)ptr;      //cast the int object into an id
  delete (int*)ptr;         //delete the int object like I read online

  //use mutex to make sure only one producer is working at a time.
  while(true) {
    pthread_mutex_lock( &random_mutex );
    randomNumber = rand() % 999999 + 2;
    pthread_mutex_unlock( &random_mutex );
  }

  pthread_exit(0);
}

bool isIntegerPrime(int toTest){
  // Function to test the primality of a number.
  // Reference: http://www.codeproject.com/Articles/465041/Primality-Test

  // If less than 2, the number cannot be prime.
  if(toTest < 2) return false;
  if(toTest == 2) return true;

  // Even numbers cannot be prime
  if(toTest % 2 == 0) return false;

  for(int i = 3; i <= sqrt(toTest); i += 2){
        if(toTest %i == 0)
            return false;
    }

    return true;
}
