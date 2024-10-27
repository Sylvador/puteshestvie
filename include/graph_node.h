#ifndef __GRAPH_NODE__
#define __GRAPH_NODE__

#include <stdbool.h>
#include <stdint.h>

typedef struct Edge {
    uint32_t weight;
    struct GraphNode *adjacent_node;
} Edge;

typedef struct GraphNode {
    uint32_t value;
    Edge *edges;
    size_t edge_count;
} GraphNode;

typedef struct PathNode {
    GraphNode *gn;
    struct PathNode *parent;
    bool marked;
} PathNode;

GraphNode *gn_create(uint32_t value, Edge *edges);
Edge *edge_create(uint32_t weight, GraphNode *adjacent_node);
void gn_print(GraphNode *gn);

PathNode *pn_create(GraphNode *gn, PathNode *parent);
PathNode *bfs(GraphNode *start, GraphNode *target, uint32_t max_step);
void pn_mark_path(PathNode *pn);
void pn_print(PathNode *pn);

#endif
