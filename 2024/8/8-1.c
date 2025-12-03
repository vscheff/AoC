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

	for (ssize_t i = 0; i < rows; i++) {
		for (ssize_t j = 0; map[i][j]; j++) {
			if (map[i][j] == '.') continue;

			for (ssize_t k = j + 1; map[i][k]; k++) {
				if (map[i][k] != map[i][j]) continue;

				if (j - (k - j) >= 0) nodes[i][j - (k - j)] = 1;
				if (k + (k - j) < cols) nodes[i][k + (k - j)] = 1;
			}

			for (ssize_t k = i + 1; k < rows; k++) {
				for (ssize_t l = 0; map[k][l]; l++) {
					if (map[k][l] != map[i][j]) continue;

					ssize_t dy = k - i;
					ssize_t dx = l - j;
					if (i - dy >= 0 && j - dx >= 0 && i - dy < rows && j - dx < cols) nodes[i - dy][j - dx] = 1;
					if (k + dy >= 0 && l + dx >= 0 && k + dy < rows && l + dx < cols) nodes[k + dy][l + dx] = 1;
				}
			}
		}
	}

	size_t total = 0;

	for (ssize_t i = 0; i < rows; i++) {
		for (ssize_t j = 0; j < cols; j++) {
			total += nodes[i][j];
		}

		free(map[i]);
		free(nodes[i]);
	}

	free(map);
	free(nodes);

	printf("Total = %lu\n", total);

	return 0;
}
