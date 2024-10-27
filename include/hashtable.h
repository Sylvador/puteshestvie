#ifndef __HASHTABLE__
#define __HASHTABLE__
#include "linked_list.h"
#define HASHTABLE_INIT_CAPACITY 10
#define HASHTABLE_LOAD_FACTOR 0.8

#include <stdint.h>

typedef struct {
    struct ListNode **buckets;
    size_t size;
    size_t capacity;
} Hashtable;
Hashtable *ht_init();
void ht_add(Hashtable *ht, uint32_t key, void *value);
uint32_t ht_resize(Hashtable *ht);
ListNode **ht_allocate_new_buckets(uint32_t new_capacity);
void ht_rehash_and_reinsert(Hashtable *ht, ListNode **new_buckets,
                            uint32_t new_capacity);
void *ht_get_node(Hashtable *ht, uint32_t key);
void ht_free(Hashtable *ht);
void ht_print(Hashtable *ht);

#endif
