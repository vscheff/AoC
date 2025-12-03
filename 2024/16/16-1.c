#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "minheap.h"

int compare(const void *, const void *);

int main(int argc, char **argv) {
	char *line_ptr = NULL, **map = NULL;
	size_t n = 0, num_char, rows = 0;
	FILE *in_file = fopen(argv[1], "r");

	while ( (num_char = getline(&line_ptr, &n, in_file)) != -1) {
		line_ptr[num_char - 1] = '\0';
		map = realloc(map, ++rows * sizeof(*map));
		map[rows - 1] = line_ptr;
		line_ptr = NULL;
		n = 0;
	}

	fclose(in_file);

	COORD_T start, end;

	for (size_t y = 0; y < rows; y++) {
		for (size_t x = 0; map[y][x]; x++) {
			if (map[y][x] == 'S') { start.x = x; start.y = y; }
			else if (map[y][x] == 'E') { end.x = x; end.y = y; }
		}
	}

	PATH_T *live_nodes = malloc(sizeof(PATH_T)), current_node, *checked = NULL;
	live_nodes[0].pos = start;
     	live_nodes[0].dir = East;
	live_nodes[0].cost = 0;
	size_t num_nodes = 1, num_checked;

	while (num_nodes > 0) {
		current_node = pop_root(&live_nodes, &num_nodes);

		if (map[current_node.pos.y][current_node.pos.x] == '#') continue;
		if (map[current_node.pos.y][current_node.pos.x] == 'E') break;

		if (bsearch(&current_node, checked, num_checked, sizeof(*checked), &compare)) continue;

		checked = realloc(checked, ++num_checked * sizeof(*checked));
		checked[num_checked - 1] = current_node;
		qsort(checked, num_checked, sizeof(*checked), &compare);

		printf("Num nodes = %lu\n", num_nodes);

		switch (current_node.dir)
		{
		case North:
			insert(&live_nodes, &num_nodes, current_node.pos.x, 
			       current_node.pos.y - 1, North, current_node.cost + 1);
			insert(&live_nodes, &num_nodes, current_node.pos.x,
			       current_node.pos.y, East, current_node.cost + 1000);
			insert(&live_nodes, &num_nodes, current_node.pos.x,
			       current_node.pos.y, West, current_node.cost + 1000);
			
			break;
		case East:
			insert(&live_nodes, &num_nodes, current_node.pos.x + 1,
			       current_node.pos.y, East, current_node.cost + 1);
			insert(&live_nodes, &num_nodes, current_node.pos.x,
			       current_node.pos.y, North, current_node.cost + 1000);
			insert(&live_nodes, &num_nodes, current_node.pos.x,
			       current_node.pos.y, South, current_node.cost + 1000);

			break;
		case South:
			insert(&live_nodes, &num_nodes, current_node.pos.x,
			       current_node.pos.y + 1, South, current_node.cost + 1);
			insert(&live_nodes, &num_nodes, current_node.pos.x,
			       current_node.pos.y, East, current_node.cost + 1000);
			insert(&live_nodes, &num_nodes, current_node.pos.x,
			       current_node.pos.y, West, current_node.cost + 1000);

			break;
		case West:
			insert(&live_nodes, &num_nodes, current_node.pos.x - 1,
			       current_node.pos.y, West, current_node.cost + 1);
			insert(&live_nodes, &num_nodes, current_node.pos.x,
			       current_node.pos.y, North, current_node.cost + 1000);
			insert(&live_nodes, &num_nodes, current_node.pos.x,
			       current_node.pos.y, South, current_node.cost + 1000);

			break;
		}
	}

	free(live_nodes);

	for (size_t i = 0; i < rows; i++) free(map[i]);

	printf("Score = %lu\n", current_node.cost);

	return 0;
}

int compare(const void *a, const void *b) {
	PATH_T path_a = *(PATH_T *)a, path_b = *(PATH_T *)b;

	if (path_a.pos.x < path_b.pos.x) return -1;
	if (path_a.pos.x > path_b.pos.x) return 1;
	if (path_a.pos.y < path_b.pos.y) return -1;
	if (path_a.pos.y > path_b.pos.y) return 1;
	if (path_a.dir < path_b.dir) return -1;
	if (path_a.dir > path_b.dir) return 1;

	return 0;
}
