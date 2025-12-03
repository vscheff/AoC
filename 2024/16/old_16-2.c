#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "minheap-2.h"

int compare(const void *, const void *);

int main(int argc, char **argv) {
	char *line_ptr = NULL, **map = NULL;
	size_t n = 0, num_char, rows = 0;
	unsigned **seats = NULL;
	FILE *in_file = fopen(argv[1], "r");

	while ( (num_char = getline(&line_ptr, &n, in_file)) != -1) {
		line_ptr[num_char - 1] = '\0';
		map = realloc(map, ++rows * sizeof(*map));
		map[rows - 1] = line_ptr;
		seats = realloc(seats, rows * sizeof(*seats));
		seats[rows - 1] = calloc(num_char - 1, sizeof(**seats));
		line_ptr = NULL;
		n = 0;
	}

	fclose(in_file);
	free(line_ptr);

	size_t cols = strlen(*map);
	COORD_T start, end;

	for (size_t y = 0; y < rows; y++) {
		for (size_t x = 0; map[y][x]; x++) {
			if (map[y][x] == 'S') { start.x = x; start.y = y; }
			else if (map[y][x] == 'E') { end.x = x; end.y = y; }
		}
	}

	PATH_T *live_nodes = malloc(sizeof(PATH_T)), current_node;
      	CHECK_T *checked = NULL;
	live_nodes[0].pos = malloc(sizeof(*live_nodes[0].pos));
	live_nodes[0].pos[0] = start;
	live_nodes[0].len = 1;
     	live_nodes[0].dir = East;
	live_nodes[0].cost = 0;
	size_t num_nodes = 1, num_checked = 0, min_cost = 0;
	unsigned solution_found = 0;

	while (num_nodes > 0) {
		current_node = pop_root(&live_nodes, &num_nodes);
		
		COORD_T pos = current_node.pos[current_node.len - 1];

		if (solution_found && current_node.cost > min_cost) { 
			free(current_node.pos);
			continue;
		}

		if (map[pos.y][pos.x] == 'E') {
			solution_found = 1;
			min_cost = current_node.cost;

			for (size_t i = 0; i < current_node.len; i++) {
				seats[current_node.pos[i].y][current_node.pos[i].x] = 1;
			}

			free(current_node.pos);
			
			continue;
		}

		/*
		CHECK_T check = { pos, current_node.dir };

		if (bsearch(&check, checked, num_checked, sizeof(*checked), &compare)) continue;

		checked = realloc(checked, ++num_checked * sizeof(*checked));
		checked[num_checked - 1].pos = pos;
		checked[num_checked - 1].dir = current_node.dir;
		qsort(checked, num_checked, sizeof(*checked), &compare);
		*/

		printf("Num nodes = %lu\n", num_nodes);

		switch (current_node.dir)
		{
		case North:
			if (map[pos.y - 1][pos.x] != '#') {
				insert(&live_nodes, &num_nodes, pos.x, 
					pos.y - 1, North, current_node.cost + 1, current_node);
			}
			if (map[pos.y][pos.x + 1] != '#') {
				insert(&live_nodes, &num_nodes, pos.x,
			       		pos.y, East, current_node.cost + 1000, current_node);
			}
			if (map[pos.y][pos.x - 1] != '#') {
				insert(&live_nodes, &num_nodes, pos.x,
			       		pos.y, West, current_node.cost + 1000, current_node);
			}
			
			break;
		case East:
			if (map[pos.y][pos.x + 1] != '#') {
				insert(&live_nodes, &num_nodes, pos.x + 1,
			       		pos.y, East, current_node.cost + 1, current_node);
			}
			if (map[pos.y - 1][pos.x] != '#') {
				insert(&live_nodes, &num_nodes, pos.x,
			       		pos.y, North, current_node.cost + 1000, current_node);
			}
			if (map[pos.y + 1][pos.x] != '#') {
				insert(&live_nodes, &num_nodes, pos.x,
			       		pos.y, South, current_node.cost + 1000, current_node);
			}

			break;
		case South:
			if (map[pos.y + 1][pos.x] != '#') {
				insert(&live_nodes, &num_nodes, pos.x,
			       		pos.y + 1, South, current_node.cost + 1, current_node);
			}
			if (map[pos.y][pos.x + 1] != '#') {
				insert(&live_nodes, &num_nodes, pos.x,
			       		pos.y, East, current_node.cost + 1000, current_node);
			}
			if (map[pos.y][pos.x - 1] != '#') {
				insert(&live_nodes, &num_nodes, pos.x,
			       		pos.y, West, current_node.cost + 1000, current_node);
			}

			break;
		case West:
			if (map[pos.y][pos.x - 1] != '#') {
				insert(&live_nodes, &num_nodes, pos.x - 1,
			       		pos.y, West, current_node.cost + 1, current_node);
			}
			if (map[pos.y - 1][pos.x] != '#') {
				insert(&live_nodes, &num_nodes, pos.x,
			       		pos.y, North, current_node.cost + 1000, current_node);
			}
			if (map[pos.y + 1][pos.x] != '#') {
				insert(&live_nodes, &num_nodes, pos.x,
			       		pos.y, South, current_node.cost + 1000, current_node);
			}

			break;
		}

		free(current_node.pos);
	}

	for (size_t i = 0; i < num_nodes; i++) free(live_nodes[i].pos);

	free(live_nodes);

	size_t num_seats = 0;

	printf("\n");

	for (size_t i = 0; i < rows; i++) {
		for (size_t j = 0; j < cols; j++) {
			num_seats += seats[i][j];
			
			if (seats[i][j]) printf("O");
			else (printf("%c", map[i][j]));
		}

		free(seats[i]);
		free(map[i]);

		printf("\n");
	}

	free(map);
	free(seats);

	printf("\nSeats = %lu\n", num_seats);

	return 0;
}

int compare(const void *a, const void *b) {
	CHECK_T check_a = *(CHECK_T *)a, check_b = *(CHECK_T *)b;

	if (check_a.pos.x < check_b.pos.x) return -1;
	if (check_a.pos.x > check_b.pos.x) return 1;
	if (check_a.pos.y < check_b.pos.y) return -1;
	if (check_a.pos.y > check_b.pos.y) return 1;
	if (check_a.dir < check_b.dir) return -1;
	if (check_a.dir > check_b.dir) return 1;

	return 0;
}
