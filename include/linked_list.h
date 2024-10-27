#ifndef __LINKED_LIST__
#define __LINKED_LIST__

#include <stdint.h>
typedef struct ListNode {
    uint32_t key;
    void *value;
    struct ListNode *next;
} ListNode;

ListNode *ln_create(uint32_t key, void *value, ListNode *parent);

#endif
