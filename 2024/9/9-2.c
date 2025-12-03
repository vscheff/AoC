#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct FILE_T {
	size_t size;
	ssize_t label;
} FILE_T;

FILE_T *map;
size_t map_len = 0;

int main(int argc, char **argv) {
	char *line_ptr = NULL;
	size_t n = 0;
	FILE *in_file = fopen(argv[1], "r");

	getline(&line_ptr, &n, in_file);

	fclose(in_file);
	
	size_t size;
	ssize_t label;

	for (size_t i = 0; line_ptr[i] > '\n'; i++) {
		size = line_ptr[i] - '0';

		if (! size) continue;

		label = (i % 2) ? -1 : i / 2;

		map = realloc(map, ++map_len * sizeof(*map));
		map[map_len - 1].size = size;
		map[map_len - 1].label = label;
	}

	free(line_ptr);

	for (size_t j = map_len - 1; ; j--) {
		while (map[j].label == -1) j--;

		if (j > map_len) break;

		for (size_t i = 0; i < j; i++) {
			if (map[i].label != -1 || map[i].size < map[j].size) continue;

			if (map[i].size == map[j].size) {
				map[i].label = map[j].label;
				map[j].label = -1;
			}
			else {
				map = realloc(map, ++map_len * sizeof(*map));
				memmove(map + i + 2, map + i + 1, (map_len - 1 - i) * sizeof(*map));
				j++;
				map[i + 1].size = map[i].size - map[j].size;
				map[i + 1].label = -1;
				map[i].size = map[j].size;
				map[i].label = map[j].label;
				map[j].label = -1;
			}

			break;
		}

	}

	size_t total = 0;
	size_t index = 0;

	for (size_t i = 0; i < map_len; i++) {
		for (size_t j = 0; j < map[i].size; j++) {
			if (map[i].label != -1) {
				total += index * map[i].label;
			}

			index++;
		}
	}

	free(map);

	printf("Total = %lu\n", total);

	return 0;
}
