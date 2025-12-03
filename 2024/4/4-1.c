#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
	char *line_ptr = NULL;
	char **puz = NULL;
	size_t n = 0, lines = 0;
	FILE *in_file = fopen(argv[1], "r");

	while (getline(&line_ptr, &n, in_file) != -1) {
		puz = realloc(puz, ++lines * sizeof(*puz));
		puz[lines - 1] = line_ptr;
		line_ptr = NULL;
		n = 0;
	}

	fclose(in_file);

	unsigned occurences = 0;
	size_t max = strlen(puz[0]) - 3;

	for (size_t i = 0; i < lines; i++) {
		for (size_t j = 0; puz[i][j]; j++) {
			if (puz[i][j] != 'X') continue;

			if (i > 2) {
				if (puz[i - 1][j] == 'M' && puz[i - 2][j] == 'A' && puz[i - 3][j] == 'S') occurences++;

				if (j > 2 && puz[i - 1][j - 1] == 'M' && puz[i - 2][j - 2] == 'A' && puz[i - 3][j - 3] == 'S') occurences++;

				if (j < max && puz[i - 1][j + 1] == 'M' && puz[i - 2][j + 2] == 'A' && puz[i - 3][j + 3] == 'S') occurences++;
			}

			if (j > 2 && puz[i][j - 1] == 'M' && puz[i][j - 2] == 'A' && puz[i][j - 3] == 'S') occurences++;

			if (j < max && puz[i][j + 1] == 'M' && puz[i][j + 2] == 'A' && puz[i][ j + 3] == 'S') occurences++;

			if (i < lines - 3) {
				if (puz[i + 1][j] == 'M' && puz[i + 2][j] == 'A' && puz[i + 3][j] == 'S') occurences++;

				if (j > 2 && puz[i + 1][j - 1] == 'M' && puz[i + 2][j - 2] == 'A' && puz[i + 3][j - 3] == 'S') occurences++;

				if (j < max && puz[i + 1][j + 1] == 'M' && puz[i + 2][j + 2] == 'A' && puz[i + 3][j + 3] == 'S') occurences++;
			}
		}
	}

	for (size_t i = 0; i < lines; i++) free(puz[i]);

	free(puz);

	printf("Occurences = %u\n", occurences);

	return 0;
}
