#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
	FILE *infile = fopen(argv[1], "r");

	char *line = NULL;
	size_t n = 0;
	ssize_t num_char = 0;

	char **inp_matrix = NULL;
	size_t height = 0, width = 0;

	while ( (num_char = getline(&line, &n, infile)) != -1) {
		inp_matrix = realloc(inp_matrix, ++height * sizeof(char *));
		inp_matrix[height - 1] = malloc((num_char + 1) * sizeof(char));
		strcpy(inp_matrix[height - 1], line);
		width = num_char - 1;
	}

	fclose(infile);
	free(line);

	long part_num;
	char *end_ptr = NULL;
	unsigned part_length, sum = 0;;

	for (unsigned i = 0; i < height; i++) {
		for (unsigned j = 0; j < width; j++) {
			if (inp_matrix[i][j] != '*') continue;

			unsigned min_i = 0, min_j = 0, max_i = height - 1, max_j = height - 1;

			if (i) min_i = i - 1;
			if (j) min_j = j - 1;
			if (i < max_i) max_i = i + 1;
			if (j < max_j) max_j = j + 1;

			long first_num = -1, second_num = -1;

			for (unsigned y = min_i; y <= max_i; y++) {
				for (unsigned x = min_j; x <= max_j; x++) {
					if (inp_matrix[y][x] < '0' || inp_matrix[y][x] > '9') continue;

					int z = x - 1;

					for (; z >= 0 && inp_matrix[y][z] >= '0' && inp_matrix[y][z] <= '9'; z--);
					
					if (first_num == -1) first_num = strtol(&inp_matrix[y][++z], &end_ptr, 10);
					else { second_num = strtol(&inp_matrix[y][++z], &end_ptr, 10); break; } 
					
					x += end_ptr - &inp_matrix[y][x];
				}

				if (second_num != -1) break;

			}

			if (second_num != -1) sum += first_num * second_num;
		}
	}

	for (unsigned i = 0; i < height; i++) free(inp_matrix[i]);

	free(inp_matrix);

	printf("Sum = %u\n", sum);

	return 0;
}
