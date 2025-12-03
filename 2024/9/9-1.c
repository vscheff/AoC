#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
	char *line_ptr = NULL;
	size_t n = 0;
	FILE *in_file = fopen(argv[1], "r");

	getline(&line_ptr, &n, in_file);

	fclose(in_file);
	
	size_t map_len = 0, size;
	ssize_t label, *map = NULL;

	for (size_t i = 0; line_ptr[i] > '\n'; i++) {
		size = line_ptr[i] - '0';
		label = (i % 2) ? -1 : i / 2;

		for (size_t j = 0; j < size; j++) {
			map = realloc(map, ++map_len * sizeof(*map));
			map[map_len - 1] = label;
		}
	}

	free(line_ptr);

	size_t j = map_len - 1;

	for (size_t i = 0; i < map_len; i++) {
		if (map[i] != -1) continue;

		while (map[j] == -1) j--;
		
		if (i >= j) break;

		map[i] = map[j];
		map[j] = -1;
		j--;
	}

	size_t total = 0;

	for (size_t i = 0; map[i] != -1; i++) total += i * map[i];

	free(map);

	printf("Total = %lu\n", total);

	return 0;
}
