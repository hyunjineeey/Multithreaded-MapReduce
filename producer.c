#include "header.h"

// Declare global structs
extern struct Queue* shared_q;
extern struct info* info;

// Track producer line number
int p_line_num = 0;

// pthread.h included in header.h

// Function to read line by line from text file and
// insert each line to shared queue
void * producer_thread(void *data) {
    // Set eof to false as end of file has not been reached
    eof = -1;

    struct info *info = data;
    char cwd[256];
    char *cur_path = getcwd(cwd, sizeof(cwd));

    // Append current path and text path
    strncat(cur_path, info->path_to_text, 100);

    // Open the text file with appended path
    FILE *fp = fopen(cur_path, "r");

    // Check if the file is opened or not
    if (fp == NULL) {
     perror("Error opening file: ");
     exit(1);
    }

    // Max number of characters in a line is 1024
    char line[1024];

    // Open log.txt file
    pthread_mutex_lock(mutex);
    if(output_log == 1) {
      fptr = fopen("log.txt", "a+");
      // Check if the file is opened or not
      if (fptr == NULL) {
          printf("Could not open file");
      }
      fprintf(fptr, "producer\n");
      fclose(fptr);
    }
    pthread_mutex_unlock(mutex);
    // Read a line
    while (1) {
      pthread_mutex_lock(mutex); // start critical section
      while(shared_q->is_full == 1) {
    		pthread_cond_wait(cond_full,mutex);
    	}
      // EOF is reached, send signal to consumers to stop
      if(fgets (line, sizeof(line), fp) == NULL) {
        eof = num_consumers;
        pthread_cond_signal((cond));
        pthread_mutex_unlock(mutex); // end critical section
        fclose(fp);
        pthread_exit(NULL);
        break;
      }

      // Log producer line number
      if(output_log == 1) {
        fptr = fopen("log.txt", "a+");
        fprintf(fptr, "producer: %d\n", info->p_line_num);
        fclose(fptr);
      }
      insert(line, shared_q); // insert line into queue, critical section
      info->p_line_num++;
      pthread_cond_signal((cond));
      pthread_mutex_unlock(mutex); // end critical section
      // Use random sleep to increases concurrency of threads
      usleep(rand() % 500);
    }

    // Close the text file
    fclose(fp);

    // Exit producer thread
    pthread_exit(NULL);
}
