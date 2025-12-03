#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "minheap-2.h"

void mark_seats(unsigned **, NODE_T **, NODE_T);

int main(int argc, char **argv) {
	char *line_ptr = NULL;
	size_t n = 0, num_char, rows = 0, cols, num_nodes = 0;
	unsigned **seats = NULL;
	NODE_T **nodes = NULL, **node_map = NULL;
	FILE *in_file = fopen(argv[1], "r");

	while ( (num_char = getline(&line_ptr, &n, in_file)) != -1) {
		cols = num_char - 1;
		seats = realloc(seats, (rows + 1) * sizeof(*seats));
		seats[rows] = calloc(cols, sizeof(**seats));
		node_map = realloc(node_map, (rows + 1) * sizeof(*node_map));
		node_map[rows] = malloc(cols * sizeof(**node_map));
		
		for (size_t i = 0; i < cols; i++) {
			node_map[rows][i].parent = NULL;
			node_map[rows][i].sym = line_ptr[i];
			node_map[rows][i].pos.x = i;
			node_map[rows][i].pos.y = rows;

			if (line_ptr[i] == 'S') {
				node_map[rows][i].dist = 0;
				node_map[rows][i].dir = East;
			}
			else {
				node_map[rows][i].dist = (size_t)-1;
				node_map[rows][i].dir = None;
			}

			insert(&nodes, &num_nodes, node_map[rows] + i);
		}

		rows++;
	}

	fclose(in_file);
	free(line_ptr);

	NODE_T *current_node, *node_ptr, *sol_node;

	while (num_nodes > 0) {
		current_node = pop_root(&nodes, &num_nodes);
		
		printf("Num nodes = %lu\n", num_nodes);

		if (current_node->sym == 'E') {
			sol_node = current_node;
			
			continue;
		}

		switch (current_node->dir)
		{
		case North:
			node_ptr = node_map[current_node->pos.y - 1] + current_node->pos.x;
			
			if (node_ptr->sym != '#') {
				if (node_ptr->dist > current_node->dist + 1) {
					node_ptr->dist = current_node->dist + 1;
					node_ptr->parent = current_node;
					node_ptr->dir = North;
				}
			}

			node_ptr = node_map[current_node->pos.y] + current_node->pos.x + 1;
			
			if (node_ptr->sym != '#') {
				if (node_ptr->dist > current_node->dist + 1001) {
					node_ptr->dist = current_node->dist + 1001;
					node_ptr->parent = current_node;
					node_ptr->dir = East;
				}
			}
			
			node_ptr = node_map[current_node->pos.y] + current_node->pos.x - 1;
			
			if (node_ptr->sym != '#') {
				if (node_ptr->dist > current_node->dist + 1001) {
					node_ptr->dist = current_node->dist + 1001;
					node_ptr->parent = current_node;
					node_ptr->dir = West;
				}
			}

			break;
		case East:
			node_ptr = node_map[current_node->pos.y] + current_node->pos.x + 1;
			
			if (node_ptr->sym != '#') {
				if (node_ptr->dist > current_node->dist + 1) {
					node_ptr->dist = current_node->dist + 1;
					node_ptr->parent = current_node;
					node_ptr->dir = East;
				}
			}

			node_ptr = node_map[current_node->pos.y - 1] + current_node->pos.x;
			
			if (node_ptr->sym != '#') {
				if (node_ptr->dist > current_node->dist + 1001) {
					node_ptr->dist = current_node->dist + 1001;
					node_ptr->parent = current_node;
					node_ptr->dir = North;
				}
			}
			
			node_ptr = node_map[current_node->pos.y + 1] + current_node->pos.x;
			
			if (node_ptr->sym != '#') {
				if (node_ptr->dist > current_node->dist + 1001) {
					node_ptr->dist = current_node->dist + 1001;
					node_ptr->parent = current_node;
					node_ptr->dir = South;
				}
			}

			break;
		case South:
			node_ptr = node_map[current_node->pos.y + 1] + current_node->pos.x;
			
			if (node_ptr->sym != '#') {
				if (node_ptr->dist > current_node->dist + 1) {
					node_ptr->dist = current_node->dist + 1;
					node_ptr->parent = current_node;
					node_ptr->dir = South;
				}
			}

			node_ptr = node_map[current_node->pos.y] + current_node->pos.x + 1;
			
			if (node_ptr->sym != '#') {
				if (node_ptr->dist > current_node->dist + 1001) {
					node_ptr->dist = current_node->dist + 1001;
					node_ptr->parent = current_node;
					node_ptr->dir = East;
				}
			}
			
			node_ptr = node_map[current_node->pos.y] + current_node->pos.x - 1;
			
			if (node_ptr->sym != '#') {
				if (node_ptr->dist > current_node->dist + 1001) {
					node_ptr->dist = current_node->dist + 1001;
					node_ptr->parent = current_node;
					node_ptr->dir = West;
				}
			}

			break;
		case West:
			node_ptr = node_map[current_node->pos.y] + current_node->pos.x - 1;
			
			if (node_ptr->sym != '#') {
				if (node_ptr->dist > current_node->dist + 1) {
					node_ptr->dist = current_node->dist + 1;
					node_ptr->parent = current_node;
					node_ptr->dir = West;
				}
			}

			node_ptr = node_map[current_node->pos.y - 1] + current_node->pos.x;
			
			if (node_ptr->sym != '#') {
				if (node_ptr->dist > current_node->dist + 1001) {
					node_ptr->dist = current_node->dist + 1001;
					node_ptr->parent = current_node;
					node_ptr->dir = North;
				}
			}
			
			node_ptr = node_map[current_node->pos.y + 1] + current_node->pos.x;
			
			if (node_ptr->sym != '#') {
				if (node_ptr->dist > current_node->dist + 1001) {
					node_ptr->dist = current_node->dist + 1001;
					node_ptr->parent = current_node;
					node_ptr->dir = South;
				}
			}

			break;
		}

		resort(nodes, num_nodes);
	}

	free(nodes);

	size_t num_seats = 0;

	printf("\n");

	mark_seats(seats, node_map, *sol_node);

	for (size_t i = 0; i < rows; i++) {
		for (size_t j = 0; j < cols; j++) {
			num_seats += seats[i][j];
			
			if (seats[i][j]) printf("O");
			else (printf("%c", node_map[i][j].sym));
		}

		free(seats[i]);
		free(node_map[i]);

		printf("\n");
	}

	free(node_map);
	free(seats);

	printf("\nSeats = %lu\n", num_seats);

	return 0;
}

void mark_seats(unsigned **seats, NODE_T **node_map, NODE_T node) {
	seats[node.pos.y][node.pos.x] = 1;

	if (node_map[node.pos.y - 1][node.pos.x].dist < node.dist) mark_seats(seats, node_map, node_map[node.pos.y - 1][node.pos.x]);
	if (node_map[node.pos.y + 1][node.pos.x].dist < node.dist) mark_seats(seats, node_map, node_map[node.pos.y + 1][node.pos.x]);
	if (node_map[node.pos.y][node.pos.x - 1].dist < node.dist) mark_seats(seats, node_map, node_map[node.pos.y][node.pos.x - 1]);
	if (node_map[node.pos.y][node.pos.x + 1].dist < node.dist) mark_seats(seats, node_map, node_map[node.pos.y][node.pos.x + 1]);
}
