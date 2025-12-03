#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **plot = NULL;
unsigned **checked = NULL, **region = NULL;
size_t rows = 0, cols, area;

void get_area(size_t, size_t, char);
size_t get_sides();

int main(int argc, char **argv) {
	char *line_ptr = NULL;
	size_t n = 0, num_char;
	FILE *in_file = fopen(argv[1], "r");

	while ( (num_char = getline(&line_ptr, &n, in_file)) > 1) {
		line_ptr[num_char - 1] = '\0';
		plot = realloc(plot, ++rows * sizeof(*plot));
		plot[rows - 1] = line_ptr;
		checked = realloc(checked, rows * sizeof(*checked));
		checked[rows - 1] = calloc((num_char - 1), sizeof(**checked));
		region = realloc(region, rows * sizeof(*region));
		region[rows - 1] = malloc((num_char - 1) * sizeof(**region));
		line_ptr = NULL;
		n = 0;
	}

	fclose(in_file);

	cols = strlen(plot[0]);
	size_t total = 0, sides;

	for (size_t i = 0; i < rows; i++) {
		for (size_t j = 0; j < cols; j++) {
			if (checked[i][j]) continue;
			
			area = 0;
			
			for (size_t k = 0; k < rows; k++) memset(region[k], 0, cols * sizeof(**region));
			
			get_area(i, j, plot[i][j]);
			sides = get_sides(i, j);
			total += area * sides;
		}
	}

	for (size_t i = 0; i < rows; i++) {
		free(plot[i]);
		free(checked[i]);
	}

	free(plot);
	free(checked);

	printf("Total = %lu\n", total);

	return 0;
}

void get_area(size_t i, size_t j, char letter) {
	if (checked[i][j]) return;
	
	checked[i][j] = 1;
	region[i][j] = 1;
	area++;

	if (i) {
		if (plot[i - 1][j] == letter) get_area(i - 1, j, letter);
	}

	if (j) {
		if (plot[i][j - 1] == letter) get_area(i, j - 1, letter);
	}

	if (i < rows - 1) {
	       if (plot[i + 1][j] == letter) get_area(i + 1, j, letter);
	}

	if (j < cols - 1) {
		if (plot[i][j + 1] == letter) get_area(i, j + 1, letter);
	}
}

size_t get_sides() {
	unsigned found_region, no_region = 1, top_side, bot_side;
	size_t sides = 0;

	for (size_t i = 0; i < rows; i++) {
		found_region = 0;
		top_side = 0;
		bot_side = 0;

		for (size_t j = 0; j < cols; j++) {
			if (! region[i][j]) { top_side = bot_side = 0; continue; }

			if (! top_side) {
				if (! i || ! region[i - 1][j]) {
					top_side = 1;
					sides++;
				}
			}
			else if (i && region[i - 1][j]) top_side = 0;

			if (! bot_side) {
				if (i == rows - 1 || ! region[i + 1][j]) {
					bot_side = 1;
					sides++;
				}
			}
			else if (i != rows - 1 && region[i + 1][j]) bot_side = 0;

			found_region = 1;
			no_region = 0;
		}

		if (! found_region && ! no_region) break;
	}

	no_region = 1;
	unsigned left_side, right_side;

	for (size_t j = 0; j < cols; j++) {
		found_region = 0;
		left_side = 0;
		right_side = 0;

		for (size_t i = 0; i < rows; i++) {
			if (! region[i][j]) { left_side = right_side = 0; continue; }

			if (! left_side) {
				if (! j || ! region[i][j - 1]) {
					left_side = 1;
					sides++;
				}
			}
			else if (j && region[i][j - 1]) left_side = 0;

			if (! right_side) {
				if (j == cols - 1 || ! region[i][j + 1]) {
					right_side = 1;
					sides++;
				}
			}
			else if (j != cols - 1 && region[i][j + 1]) right_side = 0;

			found_region = 1;
			no_region = 0;
		}

		if (! found_region && ! no_region) break;
	}

	return sides;
}
