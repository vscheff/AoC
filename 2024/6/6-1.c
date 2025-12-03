#include<stdio.h>
#include<stdlib.h>

typedef enum DIRECTION_E { Up, Right, Down, Left } DIRECTION_E;

int main(int argc, char **argv) {
	char *line_ptr = NULL, **map = NULL;
	size_t n = 0, row = 0;
	ssize_t num_char;
	FILE *in_file = fopen(argv[1], "r");

	while ( (num_char = getline(&line_ptr, &n, in_file)) != -1) {
		line_ptr[num_char - 1] = '\0';
		map = realloc(map, ++row * sizeof(*map));
		map[row - 1] = line_ptr;
		line_ptr = NULL;
		n = 0;
	}

	fclose(in_file);

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
	map[y][x] = 'X';
	size_t total = 1;

	for ( ; ; ) {
		if (dir == Up) {
			if (! y) break;

			if (map[y - 1][x] == '#') { dir = Right; continue; }
			
			y--;
		}
		else if (dir == Right) {
			if (! map[y][x + 1]) break;

			if (map[y][x + 1] == '#') { dir = Down; continue; }

			x++;
		}
		else if (dir == Down) {
			if (y == row - 1) break;

			if (map[y + 1][x] == '#') { dir = Left; continue; }

			y++;
		}
		else {
			if (! x) break;

			if (map[y][x - 1] == '#') { dir = Up; continue; }

			x--;
		}

		if (map[y][x] == '.') {
			total++;
			map[y][x] = 'X';
		}
	}

	for (size_t i = 0; i < row; i++) free(map[i]);

	free(map);

	printf("Total = %lu\n", total);

	return 0;
}
