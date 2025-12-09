#include <stdio.h>
#include <stdlib.h>

unsigned long **memory = NULL;

unsigned long travel_timeline(char**, size_t, size_t, size_t);

int main(int argc, char **argv) {
	char *line_ptr = NULL, **map = 0;;
	size_t n = 0, rows = 0;
	ssize_t len;
	FILE *in_file = fopen(argv[1], "r");

	while ( (len = getline(&line_ptr, &n, in_file)) > 0) {
		map = realloc(map, sizeof(*map) * ++rows);
		map[rows - 1] = line_ptr;
		line_ptr = NULL;
		n = 0;
		memory = realloc(memory, sizeof(*memory) * rows);
		memory[rows - 1] = calloc(len - 1, sizeof(**memory));
	}

	fclose(in_file);
	free(line_ptr);

	size_t i = 0;

	while (map[0][i] != 'S') i++;

	unsigned long sum = travel_timeline(map, rows, 1, i);

	for (size_t i = 0; i < rows; i++) {
		free(map[i]);
		free(memory[i]);
	}

	free(map);
	free(memory);

	printf("Sum = %lu\n", sum);

	return 0;
}

unsigned long travel_timeline(char **map, size_t rows, size_t y, size_t x) {
	unsigned long result = 0;
	
	for (size_t i = y; i < rows - 1; i++) 
		if (map[i][x] == '^') {
			if (! memory[i][x - 1]) memory[i][x - 1] = travel_timeline(map, rows, i, x - 1);
			if (! memory[i][x + 1]) memory[i][x + 1] = travel_timeline(map, rows, i, x + 1);

			return memory[i][x - 1] + memory[i][x + 1];
		}

	return 1;
}
