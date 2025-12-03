#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	unsigned curr[2];
	unsigned prev[2];
	uint64_t heat_loss;
} PATH_T;

uint64_t find_path();
int compar(const void *, const void *);

unsigned **map = NULL, rows = 0, cols;

int main(int argc, char **argv) {
	FILE *infile = fopen(argv[1], "r");

	char *line = NULL;
	size_t n = 0;
	ssize_t num_char;

	while ( (num_char = getline(&line, &n, infile)) > 1) {
		map = realloc(map, (rows + 1) * sizeof(*map));
		map[rows] = malloc((num_char - 1) * sizeof(**map));

		for (unsigned i = 0; i < num_char - 1; i++) map[rows][i] = line[i] - '0';

		rows++;

		cols = num_char - 1;
	}

	fclose(infile);
	free(line);

	uint64_t heat_loss = find_path();

	for (unsigned i = 0; i < rows; i++) free(map[i]);

	free(map);

	printf("Heat Loss = %lu\n", heat_loss);

	return 0;
}

uint64_t find_path() {
	PATH_T *paths = malloc(sizeof(*paths));
	paths[0] = (PATH_T){ {0, 0}, {0, 0}, 0};
	unsigned num_paths = 1, i, j;
	PATH_T *curr = malloc(sizeof(*curr));
	
	for (;;) {
		memcpy(curr, &paths[num_paths - 1], sizeof(*curr));

		if (curr->curr[0] == rows - 1 && curr->curr[1] == cols - 1) {
			free(paths);

			uint64_t result = curr->heat_loss;
			free(curr);

			return result;
		}

		num_paths--;

		if ( (i = curr->curr[0] - 1) < rows && i != curr->prev[0]) {
			paths = realloc(paths, ++num_paths * sizeof(*paths));
			paths[num_paths - 1] = (PATH_T){ {i, curr->curr[1]}, {curr->curr[0], curr->curr[1]}, curr->heat_loss + map[i][curr->curr[1]]};
		}

		if ( (j = curr->curr[1] + 1) < cols && j != curr->prev[1]) {
			paths = realloc(paths, ++num_paths * sizeof(*paths));
			paths[num_paths - 1] = (PATH_T){ {curr->curr[0], j}, {curr->curr[0], curr->curr[1]}, curr->heat_loss + map[curr->curr[0]][j]};
		}
		
		if ( (i = curr->curr[0] + 1) < rows && i != curr->prev[0]) {
			paths = realloc(paths, ++num_paths * sizeof(*paths));
			paths[num_paths - 1] = (PATH_T){ {i, curr->curr[1]}, {curr->curr[0], curr->curr[1]}, curr->heat_loss + map[i][curr->curr[1]]};
		}

		if ( (j = curr->curr[1] - 1) < cols && j != curr->prev[1]) {
			paths = realloc(paths, ++num_paths * sizeof(*paths));
			paths[num_paths - 1] = (PATH_T){ {curr->curr[0], j}, {curr->curr[0], curr->curr[1]}, curr->heat_loss + map[curr->curr[0]][j]};
		}

		qsort(paths, num_paths, sizeof(*paths), &compar);
	}
	
}

int compar(const void *a, const void *b) {
	PATH_T *path_a = (PATH_T *)a, *path_b = (PATH_T *)b;

	int heat_a = (int)path_a->heat_loss, heat_b = (int)path_b->heat_loss;

	return heat_b - heat_a;
}
