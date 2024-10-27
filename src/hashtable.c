#include "hashtable.h"
#include "graph_node.h"
#include "linked_list.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Hashtable *ht_init() {
    Hashtable *ht = malloc(sizeof(Hashtable));
    ht->buckets = calloc(HASHTABLE_INIT_CAPACITY, sizeof(ListNode *));
    ht->size = 0;
    ht->capacity = HASHTABLE_INIT_CAPACITY;
    return ht;
}

ListNode **ht_allocate_new_buckets(uint32_t capacity) {
    return calloc(capacity, sizeof(ListNode *));
}

void ht_rehash_and_reinsert(Hashtable *ht, ListNode **new_buckets,
                            uint32_t new_capacity) {
    for (size_t i = 0; i < ht->capacity; ++i) {
        ListNode *current = ht->buckets[i];

        while (current != NULL) {
            ListNode *next = current->next;
            uint32_t new_index = current->key % new_capacity;
            current->next = new_buckets[new_index];
            new_buckets[new_index] = current;
            current = next;
        }
    }
}

uint32_t ht_resize(Hashtable *ht) {
    uint32_t new_capacity = ht->capacity * 2;

    ListNode **new_buckets = ht_allocate_new_buckets(new_capacity);

    ht_rehash_and_reinsert(ht, new_buckets, new_capacity);

    free(ht->buckets);

    ht->buckets = new_buckets;
    ht->capacity = new_capacity;

    return new_capacity;
}

void ht_add(Hashtable *ht, uint32_t key, void *value) {
    if (ht->size >= ht->capacity * HASHTABLE_LOAD_FACTOR) {
        ht_resize(ht);
    }

    uint32_t index = key % ht->capacity;
    ListNode *current = ht->buckets[index];
    ListNode *ln = ln_create(key, value, NULL);

    while (current != NULL) {
        if (current->key == key) {
            printf("Error: Node with key %u already exists in the hashmap",
                   key);
            exit(EXIT_FAILURE);
        }
        current = current->next;
    }

    ln->next = ht->buckets[index];
    ht->buckets[index] = ln;
    ht->size++;
}

void *ht_get_node(Hashtable *ht, uint32_t key) {
    uint32_t index = key % ht->capacity;

    ListNode *current = ht->buckets[index];

    if (current == NULL) {
        return NULL;
    }

    while (current->key != key) {
        current = current->next;
    }

    return current->value;
}

static void print_bucket(ListNode *bucket) {
    ListNode *node = bucket;
    while (node != NULL) {
        printf("Node: %u\n", ((GraphNode *)(node->value))->value);
        node = node->next;
    }
}

static void free_bucket(ListNode *bucket) {
    ListNode *node = bucket;
    while (node != NULL) {
        ListNode *next = node->next;
        node->value = NULL;
        free(node);
        node = next;
    }
}

void ht_free(Hashtable *ht) {
    for (uint32_t i = 0; i < ht->capacity; ++i) {
        ListNode *bucket = ht->buckets[i];
        free_bucket(bucket);
    }
}

void ht_print(Hashtable *ht) {
    puts("Hashtable:");
    printf("size: %zu;\ncapacity: %zu\n\n", ht->size, ht->capacity);
    for (uint32_t i = 0; i < ht->capacity; ++i) {
        ListNode *bucket = ht->buckets[i];
        printf("Bucket %d.\n", i);
        if (bucket != NULL) {
            /*printf("Bucket %d.\n", i);*/
            print_bucket(bucket);
            puts("");
        } else {
            printf("<Empty>\n\n");
        }
    }
}
