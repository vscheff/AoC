#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
	char *line_ptr = NULL;
	size_t n = 0;
	unsigned **grid = malloc(1000 * sizeof(*grid));
	
	for (unsigned i = 0; i < 1000; i++) {
		grid[i] = calloc(1000, sizeof(**grid));
	}

	char *end_ptr = NULL;
	unsigned x1, y1, x2, y2;
	FILE *in_file = fopen(argv[1], "r");
	
	while (getline(&line_ptr, &n, in_file) != -1) {
		if (line_ptr[1] == 'o') {
			x1 = strtoul(line_ptr + 7, &end_ptr, 10);
			y1 = strtoul(end_ptr + 1, &end_ptr, 10);
			x2 = strtoul(end_ptr + 9, &end_ptr, 10);
			y2 = strtoul(end_ptr + 1, NULL, 10);

			for (unsigned i = x1; i <= x2; i++) {
				for (unsigned j = y1; j <= y2; j++) grid[i][j] ^= 1;
			}
		}
		else if (line_ptr[6] == 'f') {
			x1 = strtoul(line_ptr + 9, &end_ptr, 10);
			y1 = strtoul(end_ptr + 1, &end_ptr, 10);
			x2 = strtoul(end_ptr + 9, &end_ptr, 10);
			y2 = strtoul(end_ptr + 1, NULL, 10);

			for (unsigned i = x1; i <= x2; i++) {
				for (unsigned j = y1; j <= y2; j++) grid[i][j] = 0;
			}
		}
		else {
			x1 = strtoul(line_ptr + 8, &end_ptr, 10);
			y1 = strtoul(end_ptr + 1, &end_ptr, 10);
			x2 = strtoul(end_ptr + 9, &end_ptr, 10);
			y2 = strtoul(end_ptr + 1, NULL, 10);

			for (unsigned i = x1; i <= x2; i++) {
				for (unsigned j = y1; j <= y2; j++) grid[i][j] = 1;
			}
		}
	}

	fclose(in_file);
	free(line_ptr);

	unsigned total = 0;

	for (unsigned i = 0; i < 1000; i++) {
		for (unsigned j = 0; j < 1000; j++) total += grid[i][j];

		free(grid[i]);
	}

	free(grid);

	printf("Total = %u\n", total);

	return 0;
}
