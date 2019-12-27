#include "header.h"

/* Function to initialize the histogram so
 * the elements of the count array are set to 0.
 */
struct histogram* initializeHistogram() {
  struct histogram* final_result = (struct histogram*)malloc(sizeof(struct histogram));
  for(int i=0; i<26; i++) {
    final_result->count[i] = 0;
  }
  return final_result;
}

/* Initialize the info struct for producer thread
 * Sets producer line number to 0 and stores file path
 */
struct info* initializeInfo(char* c) {
  struct info *info = (struct info*)malloc(sizeof(struct info));
  info->path_to_text = (char*)malloc(sizeof(char)*strlen(c) + 2);
  strcpy(info->path_to_text, c);
  info->p_line_num = 0;
  return info;
};

/* Update the count array of histogram h by 1 at index i.
 */
void updateHistogram(int i, struct histogram* h) {
  h->count[i]++;
}

/* Function to write the final result into the ouput
 * file named "result.txt".
 */
int make_result(struct histogram* h) {
  // Character array contains all the alpabets
  char arr[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
              'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u',
              'v', 'w', 'x', 'y', 'z'};

  // Open result.txt file
  FILE *fptr = fopen("result.txt", "a");

  // Check if the file is opened or not
  if (fptr == NULL)
  {
      printf("Could not open file");
  }

  // Write correct results with the alpabets
  for(int i=0; i<26; i++) {
    fprintf(fptr, "%c: %d\n", arr[i], h->count[i]);
  }

  // If a file with the same name already exists, its contents are
  // discarded and the file is treated as a new empty file
  fclose(fopen("result.txt", "w"));
  return 0;
}

/* Function to create a new node.
 * Takes in an int v to initialize the node's value.
 * returns struct Node* containing the nodes id information
 * and node's next value set to NULL.
 */
 struct Node* createNode(char* v) {
  struct Node* node = (struct Node*) malloc(sizeof(struct Node));
  node->value = (char*)malloc(sizeof(char)*strlen(v) + 2);
  strcpy(node->value, v);
  node->next = NULL;
  return node;
}

/* Function to initialize an empty queue.
 * Head and tail of the queue are set to NULL.
 * The empty queue of type struct Queue* is returned.
 */
struct Queue* initializeQueue(int m) {
  struct Queue* queue = (struct Queue*)malloc(sizeof(struct Queue));
  queue->head = NULL;
  queue->tail = NULL;
  queue->node_count = 0;
  queue->max = m;
  queue->is_full = 0;
  return queue;
}

// Add a node with value v to the tail of the queue q.
void insert(char* v, struct Queue* q) {
   struct Node* n = createNode(v);
   // If q is empty, set head and tail to node n
   if(q->head == NULL) {
     q->head = n;
     q->tail = n;
     q->node_count++;
   }

   // otherwise insert node n at the tail of the list
   else {
     // first check if max is reached
     if(q->max == -1 || q->max == 0 || q->node_count <= q->max) {
       q->tail->next = n;
       q->tail = n; // n is now the tail of the list
       q->node_count++; // Update node count
     } else {
       // Queue is full
       q->is_full = 1;
     }
   }
 }

/* Given a queue q, this function deletes the node
 * at the head of the list and returns it.
 */
struct Node* delete(struct Queue* q) {
   // Check if head is null, and return NULL if true
   if(q->head == NULL) {
     return NULL;
   }
   struct Node* n = q->head;
   //free(n->value);
   //free(n);
   // Set new head to the node after original head
   q->head = q->head->next;
   // If new head is now NULL, change tail to be NULL as well
   if(q->head == NULL) {
     q->tail = NULL;
   }
   // Update node count and is_full to false
   q->node_count--;
   q->is_full = 0;
   return n;
 }
