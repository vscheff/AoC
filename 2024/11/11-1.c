#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
	if (argc != 3) { fprintf(stderr, "Usage: %s <filepath> <blinks>\n", *argv); return -1; }

	char *line_ptr = NULL;
	size_t n = 0;
	unsigned long blinks = strtoul(argv[2], NULL, 10);
	FILE *in_file = fopen(argv[1], "r");

	getline(&line_ptr, &n, in_file);

	fclose(in_file);

	unsigned long *stones = NULL;
	size_t num_stones = 0;
	char *tok_ptr = strtok(line_ptr, " ");

	do {
		stones = realloc(stones, ++num_stones * sizeof(*stones));
		stones[num_stones - 1] = strtoul(tok_ptr, NULL, 10);
	} while ( (tok_ptr = strtok(NULL, " ")) );

	free(line_ptr);

	char buff[32];
	char left[32];
	size_t len, j, k;

	for (unsigned long i = 0; i < blinks; i++) {
		for (j = 0; j < num_stones; j++) {
			if (! stones[j]) { stones[j] = 1; continue; }

			sprintf(buff, "%lu\0", stones[j]);
			len = strlen(buff);

			if (len % 2 == 0) {
				stones = realloc(stones, ++num_stones * sizeof(*stones));
				
				if (j < num_stones - 2) memmove(stones + j + 2, stones + j + 1, (num_stones - j - 2) * sizeof(*stones));

				for (k = 0; k < len / 2; k++) left[k] = buff[k];

				left[k] = '\0';
				stones[j] = strtoul(left, NULL, 10);
				stones[j + 1] = strtoul(buff + k, NULL, 10);
				j++;
			}
			else stones[j] *= 2024;
		}
	}

	for (size_t i = 0; i < num_stones; i++) printf("%lu ", stones[i]);

	free(stones);

	printf("\n\nNum Stones = %lu\n", num_stones);

	return 0;
}
