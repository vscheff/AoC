#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EXPANSION 1000000

typedef struct {
	int64_t x;
	int64_t y;
} COORD_T;

int main(int argc, char **argv) {
	FILE *infile = fopen(argv[1], "r");

	char *line = NULL, **map = NULL;
	unsigned map_height = 0, map_width = 0;
	size_t n = 0;
	ssize_t chars;

	while ( (chars = getline(&line, &n, infile)) > 1) {
		if (! map_width) map_width = chars;
		
		line[chars - 1] = '\0';
		map = realloc(map, (map_height + 1) * sizeof(*map));
		map[map_height] = malloc(chars * sizeof(char));
		strcpy(map[map_height], line);
		
		map_height++;
	}

	fclose(infile);
	free(line);

	char found_galaxy;
	unsigned *blank_rows = NULL, num_blank_rows = 0;

	for (unsigned i = 0; i < map_height; i++) {
		found_galaxy = 0;

		for (unsigned j = 0; map[i][j]; j++) if (map[i][j] == '#') { found_galaxy = 1; break; }

		if (! found_galaxy) {
			blank_rows = realloc(blank_rows, ++num_blank_rows * sizeof(*blank_rows));
			blank_rows[num_blank_rows - 1] = i;
		}		
	}

	unsigned *blank_cols = NULL, num_blank_cols = 0;

	for (unsigned j = 0; j < map_width; j++) {
		found_galaxy = 0;

		for (unsigned i = 0; i < map_height; i++) if (map[i][j] == '#') { found_galaxy = 1; break; }

		if (! found_galaxy) {
			blank_cols = realloc(blank_cols, ++num_blank_cols * sizeof(*blank_cols));
			blank_cols[num_blank_cols - 1] = j;
		}
	}

	COORD_T *galaxies = NULL;
	unsigned num_galaxy = 0, b_row = 0, b_col = 0;
	uint64_t x_offset = 0, y_offset = 0;
	
	for (int y = 0; y < map_height; y++) {
		if (b_row < num_blank_rows && blank_rows[b_row] == y) {
			y_offset += EXPANSION - 1;
			b_row++;
			free(map[y]);
			continue;
		}

		x_offset = 0;
		b_col = 0;

		for (int x = 0; map[y][x]; x++) {
			if (b_col < num_blank_cols && blank_cols[b_col] == x) {
				x_offset += EXPANSION - 1;
				b_col++;
				continue;
			}
			
			if (map[y][x] == '#') {
				galaxies = realloc(galaxies, (num_galaxy + 1) * sizeof(*galaxies));
				galaxies[num_galaxy].x = x + x_offset;
				galaxies[num_galaxy].y = y + y_offset;
				num_galaxy++;
			}
		}
		
		free(map[y]);
	}

	free(map);
	free(blank_rows);
	free(blank_cols);

	uint64_t sum = 0;

	for (unsigned i = 0; i < num_galaxy; i++) {
		for (unsigned j = i + 1; j < num_galaxy; j++) {
			sum += abs(galaxies[i].x - galaxies[j].x) + abs(galaxies[i].y - galaxies[j].y);
		}
	}

	free(galaxies);

	printf("Sum = %lu\n", sum);

	return 0;
}
