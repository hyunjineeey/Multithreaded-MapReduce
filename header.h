/*
header.h, header for all source files
it will:
- structure definition
- global variable, lock declaration (extern)
- function declarations
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <ctype.h>
#include <signal.h>
#include <errno.h>

#ifndef _HEADER_H_
#define _HEADER_H_

// store node information for queue in a struct
struct Node {
  char* value;
  struct Node* next;
};

// Implement queue as a struct, tracking head and tail node
struct Queue {
  // node at head of linked list
  struct Node *head;
  // node at tail of linked list
  struct Node *tail;
  // max number of nodes in a queue
  // if unbounded, set to -1
  int max;
  // current number of nodes in Queue
  int node_count;
  // if node count is equal to max, set to 1
  int is_full;
};

// Struct to store id for each consumer thread
typedef struct consumer_id {
    int id;
} c_args;

c_args **args;

// Store information for producer
struct info {
    char* path_to_text;
    int p_line_num; // Producer line number
};

// Implement result histogram as a struct, tracking first letter count
struct histogram {
    int count[26];
};

// Initialize the result histogram
struct histogram* initializeHistogram();

// Update the count of characters in the histogram
void updateHistogram(int i, struct histogram* h);

// Create the result.txt file with the first letter frequency
int make_result(struct histogram* h);

// Creates a new node with the value v
struct Node* createNode(char* v);

// Initializes a queue with max number of nodes m
struct Queue* initializeQueue(int m);

// Intializes the struct storing information for producer
struct info* initializeInfo(char* c);

// Inserts a new nodes into queue q with value v
void insert(char* v, struct Queue* q);

// Deletes the node at the head of the queue
struct Node* delete(struct Queue* q);

// Reads a file and passes each line of the file into the shared queue
void * producer_thread(void *path_to_text);

// Gets each line from the shared queue and updates the histogram with the
// frequency count of the first letter of each word
void * consumer_thread(void *id);

pthread_mutex_t* mutex;

pthread_cond_t* cond;

pthread_cond_t* cond_full;

int eof;

int output_log;

int num_consumers;

FILE *fptr;

#endif
