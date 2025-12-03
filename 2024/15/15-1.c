#include <stdio.h>
#include <stdlib.h>

typedef struct COORD_T {
	unsigned x;
	unsigned y;
} COORD_T;

void print_map(char **, size_t);

int main(int argc, char **argv) {
	char *line_ptr = NULL, **map = NULL;
	size_t n = 0, num_char, rows = 0;
	FILE *in_file = fopen(argv[1], "r");

	while ( (num_char = getline(&line_ptr, &n, in_file)) > 1) {
		line_ptr[num_char - 1] = '\0';
		map = realloc(map, ++rows * sizeof(*map));
		map[rows - 1] = line_ptr;
		line_ptr = NULL;
		n = 0;
	}

	COORD_T pos;

	for (size_t i = 0; i < rows; i++) {
		for (size_t j = 0; map[i][j]; j++) {
			if (map[i][j] == '@') {
				pos.x = j;
				pos.y = i;
			}
		}
	}

	unsigned x, y;

	while (getline(&line_ptr, &n, in_file) != -1) {
		for (size_t i = 0; line_ptr[i] > '\n'; i++) {
			//printf("Move %c:\n", line_ptr[i]);
			//print_map(map, rows);
			
			if (line_ptr[i] == '^')	{
				for (y = pos.y; map[y][pos.x] != '#'; y--) {
					if (map[y][pos.x] == '.') break;
				}

				if (map[y][pos.x] != '.') continue;

				for ( ; y < pos.y; y++) map[y][pos.x] = map[y + 1][pos.x];

				map[pos.y][pos.x] = '.';
				pos.y--;
			}
			else if (line_ptr[i] == '>') {
				for (x = pos.x; map[pos.y][x] != '#'; x++) {
					if (map[pos.y][x] == '.') break;
				}

				if (map[pos.y][x] != '.') continue;

				for ( ; x > pos.x; x--) map[pos.y][x] = map[pos.y][x - 1];

				map[pos.y][pos.x] = '.';
				pos.x++;
			}
			else if (line_ptr[i] == 'v') {
				for (y = pos.y; map[y][pos.x] != '#'; y++) {
					if (map[y][pos.x] == '.') break;
				}

				if (map[y][pos.x] != '.') continue;

				for ( ; y > pos.y; y--) map[y][pos.x] = map[y - 1][pos.x];

				map[pos.y][pos.x] = '.';
				pos.y++;
			}
			else if (line_ptr[i] == '<') {
				for (x = pos.x; map[pos.y][x] != '#'; x--) {
					if (map[pos.y][x] == '.') break;
				}

				if (map[pos.y][x] != '.') continue;

				for ( ; x < pos.x; x++) map[pos.y][x] = map[pos.y][x + 1];

				map[pos.y][pos.x] = '.';
				pos.x--;
			}
		}
	}

	fclose(in_file);
	free(line_ptr);

	size_t total = 0;

	for (size_t i = 0; i < rows; i++) {
		for (size_t j = 0; map[i][j]; j++) {
			if (map[i][j] == 'O') total += 100 * i + j;
		}

		free(map[i]);
	}

	free(map);

	printf("Total = %lu\n", total);

	return 0;
}

void print_map(char **map, size_t rows) {
	for (size_t i = 0; i < rows; i++) printf("%s\n", map[i]);

	printf("\n");
}
