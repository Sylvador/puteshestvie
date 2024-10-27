#include "linked_list.h"
#include <stdint.h>
#include <stdlib.h>

ListNode *ln_create(uint32_t key, void *value, ListNode *parent) {
    ListNode *node = malloc(sizeof(ListNode));
    node->key = key;
    node->value = value;
    if (parent != NULL) {
        parent->next = node;
    }

    return node;
}
