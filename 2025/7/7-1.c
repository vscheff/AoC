#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	char *line_ptr = NULL, **map = 0;;
	size_t n = 0, rows = 0;
	FILE *in_file = fopen(argv[1], "r");

	while (getline(&line_ptr, &n, in_file) > 0) {
		map = realloc(map, sizeof(*map) * ++rows);
		map[rows - 1] = line_ptr;
		line_ptr = NULL;
		n = 0;
	}

	fclose(in_file);

	unsigned long sum = 0;

	for (size_t i = 1; i < rows; i++) {
		for (size_t j = 0; map[i][j] > '\n'; j++) {
			switch (map[i][j]) {
				case '.':
					if (map[i - 1][j] == '|' || map[i - 1][j] == 'S') map[i][j] = '|';
					
					break;
				case '^':
					if (map[i - 1][j] != '|') break;

					map[i + 1][j - 1] = map[i + 1][j + 1] = '|';
					j++;
					sum++;
					
					break;
				default: continue;
			}
		}
	}

	for (size_t i = 0; i < rows; i++) free(map[i]);

	free(map);

	printf("Sum = %lu\n", sum);

	return 0;
}
