#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
	FILE *infile = fopen(argv[1], "r");

	char *line = NULL, **platform = 0;
	size_t n = 0;
	unsigned num_rows = 0;
	ssize_t num_char;

	printf("Input:\n\n");

	while ( (num_char = getline(&line, &n, infile)) > 1) {
		printf("%s", line);
		line[num_char - 1] = '\0';
		platform = realloc(platform, ++num_rows * sizeof(*platform));
		platform[num_rows - 1] = strdup(line);
	}

	fclose(infile);
	free(line);

	int i, j, k;

	for (i = 0; i < num_rows; i++) {
		for (j = 0; platform[i][j]; j++) {
			if (platform[i][j] != 'O') continue;
			
			for (k = i - 1; k >= 0; k--) {
				if (platform[k][j] != '.') break;
			}

			if (i == ++k) continue;

			platform[k][j] = 'O';
			platform[i][j] = '.';
		}
	}

	unsigned long sum = 0;
	printf("\nOutput:\n\n");

	for (i = 0; i < num_rows; i++) {
		int val = num_rows - i;
		for (j = 0; platform[i][j]; j++) {
			if (platform[i][j] == 'O') sum += val;
		}

		printf("%s\n", platform[i]);
		free(platform[i]);
	}

	free(platform);

	printf("\nSum = %ld\n", sum);

	return 0;
}
