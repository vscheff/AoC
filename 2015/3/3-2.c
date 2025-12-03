#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned **map = NULL;
size_t row = 0, col = 0;

void traverse(unsigned *, unsigned *, char, unsigned *, unsigned*); 

int main(int argc, char **argv) {
	char *line_ptr = NULL;
	size_t n = 0;
	FILE *in_file = fopen(argv[1], "r");

	while (getline(&line_ptr, &n, in_file) != -1) {
		row = col = 1;

		map = malloc(sizeof(*map));
		*map = malloc(sizeof(**map));
		**map = 1;

		unsigned x = 0, y = 0, rx = 0, ry = 0;
		unsigned odd = 0;

		for (size_t i = 0; line_ptr[i] > '\n'; i++) {
			if(! odd) traverse(&y, &x, line_ptr[i], &ry, &rx);
			else traverse(&ry, &rx, line_ptr[i], &y, &x);

			odd ^= 1;
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
	free(line_ptr);

	return 0;
}

void traverse(unsigned *y_val, unsigned *x_val, char dir, unsigned *alt_y, unsigned *alt_x) {
	unsigned y = *y_val, x = *x_val;
	
	if (dir == '^') {
		if (! y) {
			map = realloc(map, ++row * sizeof(*map));
			memmove(map + 1, map, (row - 1) * sizeof(*map));
			map[0] = calloc(col, sizeof(**map));
			y = 1;
			*alt_y += 1;
		}

		y--;
	}

	if (dir == 'v') {
		if (y == row - 1) {
			map = realloc(map, ++row * sizeof(*map));
			map[row - 1] = calloc(col, sizeof(**map));
		}

		y++;
	}

	if (dir == '<') {
		if (! x) {
			col++;

			for (unsigned j = 0; j < row; j++) {
				map[j] = realloc(map[j], col * sizeof(**map));
				memmove(map[j] + 1, map[j], (col - 1) * sizeof(**map));
				map[j][0] = 0;
			}
			
			x = 1;
			*alt_x += 1;
		}

		x--;
	}

	if (dir == '>') {
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

	*y_val = y;
	*x_val = x;
}
