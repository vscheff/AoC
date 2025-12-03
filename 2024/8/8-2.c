#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	char *line_ptr = NULL, **map = NULL;
	size_t n = 0;
	ssize_t num_char, rows = 0, cols;
	unsigned **nodes = NULL;
	FILE *in_file = fopen(argv[1], "r");

	while ( (num_char = getline(&line_ptr, &n, in_file)) != -1) {
		line_ptr[num_char - 1] = '\0';
		map = realloc(map, ++rows * sizeof(*map));
		map[rows - 1] = line_ptr;
		nodes = realloc(nodes, rows * sizeof(*nodes));
		nodes[rows - 1] = calloc(num_char - 1, sizeof(**nodes));
		cols = num_char - 1;
		line_ptr = NULL;
		n = 0;
	}

	fclose(in_file);

	ssize_t j, k, l, dx, dy, mod_x, mod_y;
	unsigned mod, done, mark;

	for (ssize_t i = 0; i < rows; i++) {
		for (j = 0; map[i][j]; j++) {
			if (map[i][j] == '.') continue;

			mark = 0;

			for (l = j + 1; map[i][l]; l++) {
				if (map[i][l] != map[i][j]) continue;

				nodes[i][l] = 1;
				mark = 1;
				dx = l - j;
				mod = 1;
				done = 0;

				while (! done) {
					mod_x = mod * dx;
					done = 1;

					if (j - (mod_x) >= 0) { nodes[i][j - (mod_x)] = 1; done = 0; }
					if (l + (mod_x) < cols) { nodes[i][l + (mod_x)] = 1; done = 0; }

					mod++;
				}
			}

			for (k = i + 1; k < rows; k++) {
				for (l = 0; map[k][l]; l++) {
					if (map[k][l] != map[i][j]) continue;

					nodes[k][l] = 1;
					mark = 1;
					dy = k - i;
					dx = l - j;
					mod = 1;
					done = 0;

					while (! done) {
						mod_x = mod * dx;
						mod_y = mod * dy;
						done = 1;

						if (i - mod_y >= 0 && j - mod_x >= 0 && i - mod_y < rows && j - mod_x < cols) { 
							nodes[i - mod_y][j - mod_x] = 1;
							done = 0;
						}
						if (k + mod_y >= 0 && l + mod_x >= 0 && k + mod_y < rows && l + mod_x < cols) {
							nodes[k + mod_y][l + mod_x] = 1;
							done = 0;
						}

						mod++;
					}
				}
			}

			if (mark) nodes[i][j] = 1;
		}
	}

	size_t total = 0;

	for (ssize_t i = 0; i < rows; i++) {
		for (ssize_t j = 0; j < cols; j++) {
			total += nodes[i][j];
			if (nodes[i][j] && map[i][j] == '.') map[i][j] = '#';
		}

		printf("%s\n", map[i]);

		free(map[i]);
		free(nodes[i]);
	}

	free(map);
	free(nodes);

	printf("Total = %lu\n", total);

	return 0;
}
