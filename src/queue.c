#include "queue.h"
#include <stdlib.h>

QueueNode *qn_create(void *item) {
    QueueNode *qn = malloc(sizeof(QueueNode));
    qn->item = item;
    qn->next = NULL;
    return qn;
}

Queue *q_init() {
    Queue *q = malloc(sizeof(Queue));
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

void q_enqueue(Queue *q, void *item) {
    q->size++;
    QueueNode *qn = qn_create(item);
    if (q->tail == NULL) {
        q->head = qn;
    } else {
        q->tail->next = qn;
    }
    q->tail = qn;
}

void *q_dequeue(Queue *q) {
    if (q->head == NULL) {
        return NULL;
    }

    q->size--;
    QueueNode *qn = q->head;
    void *item = qn->item;

    if (q->head == q->tail) {
        q->tail = NULL;
    }

    q->head = q->head->next;

    free(qn);

    return item;
}

void q_free(Queue *q) {
    QueueNode *qn = q->head;
    while (qn != NULL) {
        QueueNode *next = qn->next;
        qn->next = NULL;
        free(qn);
        qn = next;
    }
    q->head = NULL;
    q->tail = NULL;
}
