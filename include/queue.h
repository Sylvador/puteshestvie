#ifndef __QUEUE__
#define __QUEUE__

#include <stddef.h>
typedef struct QueueNode {
    void *item;
    struct QueueNode *next;
} QueueNode;

typedef struct Queue {
    size_t size;
    QueueNode *head;
    QueueNode *tail;
} Queue;

QueueNode *qn_create(void *item);

Queue *q_init();
void q_enqueue(Queue *q, void *item);
void *q_dequeue(Queue *q);
void q_free(Queue *q);

#endif
