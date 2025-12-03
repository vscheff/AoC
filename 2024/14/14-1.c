#include <stdio.h>
#include <stdlib.h>

#define SECONDS 100

typedef struct COORD_T {
	long x;
	long y;
} COORD_T;

typedef struct ROBOT_T {
	COORD_T position;
	COORD_T velocity;
} ROBOT_T;

int main(int argc, char **argv) {
	if (argc != 4) { fprintf(stderr, "Usage: %s <filepath> <width> <height>\n", *argv); return -1; }

	char *line_ptr = NULL, *end_ptr;
	size_t n = 0, num_bots = 0;
	long width = strtol(argv[2], NULL, 10), height = strtol(argv[3], NULL, 10);
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

	for (unsigned long i = 0; i < SECONDS; i++) {
		for (size_t j = 0; j < num_bots; j++) {
			robots[j].position.x += robots[j].velocity.x;
			robots[j].position.y += robots[j].velocity.y;

			if (robots[j].position.x < 0) robots[j].position.x += width;
			else if (robots[j].position.x >= width) robots[j].position.x -= width;

			if (robots[j].position.y < 0) robots[j].position.y += height;
			else if (robots[j].position.y >= height) robots[j].position.y -= height;
		}
	}

	size_t quad1 = 0, quad2 = 0, quad3 = 0, quad4 = 0;
	long mid_width = width / 2 - 1, mid_height = height / 2 - 1;

	for (size_t j = 0; j < num_bots; j++) {
		if (robots[j].position.x <= mid_width && robots[j].position.y <= mid_height) quad1++;
		else if (robots[j].position.x > mid_width + 1 && robots[j].position.y <= mid_height) quad2++;
		else if (robots[j].position.x <= mid_width && robots[j].position.y > mid_height + 1) quad3++;
		else if (robots[j].position.x > mid_width + 1 && robots[j].position.y > mid_height + 1) quad4++;
	}

	printf("Safety Factor: %lu * %lu * %lu * %lu =  %lu\n",
		quad1, quad2, quad3, quad4, quad1 * quad2 * quad3 * quad4);

	return 0;
}
