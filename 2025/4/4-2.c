#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
	char **map = NULL;
	char *line_ptr = NULL;
	size_t n = 0, rows = 0;
	ssize_t cols;
	FILE *in_file = fopen(argv[1], "r");

	while ( (cols = getline(&line_ptr, &n, in_file)) > 1) {
		map = realloc(map, sizeof(*map) * ++rows);
		map[rows - 1] = malloc(cols - 1);
		line_ptr[cols - 1] = '\0';
		strcpy(map[rows - 1], line_ptr);
	}

	fclose(in_file);
	free(line_ptr);

	size_t sum = 0, empty;
	short removed;

loop:
	removed = 0;

	for (unsigned i = 0; i < rows; i++) {
		for (unsigned j = 0; map[i][j]; j++) {
			if (map[i][j] != '@') continue;
			
			empty = 0;

			if (! i) empty += 3;
			else {
				if (! j) empty++;
				else if (map[i - 1][j - 1] != '@') empty++;

				if (map[i - 1][j] != '@') empty++;

				if (map[i - 1][j + 1] != '@') empty++;
			}

			if (map[i][j + 1] != '@') empty++;

			if (i == rows - 1) empty += 3;
			else {
				if (! j) empty++;
				else if (map[i + 1][j - 1] != '@') empty++;

				if (map[i + 1][j] != '@') empty++;

				if (map[i + 1][j + 1] != '@') empty++;
			}

			if (j == 0 || map[i][j - 1] != '@') empty++;

			if (empty > 4) {
				sum++;
				removed = 1;
				map[i][j] = 'x';
			}
		}
	}

	if (removed) goto loop;

	for (unsigned i = 0; i < rows; i++) free(map[i]);

	free(map);

	printf("Sum = %lu\n", sum);

	return 0;
}
