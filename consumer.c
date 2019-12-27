#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

// Declare global shared queue and histogram
extern struct Queue* shared_q;
extern struct histogram* final_result;

// Count consumer line number
int c_line_num = 0;

/* The consumer_thread extracts the individual lines from the
 * shared queue and then deletes them from the shared queue.
 * While the end of file notification has not been recieved
 * and the queue is not empty, the consumer counts the first
 * character of each word in the line and updates the gloabl histogram.
 */
void * consumer_thread(void *data) {
  // Struct that stores the consumer id number
  c_args* c = (c_args*) data;

  // Critical section as file pointer is global
  pthread_mutex_lock(mutex);
  if(output_log == 1) {
    // Open log.txt file
    fptr = fopen("log.txt", "a+");

    // Check if the file is opened or not
    if (fptr == NULL) {
        printf("Could not open file");
    }

    // Log the consumer id number
    fprintf(fptr, "consumer %d\n", c->id);
    fclose(fptr);
  }
  pthread_mutex_unlock(mutex);

  // Continues to read each line in the shared queue while the queue is
  // not empty and the end of file notification is not recieved.
  while(1) {
    // Set mutex lock, as consumer is accessing the shared queue
    // which is a global struct
    pthread_mutex_lock(mutex);

    // If the shared queue is empty and eof notification has not been recieved,
    // then wait for the producer thread to add an item to the queue.
    while(shared_q->node_count == 0 && eof == -1) {
  		pthread_cond_wait(cond,mutex);
  	}

    // If the queue is empty and EOF notification has been recieved,
    // then consumer terminates.
    if(shared_q->node_count == 0 && eof != -1) {
      // Each consumer decrements the eof counter before exiting
      eof--;
      c_line_num = -1;

      // Log the consumer terminating with '-1'
      if(output_log == 1) {
        fptr = fopen("log.txt", "a+");
        fprintf(fptr, "consumer %d: %d\n", c->id, c_line_num);
        fclose(fptr);
      }

      // Broadcast signal to other consumers, unlock mutex, and terminate
      pthread_cond_signal(cond);
      pthread_mutex_unlock(mutex);
      pthread_exit(NULL);
      break;
    }

    c_line_num++;

    // Delete and store the head of the shared queue
    struct Node* n = delete(shared_q); // Critical Section

    // Log the line number consumer is reading
    if(output_log == 1) {
      fptr = fopen("log.txt", "a+");
      fprintf(fptr, "consumer %d: %d\n", c->id, c_line_num);
      fclose(fptr);
    }

    char* s = (char*)malloc(sizeof(char)*strlen(n->value) + 2);
    strcpy(s, n->value);

    char* first_letters[strlen(s)]; // Array to store first letters

    // Initialize array to empty spaces
    for (int i = 0; i < strlen(s); i++){
      first_letters[i] = " ";
    }

    // The first character in the file will be the first character of the word
    // so first is set to 1 (true).
    int first = 1;

    for(int i=0; i<strlen(s); i++) {
      // If the current character is a non alphabet character, then the next
      // character that will be read will be the first character of a word
      if(!(s[i]>='a' && s[i]<='z') && !(s[i]>='A' && s[i]<='Z')) {
        // Set first to true, so in the next iteration, the histogram
        // will be updated
        first = 1;
      }
      // If first is true then check if it is a valid character
      // and update histogram
      else if(strncmp(&s[i]," ",1) != 0 && first == 1) {
        if(s[i] >= 'a' && s[i] <= 'z') {
          updateHistogram(s[i]-'a',final_result);
        } else if(s[i] >= 'A' && s[i] <= 'Z') {
          updateHistogram(s[i]-'A',final_result);
        }
        // Set first to false
        first = 0;
      }
    }
    // Signal full signal to producer, for bounded buffer
    pthread_cond_signal(cond_full);
    pthread_mutex_unlock(mutex);
    // Use random sleep to increases concurrency of threads
    usleep(rand() % 500);
  }
  // Exit consumer thread
  pthread_exit(NULL);
}
