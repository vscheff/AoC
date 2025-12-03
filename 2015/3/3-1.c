#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
	char *line_ptr = NULL;
	size_t n = 0, row = 0, col = 0;
	FILE *in_file = fopen(argv[1], "r");
	unsigned **map = NULL;

	while (getline(&line_ptr, &n, in_file) != -1) {
		row = col = 1;

		map = malloc(sizeof(*map));
		*map = malloc(sizeof(**map));
		**map = 1;

		unsigned x = 0, y = 0;

		for (size_t i = 0; line_ptr[i] > '\n'; i++) {
			if (line_ptr[i] == '^') {
				if (! y) {
					map = realloc(map, ++row * sizeof(*map));
					memmove(map + 1, map, (row - 1) * sizeof(*map));
					map[0] = calloc(col, sizeof(**map));
					y = 1;
				}

				y--;
			}

			if (line_ptr[i] == 'v') {
				if (y == row - 1) {
					map = realloc(map, ++row * sizeof(*map));
					map[row - 1] = calloc(col, sizeof(**map));
				}

				y++;
			}

			if (line_ptr[i] == '<') {
				if (! x) {
					col++;

					for (unsigned j = 0; j < row; j++) {
						map[j] = realloc(map[j], col * sizeof(**map));
						memmove(map[j] + 1, map[j], (col - 1) * sizeof(**map));
						map[j][0] = 0;
					}
					
					x = 1;
				}

				x--;
			}

			if (line_ptr[i] == '>') {
				if (x == col - 1) {
					col++;

					for (unsigned j = 0; j < row; j++) {
						map[j] = realloc(map[j], col * sizeof(**map));
						map[j][col - 1] = 0;
					}

				}

				x++;
			}

			map[y][x] = 1;
		}

		unsigned total = 0;

		for (unsigned i = 0; i < row; i++) {
			for (unsigned j = 0; j < col; j++) total += map[i][j];

			free(map[i]);
		}

		free(map);

		printf("Total = %u\n", total);
	}

	fclose(in_file);

	return 0;
}
