#include "graph_node.h"
#include "hashtable.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    uint32_t city_count;
    int (*coords)[2];
    uint32_t max_step;
    uint32_t src_dest[2];
} Input;

Input *read_input(char *input_path) {
    FILE *file = fopen(input_path, "r");
    if (file == NULL) {
        puts("Failed to open file.");
        exit(EXIT_FAILURE);
    }

    Input *input = malloc(sizeof(Input));

    fscanf(file, "%u", &input->city_count);

    printf("City count: %u\n", input->city_count);

    input->coords = calloc(input->city_count, sizeof(uint32_t[2]));

    for (uint32_t i = 0; i < input->city_count; ++i) {
        fscanf(file, "%d %d", &input->coords[i][0], &input->coords[i][1]);
        printf("City %u: [%d, %d]\n", i + 1, input->coords[i][0],
               input->coords[i][1]);
    }

    fscanf(file, "%u", &input->max_step);
    fscanf(file, "%u %u", &input->src_dest[0], &input->src_dest[1]);
    printf("Max step: %u\n", input->max_step);
    printf("Src: %u; Dest: %u\n", input->src_dest[0], input->src_dest[1]);
    fclose(file);

    --input->src_dest[0];
    --input->src_dest[1];

    return input;
}

void free_input(Input *input) {
    free(input->coords);
    input->coords = NULL;
    free(input);
}

void weigh_edges_for_city(Hashtable *ht, Input *input, uint8_t node_index) {
    int node_coords[2];
    node_coords[0] = input->coords[node_index][0];
    node_coords[1] = input->coords[node_index][1];

    Edge *edges = calloc(input->city_count - 1, sizeof(Edge));
    GraphNode *node = ht_get_node(ht, node_index);
    node->edges = edges;

    uint8_t edge_index = 0;

    for (uint8_t i = 0; i < input->city_count; ++i) {
        if (i == node_index)
            continue;

        int x_diff = abs(input->coords[i][0] - node_coords[0]);
        int y_diff = abs(input->coords[i][1] - node_coords[1]);
        Edge *edge = &edges[edge_index++];
        edge->adjacent_node = ht_get_node(ht, i);
        edge->weight = x_diff + y_diff;
        node->edge_count++;
    }

    /*gn_print(node);*/
}

Hashtable *init_graph_with_cities(Input *input) {
    Hashtable *ht = ht_init();
    for (uint8_t i = 0; i < input->city_count; ++i) {
        GraphNode *node = gn_create(i, NULL);
        ht_add(ht, i, node);
    }
    return ht;
}

int main(const int argc, const char **argv) {
    char input_n = *argv[1];
    const char *input_format = "inputs/input%c.txt";
    char *input_path = malloc(strlen(input_format));
    sprintf(input_path, input_format, input_n);
    Input *input = read_input(input_path);

    Hashtable *ht = init_graph_with_cities(input);

    for (uint32_t i = 0; i < input->city_count; ++i) {
        weigh_edges_for_city(ht, input, i);
    }

    puts("");
    /*ht_print(ht);*/

    GraphNode *start = ht_get_node(ht, input->src_dest[0]);
    GraphNode *target = ht_get_node(ht, input->src_dest[1]);
    PathNode *path = bfs(start, target, input->max_step);
    pn_print(path);

    free(input);
    return EXIT_SUCCESS;
}
