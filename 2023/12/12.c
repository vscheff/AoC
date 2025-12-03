#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned count_arrangements(char *, int *);

int main(int argc, char **argv) {
	FILE *infile = fopen(argv[1], "r");

	char *line = NULL, **map = NULL, *num_ptr = NULL, *str_ptr = NULL;
	size_t n = 0;
	unsigned map_height = 0;
	int **damaged = NULL;

	while (getline(&line, &n, infile) > 1) {
		map = realloc(map, (map_height + 1) * sizeof(*map));
		damaged = realloc(damaged, (map_height + 1) * sizeof(*damaged));
		num_ptr = line;
		line = strsep(&num_ptr, " ");
		map[map_height] = malloc((num_ptr - line) * sizeof(char));
		strcpy(map[map_height], line);
		str_ptr = num_ptr;

		unsigned num_damaged = 0;
		damaged[map_height] = NULL;

		while (str_ptr) {
			num_ptr = strsep(&str_ptr, ",");
			damaged[map_height] = realloc(damaged[map_height], ++num_damaged * sizeof(**damaged));
			damaged[map_height][num_damaged - 1] = atoi(num_ptr);
		}

		damaged[map_height] = realloc(damaged[map_height], (num_damaged + 1) * sizeof(**damaged));
		damaged[map_height][num_damaged] = -1;
		
		map_height++;
	}

	free(line);
	fclose(infile);

	unsigned sum = 0, result;

	for (unsigned i = 0; i < map_height; i++) {
		result = count_arrangements(map[i], damaged[i]);
		sum += result;
		free(map[i]);
		free(damaged[i]);
	}

	free(damaged);
	free(map);

	printf("Sum = %u\n", sum);
	
	return 0;
}

unsigned count_arrangements(char *line, int *damaged) {
	unsigned index = 0, contiguous = 0, result = 0;
	
	for (unsigned i = 0; line[i]; i++) {
		switch (line[i]) {
			case '.':
				if (contiguous) {
					if (contiguous != damaged[index]) 
						return 0;
				      	
				       	contiguous = 0;	
					index++;
				}
				
				break;
			case '#':
				if (damaged[index] == -1) 
					return 0;

				if (contiguous < damaged[index]) {
					contiguous++;
					break;
				}
				
				return 0;
			case '?':
				if (damaged[index] != -1 && contiguous < damaged[index]) {
					line[i] = '#';
					result += count_arrangements(line, damaged);
				}

				line[i] = '.';
				result += count_arrangements(line, damaged);
				
				line[i] = '?';

				return result;
			default:
				fprintf(stderr, "Bad char found: %c\n", line[i]);
		}
	}

	if (damaged[index] != -1 && (damaged[index] != contiguous || damaged[index + 1] != -1)) 
		return 0;

	
	return 1;
}
