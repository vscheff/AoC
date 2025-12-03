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
	size_t max = strlen(puz[0]) - 2;

	for (size_t i = 1; i < lines - 1; i++) {
		for (size_t j = 1; puz[i][j + 1]; j++) {
			if (puz[i][j] != 'A') continue;
			
			if ( (puz[i - 1][j - 1] == 'M' && puz[i + 1][j + 1] == 'S') || (puz[i - 1][j - 1] == 'S' && puz[i + 1][j + 1] == 'M') )
				if ( (puz[i - 1][j + 1] == 'M' && puz[i + 1][j - 1] == 'S') ||
				     (puz[i - 1][j + 1] == 'S' && puz[i + 1][j - 1] == 'M') ) occurences++;
		}
	}

	for (size_t i = 0; i < lines; i++) free(puz[i]);

	free(puz);

	printf("Occurences = %u\n", occurences);

	return 0;
}
