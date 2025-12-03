#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **plot = NULL;
unsigned **checked = NULL;
size_t rows = 0, cols, area, perimeter;

void get_region(size_t, size_t, char);

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
		line_ptr = NULL;
		n = 0;
	}

	fclose(in_file);

	cols = strlen(plot[0]);
	char curr_letter = '\0';
	size_t total = 0;

	for (size_t i = 0; i < rows; i++) {
		for (size_t j = 0; j < cols; j++) {
			area = perimeter = 0;
			get_region(i, j, plot[i][j]);
			total += area * perimeter;
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

void get_region(size_t i, size_t j, char letter) {
	if (checked[i][j]) return;
	
	checked[i][j] = 1;
	area++;

	if (i) {
		if (plot[i - 1][j] == letter) get_region(i - 1, j, letter);
		else perimeter++;
	}
	else perimeter++;

	if (j) {
		if (plot[i][j - 1] == letter) get_region(i, j - 1, letter);
		else perimeter++;
	}
	else perimeter++;

	if (i < rows - 1) {
	       if (plot[i + 1][j] == letter) get_region(i + 1, j, letter);
	       else perimeter++;
	}
	else perimeter++;

	if (j < cols - 1) {
		if (plot[i][j + 1] == letter) get_region(i, j + 1, letter);
		else perimeter++;
	}
	else perimeter++;
}
