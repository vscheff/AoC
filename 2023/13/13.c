#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int solve(char **, unsigned);

int main(int argc, char **argv) {
	FILE *infile = fopen(argv[1], "r");

	char *line = NULL, **pattern = NULL;
	size_t n = 0;
	ssize_t chars;
	unsigned pat_size = 0, sum = 0;
	int result;

	while( (chars = getline(&line, &n, infile)) != -1) {
		if (*line == '\n') {
			result = solve(pattern, pat_size);
			
			printf("Result = %u\n", result);

			sum += result;

			for (unsigned i = 0; i < pat_size; i++) free(pattern[i]);
			
			free(pattern);
			pattern = NULL;
			pat_size = 0;
			
			continue;
		}
		
		line[chars - 1] = '\0';
		pattern = realloc(pattern, ++pat_size * sizeof(*pattern));
		pattern[pat_size - 1] = strdup(line);
	}

	printf("Sum = %u\n", sum);

	fclose(infile);
	free(line);

	return 0;
}

int solve(char **pattern, unsigned pat_size) {
	char found_mirror;

	for (int i = 1; i < pat_size; i++) {
		found_mirror = 1;
		
		for (int j = 1; i - j >= 0 && i + j <= pat_size; j++) {
			if (strcmp(pattern[i - j], pattern[i + j - 1]))	{ found_mirror = 0; break; }
		}

		if (found_mirror) return i * 100;
	}

	for (int i = 1; pattern[0][i]; i++) {
		found_mirror = 1;
		
		for (int j = 1; i - j >= 0 && pattern[0][i + j]; j++) {
			for (unsigned k = 0; k < pat_size; k++) {
				if (pattern[k][i - j] != pattern[k][i + j - 1]) { found_mirror = 0; break; }
			}
		
			if (! found_mirror) break;
		}
		
		if (found_mirror) return i;
	}
}
