#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum DIRECTION_E { Up, Right, Down, Left } DIRECTION_E;

int traverse(char **, size_t, size_t, size_t);

size_t MAX_STEPS;

int main(int argc, char **argv) {
	char *line_ptr = NULL, **map = NULL;
	size_t n = 0, row = 0;
	ssize_t num_char;
	FILE *in_file = fopen(argv[1], "r");
	unsigned **check = NULL;

	while ( (num_char = getline(&line_ptr, &n, in_file)) != -1) {
		line_ptr[num_char - 1] = '\0';
		map = realloc(map, ++row * sizeof(*map));
		map[row - 1] = line_ptr;
		check = realloc(check, row * sizeof(*check));
		check[row - 1] = calloc(num_char - 1, sizeof(**check));

		line_ptr = NULL;
		n = 0;
	}

	fclose(in_file);

	MAX_STEPS = row * strlen(map[0]);
	size_t x = 0, y = 0;

	for (size_t i = 0; i < row; i++) {
		for (size_t j = 0; map[i][j]; j++) {
			if (map[i][j] != '^') continue;

			x = j;
			y = i;
			break;
		}

		if (x || y) break;
	}

	DIRECTION_E dir = Up;
	map[y][x] = '.';
	size_t total = 0, start_x = x, start_y = y;

	for ( ; ; ) {
		if (dir == Up) {
			if (! y) break;

			if (map[y - 1][x] == '#') { dir = Right; continue; }
		
			if (! check[y - 1][x]) {	
				map[y - 1][x] = 'O';
				total += traverse(map, start_x, start_y, row);
				map[y - 1][x] = '.';
				check[y - 1][x] = 1;
			}

			y--;
		}
		else if (dir == Right) {
			if (! map[y][x + 1]) break;

			if (map[y][x + 1] == '#') { dir = Down; continue; }

			if (! check[y][x + 1]) {	
				map[y][x + 1] = 'O';
				total += traverse(map, start_x, start_y, row);
				map[y][x + 1] = '.';
				check[y][x + 1] = 1;
			}

			x++;
		}
		else if (dir == Down) {
			if (y == row - 1) break;

			if (map[y + 1][x] == '#') { dir = Left; continue; }

			if (! check[y + 1][x]) {	
				map[y + 1][x] = 'O';
				total += traverse(map, start_x, start_y, row);
				map[y + 1][x] = '.';
				check[y + 1][x] = 1;
			}

			y++;
		}
		else {
			if (! x) break;

			if (map[y][x - 1] == '#') { dir = Up; continue; }

			if (! check[y][x - 1]) {	
				map[y][x - 1] = 'O';
				total += traverse(map, start_x, start_y, row);
				map[y][x - 1] = '.';
				check[y][x - 1] = 1;
			}

			x--;
		}
	}

	for (size_t i = 0; i < row; i++) {
		free(map[i]);
		free(check[i]);
	}

	free(map);
	free(check);

	printf("Total = %lu\n", total);

	return 0;
}

void print_map(char **map, size_t row) {
	for (size_t i = 0; i < row; i++) printf("%s\n", map[i]);

	printf("\n");
}

int traverse(char **map, size_t x, size_t y, size_t row) {
	size_t steps = 0, start_x = x, start_y = y;
	DIRECTION_E dir = Up;

	for ( ; ; ) {
		if (steps > MAX_STEPS) {
			/*
			map[start_y][start_x] = '^';			
			print_map(map, row);
			map[start_y][start_x] = '.';			
			*/

			return 1;
		}

		switch (dir) {
		case Up:
			if (! y) return 0;

			if (map[y - 1][x] != '.') { dir = Right; continue; }
			
			y--;
			break;

		case Right:
			if (! map[y][x + 1]) return 0;

			if (map[y][x + 1] != '.') { dir = Down; continue; }

			x++;
			break;

		case Down:
			if (y == row - 1) return 0;

			if (map[y + 1][x] != '.') { dir = Left; continue; }

			y++;
			break;

		case Left:
			if (! x) return 0;

			if (map[y][x - 1] != '.') { dir = Up; continue; }

			x--;
		}

		steps++;
	}
}
