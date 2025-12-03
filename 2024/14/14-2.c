#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct COORD_T {
	long x;
	long y;
} COORD_T;

typedef struct ROBOT_T {
	COORD_T position;
	COORD_T velocity;
} ROBOT_T;

void print_map(ROBOT_T *, long, long, size_t, unsigned);

int main(int argc, char **argv) {
	if (argc != 5) {
		fprintf(stderr, "Usage: %s <filepath> <width> <height> <seconds>\n", *argv);
		return -1;
	}

	char *line_ptr = NULL, *end_ptr;
	size_t n = 0, num_bots = 0;
	long width = strtol(argv[2], NULL, 10), height = strtol(argv[3], NULL, 10);
	long seconds = strtol(argv[4], NULL, 10);
	ROBOT_T *robots = NULL;
	FILE *in_file = fopen(argv[1], "r");

	while (getline(&line_ptr, &n, in_file) != -1) {
		robots = realloc(robots, (num_bots + 1) * sizeof(*robots));
		robots[num_bots].position.x = strtol(line_ptr + 2, &end_ptr, 10);
		robots[num_bots].position.y = strtol(end_ptr + 1, &end_ptr, 10);
		robots[num_bots].velocity.x = strtol(end_ptr + 3, &end_ptr, 10);
		robots[num_bots].velocity.y = strtol(end_ptr + 1, &end_ptr, 10);
		num_bots++;
	}

	fclose(in_file);
	free(line_ptr);

	for (unsigned long i = 0; i < seconds; i++) {
		for (size_t j = 0; j < num_bots; j++) {
			robots[j].position.x += robots[j].velocity.x;
			robots[j].position.y += robots[j].velocity.y;

			if (robots[j].position.x < 0) robots[j].position.x += width;
			else if (robots[j].position.x >= width) robots[j].position.x -= width;

			if (robots[j].position.y < 0) robots[j].position.y += height;
			else if (robots[j].position.y >= height) robots[j].position.y -= height;
		}

		print_map(robots, width, height, num_bots, i);
	}

	free(robots);

	return 0;
}

void print_map(ROBOT_T *robots, long width, long height, size_t num_bots, unsigned seconds) {
	char **map = malloc(height * sizeof(*map));
	for (long i = 0; i < height; i++) {
		map[i] = malloc(width);
		memset(map[i], ' ', width);
	}

	for (size_t i = 0; i < num_bots; i++) map[robots[i].position.y][robots[i].position.x] = 'R';

	unsigned in_a_row;

	for (long i = 0; i < height; i++) {
		in_a_row = 0;

		for (long j = 0; j < width; j++) {
			if (map[i][j] != ' ') in_a_row++;
			else in_a_row = 0;

			if (in_a_row >= 8) break;
		}

		if (in_a_row >= 8) break;
	}

	if (in_a_row >= 8) {
		printf("%u Seconds\n", seconds);

		for (long i = 0; i < height; i++) {
			for (long j = 0; j < width; j++) printf("%c", map[i][j]);

			printf("\n");
		}

		printf("\n");
	}

	for (long i = 0; i < height; i++) free(map[i]);

	free(map);
}
