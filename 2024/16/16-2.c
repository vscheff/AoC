#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "minheap-2.h"

void mark_seats(unsigned **, NODE_T **, NODE_T);

int main(int argc, char **argv) {
	char *line_ptr = NULL, **map = NULL;
	size_t n = 0, num_char, rows = 0, cols, num_nodes = 0;
	unsigned **seats = NULL;
	NODE_T **nodes = NULL, **node_map = NULL;
	FILE *in_file = fopen(argv[1], "r");
	COORD_T start;

	while ( (num_char = getline(&line_ptr, &n, in_file)) != -1) {
		cols = num_char - 1;
		line_ptr[cols] = '\0';
		map = realloc(map, ++rows * sizeof(*map));
		map[rows - 1] = line_ptr;
		seats = realloc(seats, rows * sizeof(*seats));
		seats[rows - 1] = calloc(cols, sizeof(**seats));
		
		for (size_t i = 0; i < cols; i++) {
			if (line_ptr[i] == 'S') {
				start.y = rows - 1;
				start.x = i;
			}
		}
		
		line_ptr = NULL;
		n = 0;
	}

	fclose(in_file);
	free(line_ptr);

	NODE_T *nodes = NULL, new_node, current_node;
	size_t best_score = (size_t)-1, num_best = 0. num_visited = 0;
	COORD_T **best_paths = NULL, search_pos;
	VISITED_T *visited = NULL;
	new_node.dist = 0;
	new_node.path = malloc(sizeof(*new_node.path));
	*new_node.path = start;
	new_node.len = 1;
	new_node.dir = East;
	insert(nodes, 0, new_node);

	while (num_nodes > 0) {
		current_node = pop_root(&nodes, &num_nodes);
		
		printf("Num nodes = %lu\n", num_nodes);

		if (current_node.dist > best_score) continue;

		if (map[current_node.pos.y][current_node.pos.x] == 'E') {
			best = current_node.dist;
			best_paths = realloc(best_paths, ++num_best * sizeof(*best_paths));
			best_paths[num_best - 1] = current.node.path;
		}

		visited = realloc(visited, (num_visited + 1) * sizeof(*visited));
		visited[num_visited].dist = current_node.dist;
		visited[num_visited].pos = current_node.path[current_node.len - 1];
		visited[num_visited].dir = current_node.dir;
		num_visited++;

		switch (current_node.dir)
		{
		case North:
			if (map[current_node.pos.y - 1][current_node.pos.x] != '#') {
				search_pos.y = current_node.pos.y - 1;
				search_pos.x = current_node.pos.x;
				if (! lfind(&search_pos, ))
			}

			break;
		case East:
			break;
		case South:
			break;
		case West:
			break;
		}
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
