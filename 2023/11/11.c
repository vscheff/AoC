#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	int x;
	int y;
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
	
	for (unsigned i = 0; i < map_height; i++) {
		found_galaxy = 0;
		for (unsigned j = 0; map[i][j]; j++) if (map[i][j] == '#') { found_galaxy = 1; break; }

		if (! found_galaxy) {
			map = realloc(map, ++map_height * sizeof(*map));
			memmove(&map[i + 2], &map[i + 1], (map_height - i - 2) * sizeof(*map));
			map[i + 1] = malloc(map_width * sizeof(char));
			strcpy(map[i + 1], map[i]);
			i++;
		}		
	}

	for (unsigned j = 0; j < map_width; j++) {
		found_galaxy = 0;
		for (unsigned i = 0; i < map_height; i++) if (map[i][j] == '#') { found_galaxy = 1; break; }

		if (! found_galaxy) {
			map_width++;

			for (unsigned k = 0; k < map_height; k++) {
				map[k] = realloc(map[k], map_width * sizeof(char));
				memmove(&map[k][j + 2], &map[k][j + 1], (map_width - j - 2) * sizeof(char));
				map[k][j + 1] = '.';
			}

			j++;
		}
	}

	COORD_T *galaxies = NULL;
	unsigned num_galaxy = 0;
	
	for (int y = 0; y < map_height; y++) {
		for (int x = 0; map[y][x]; x++) {
			if (map[y][x] == '#') {
				galaxies = realloc(galaxies, (num_galaxy + 1) * sizeof(*galaxies));
				galaxies[num_galaxy].x = x;
				galaxies[num_galaxy].y = y;
				num_galaxy++;
			}
		}
		
		free(map[y]);
	}

	free(map);

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
