#include "header.h"

// Declare shared queue, final result histogram, and producer info
struct Queue* shared_q;
struct histogram* final_result;
struct info* info;

int main(int argc, char *argv[]) {

  info = initializeInfo(argv[2]);

  // If queue is unbounded, set max # of nodes to -1
  int m = -1;
  output_log = 0;

  // Check if number of arguments is atleast 3 arguments
  if(argc < 3) {
    printf("Not enough arguments provided.\n");
    exit(0);
  }

  // Store number of consumer threads
  int consumers = atoi(argv[1]);
  num_consumers = consumers;

  // Check if number of consumer threads is atleast 1
  if(consumers < 1) {
    printf("Not enough consumers.\n");
    exit(0);
  }

  // Check if there are 4 or more arguments
  if(argc >= 4) {
    // -p: Generate log
    if(strcmp(argv[3],"-p") == 0) {
      printf("Generating log...\n");
      output_log = 1;
      fptr = fopen("log.txt", "w");
      fclose(fptr);
    }
    // -b: Use bounded buffer
    else if(strcmp(argv[3],"-b") == 0 && argc > 4) {
      printf("Using bounded buffer...\n");
      m = atoi(argv[4]);
    }
    // -bp: Generate log and use bounded buffer
    else if(strcmp(argv[3],"-bp") == 0 && argc > 4) {
      m = atoi(argv[4]);
      if(m < 1) {
        printf("Not a valid assignment for queue size.\n");
        exit(0);
      }
      printf("Generating log & bounded buffer...\n");
      output_log = 1;
      fptr = fopen("log.txt", "w");
      fclose(fptr);
    }
    // Invalid command line arguments
    else {
      printf("Could not process arguments.\n");
      exit(0);
    }
  }

  // Initialize pthread mutex and cond
  mutex = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));
  cond = (pthread_cond_t*) malloc(sizeof(pthread_cond_t));
  cond_full = (pthread_cond_t*) malloc(sizeof(pthread_cond_t));
  pthread_mutex_init(mutex, NULL);
  pthread_cond_init(cond, NULL);
  pthread_cond_init(cond_full, NULL);

  // Intialize array to store producer and consumer threads
  pthread_t condPool[consumers+1];

  // Initialze the histogram
  final_result = initializeHistogram();

  // Initialze the shared queue
  shared_q = initializeQueue(m);

  // Begin the producer thread
  pthread_create(&condPool[0], NULL, producer_thread, info);

  // Initialize the c_args args struct, which stores the consumer ids
  args = (c_args**)malloc(sizeof(c_args*)*consumers);
  for(int i=0; i < consumers; i++) {
		 args[i] = (c_args*)malloc(sizeof(c_args) + 1);
	}

  // Set the id for each consumer
  for (int i = 0; i < consumers; i++){
    args[i]->id = i;
  }

  // Begin consumer threads
  for(int i=0; i < consumers; i++) {
		pthread_create(&condPool[i+1], NULL, consumer_thread, (void*)args[i]);
	}

  // Join all threads together
  for(int i=0; i<consumers+1; i++) {
    pthread_join(condPool[i], NULL);
  }

  // Output final result to file
  make_result(final_result);

  return 0;
}
