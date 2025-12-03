#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum DIRS {Up, Left, Down, Right};

void travel_path(char **, char **, char **, unsigned, unsigned, unsigned, unsigned, int);

int main(int argc, char **argv) {
	FILE *infile = fopen(argv[1], "r");

	char *line = NULL, **layout = NULL, **energized = NULL, **split = NULL;
	size_t n = 0;
       	unsigned rows = 0, cols;
	ssize_t num_char;

	while ( (num_char = getline(&line, &n, infile)) > 1) {
		line[num_char - 1] = '\0';
		
		layout = realloc(layout, ++rows * sizeof(*layout));
		energized = realloc(energized, rows * sizeof(*energized));
		split = realloc(split, rows * sizeof(*energized));

		layout[rows - 1] = strdup(line);
		
		energized[rows - 1] = calloc(num_char - 1, sizeof(char));
		split[rows - 1] = calloc(num_char - 1, sizeof(char));
		
		cols = num_char - 1;
	}

	fclose(infile);
	free(line);

	travel_path(layout, energized, split, 0, 0, rows, cols, Right);

	unsigned sum = 0;

	for (unsigned i = 0; i < rows; i++) {
		for (unsigned j = 0; layout[i][j]; j++) sum += energized[i][j];

		free(layout[i]);
		free(energized[i]);
		free(split[i]);
	}

	free(layout);
	free(energized);
	free(split);

	printf("Sum = %u\n", sum);

	return 0;
}

void travel_path(char **layout, char **energized, char **split, unsigned i, unsigned j, unsigned rows, unsigned cols, int dir) {
loop_top:
	if (i >= rows || j >= cols) return;

	energized[i][j] = 1;

	switch (layout[i][j]) {
		case '.':
			switch (dir) {
				case Up: 
					i--;
					break;
				case Right:
					j++;
					break;
				case Down:
					i++;
					break;
				case Left:
					j--;
			}
			break;
		case '/':
			switch (dir) {
				case Up:
					j++;
					dir = Right;
					break;
				case Right:
					i--;
					dir = Up;
					break;
				case Down:
					j--;
					dir = Left;
					break;
				case Left:
					i++;
					dir = Down;
			}
			break;
		case '\\':
			switch (dir) {
				case Up:
					j--;
					dir = Left;
					break;
				case Right:
					i++;
					dir = Down;
					break;
				case Down:
					j++;
					dir = Right;
					break;
				case Left:
					i--;
					dir = Up;
			}
			break;
		case '|':
			switch (dir) {
				case Up:
					i--;
					break;
				case Right:
					if (split[i][j]) return;
					
					split[i][j] = 1;
					travel_path(layout, energized, split, i - 1, j, rows, cols, Up);
					i++;
					dir = Down;
					break;
				case Down:
					i++;
					break;
				case Left:
					if (split[i][j]) return;
					
					split[i][j] = 1;
					travel_path(layout, energized, split, i - 1, j, rows, cols, Up);
					i++;
					dir = Down;
			}
			break;
		case '-':
			switch (dir) {
				case Up:
					if (split[i][j]) return;
					
					split[i][j] = 1;
					travel_path(layout, energized, split, i, j - 1, rows, cols, Left);
					j++;
					dir = Right;
					break;
				case Right:
					j++;
					break;
				case Down:
					if (split[i][j]) return;
					
					split[i][j] = 1;
					travel_path(layout, energized, split, i, j - 1, rows, cols, Left);
					j++;
					dir = Right;
					break;
				case Left:
					j--;
			}
			break;
	}

	goto loop_top;
}
