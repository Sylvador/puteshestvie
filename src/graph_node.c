#include "graph_node.h"
#include "hashtable.h"
#include "queue.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

GraphNode *gn_create(uint32_t value, Edge *edges) {
    GraphNode *gn = malloc(sizeof(GraphNode));
    gn->value = value;
    gn->edges = edges;
    gn->edge_count = 0;
    return gn;
}

Edge *edge_create(uint32_t weight, GraphNode *adjacent_node) {
    Edge *edge = malloc(sizeof(Edge));
    edge->adjacent_node = adjacent_node;
    edge->weight = weight;
    return edge;
}

void gn_print(GraphNode *gn) {
    printf("GraphNode %u:\n", gn->value);
    for (size_t i = 0; i < gn->edge_count; ++i) {
        Edge edge = gn->edges[i];
        printf("--%u-->%u\n", edge.weight, edge.adjacent_node->value);
    }
}

PathNode *pn_create(GraphNode *gn, PathNode *parent) {
    PathNode *pn = malloc(sizeof(PathNode));
    pn->parent = parent;
    pn->gn = gn;
    pn->marked = false;
    return pn;
}

void pn_mark_path(PathNode *pn) {
    while (pn != NULL) {
        pn->marked = true;
        pn = pn->parent;
    }
}

static void free_unmarked_pns(Queue *q) {
    QueueNode *node = q->head;
    while (node != NULL) {
        PathNode *pn = node->item;
        if (!pn->marked) {
            free(pn);
        }
        node = node->next;
    }
}

PathNode *bfs(GraphNode *start, GraphNode *target, uint32_t max_step) {
    Queue *queue = q_init();
    PathNode *path = pn_create(start, NULL);
    q_enqueue(queue, path);

    Hashtable *visited = ht_init();
    ht_add(visited, start->value, start);

    PathNode *result = NULL;

    Queue *allocated_nodes = q_init();

    while (queue->head != NULL) {
        PathNode *current_path = q_dequeue(queue);
        GraphNode *gn = current_path->gn;
        for (size_t i = 0; i < gn->edge_count; ++i) {
            Edge *edge = &gn->edges[i];

            if (edge->weight > max_step)
                continue;
            if (ht_get_node(visited, edge->adjacent_node->value) != NULL)
                continue;
            ht_add(visited, edge->adjacent_node->value, edge->adjacent_node);

            PathNode *updated_path =
                pn_create(edge->adjacent_node, current_path);
            q_enqueue(allocated_nodes, updated_path);

            if (edge->adjacent_node == target) {
                result = updated_path;
                pn_mark_path(result);
                break;
            }
            q_enqueue(queue, updated_path);
        }
    }

    ht_free(visited);
    free_unmarked_pns(allocated_nodes);

    return result;
}

void pn_print(PathNode *pn) {
    if (pn == NULL) {
        printf("Path not found.\nSteps: -1");
        return;
    }
    PathNode *reversed_pn = pn_create(pn->gn, NULL);
    PathNode *reversed_parent = pn->parent;

    while (reversed_parent != NULL) {
        reversed_pn = pn_create(reversed_parent->gn, reversed_pn);
        reversed_parent = reversed_parent->parent;
    }

    pn = reversed_pn;
    uint32_t steps = 0;

    printf("Path: ");
    while (true) {
        printf("%u", pn->gn->value + 1);
        if (pn->parent != NULL)
            printf("->");
        else
            break;
        ++steps;
        pn = pn->parent;
    }
    printf("\nSteps: %u", steps);
}
