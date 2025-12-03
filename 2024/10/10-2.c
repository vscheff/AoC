#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned **map = NULL, rows = 0, cols, **found = NULL;

void traverse(unsigned, unsigned);

int main(int argc, char **argv) {
	char *line_ptr = NULL;
	size_t n = 0;
	ssize_t num_char;
	FILE *in_file = fopen(argv[1], "r");

	while ( (num_char = getline(&line_ptr, &n, in_file)) > 1) {
		map = realloc(map, (rows + 1) * sizeof(*map));
		map[rows] = malloc((num_char - 1) * sizeof(**map));
		found = realloc(found, (rows + 1) * sizeof(*found));
		found[rows] = malloc((num_char - 1) * sizeof(**map));
	
		for (ssize_t i = 0; line_ptr[i] > '\n'; i++) map[rows][i] = line_ptr[i] - '0';

		rows++;
		cols = num_char - 1;
	}

	fclose(in_file);
	free(line_ptr);

	unsigned total = 0;

	for (unsigned i = 0; i < rows; i++) {
		for (unsigned j = 0; j < cols; j++) {
			if (map[i][j]) continue;

			for (unsigned k = 0; k < rows; k++) memset(found[k], 0, cols * sizeof(**found));

			traverse(i, j);

			for (unsigned k = 0; k < rows; k++) {
				for (unsigned l = 0; l < cols; l++) total += found[k][l];
			}
		}
	}

	for (unsigned i = 0; i < rows; i++) {
		free(map[i]);
		free(found[i]);
	}

	free(map);
	free(found);

	printf("Total = %u\n", total);

	return 0;
}

void traverse(unsigned i, unsigned j) {
	if (map[i][j] == 9) { found[i][j] += 1; return; }

	unsigned next_num = map[i][j] + 1;

	if (i && map[i - 1][j] == next_num) traverse(i - 1, j);
	if (j < cols - 1 && map[i][j + 1] == next_num) traverse(i, j + 1);
	if (i < rows - 1 && map[i + 1][j] == next_num) traverse(i + 1, j);
	if (j && map[i][j - 1] == next_num) traverse(i, j - 1);
}
