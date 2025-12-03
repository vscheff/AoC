#include <search.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIN_REPEAT 1024 

void cycle(char **, int);
char *stringify(char **, int);
uint64_t score(char **, int);

int compar(const void *a, const void *b) { return strcmp(a, b); }

int main(int argc, char **argv) {
	FILE *infile = fopen(argv[1], "r");

	uint64_t cycles = strtoul(argv[2], NULL, 10);
	char *line = NULL, **platform = NULL, **platform_bu = NULL;
	size_t n = 0;
	unsigned num_rows = 0;
	ssize_t num_char;

	printf("Input:\n\n");

	while ( (num_char = getline(&line, &n, infile)) > 1) {
		printf("%s", line);
		line[num_char - 1] = '\0';
		platform = realloc(platform, ++num_rows * sizeof(*platform));
		platform_bu = realloc(platform_bu, num_rows * sizeof(*platform_bu));
		platform[num_rows - 1] = strdup(line);
		platform_bu[num_rows - 1] = strdup(line);
	}

	fclose(infile);
	free(line);

	uint64_t sum, cycle_base, i, offset;
	char **states = NULL, **found = NULL, *string = NULL;
	size_t num_states = 0;

	printf("\n");

	for (i = 0; i < cycles; i++) {
		cycle(platform, num_rows);
		states = realloc(states, (num_states + 1) * sizeof(*states));
		string = stringify(platform, num_rows);
		states[num_states] = string;

		if(! found) {
			if ( (found = lfind(&string, states, &num_states, sizeof(*states), &compar)) ) {
				printf("Found cycle at: %lu\n", i);

				cycle_base = found - states;
				offset = 0;	
			}
		}
		else if (! strcmp(states[cycle_base + ++offset], states[num_states])) {
			if (! strcmp(states[cycle_base], states[num_states]))
				break;
		}
		else {
			found = NULL;
		}

		num_states++;
	}

	free(states);

	uint64_t num_cycles;
	
	if (i == cycles) num_cycles = cycles;
	else {
		num_cycles = (cycles - cycle_base) / offset;
		num_cycles += cycles % num_cycles;
	}

	printf("\nCycling %lu times\n", num_cycles);
	
	for (i = 0; i < num_cycles; i++) cycle(platform_bu, num_rows);

	sum = score(platform_bu, num_rows);

	printf("\nOutput:\n\n");

	for (i = 0; i < num_rows; i++) {
		printf("%s\n", platform_bu[i]);
		free(platform[i]);
		free(platform_bu[i]);
	}

	free(platform);
	free(platform_bu);

	printf("\nSum = %lu\n", sum);

	return 0;
}

void cycle(char **platform, int num_rows) {
	int i, j, k, width = 0;

	// North
	for (i = 0; i < num_rows; i++) {
		for (j = 0; platform[i][j]; j++) {
			if (platform[i][j] != 'O') continue;
			
			for (k = i - 1; k >= 0; k--) {
				if (platform[k][j] != '.') break;
			}

			if (i == ++k) continue;

			platform[k][j] = 'O';
			platform[i][j] = '.';
		}

		width = j;
	}

	//printf("\nNorth: \n\n");

	//for (i = 0; i < num_rows; i++) printf("%s\n", platform[i]);
	
	// West
	for (i = 0; i < num_rows; i++) {
		for (j = 0; platform[i][j]; j++) {
			if (platform[i][j] != 'O') continue;

			for (k = j - 1; k >= 0; k--) {
				if (platform[i][k] != '.') break;
			}

			if (j == ++k) continue;

			platform[i][k] = 'O';
			platform[i][j] = '.';
		}
	}
	
	//printf("\nWest: \n\n");

	//for (i = 0; i < num_rows; i++) printf("%s\n", platform[i]);
	
	// South
	for (i = num_rows - 1; i >= 0; i--) {
		for (j = 0; platform[i][j]; j++) {
			if (platform[i][j] != 'O') continue;

			for (k = i + 1; k < num_rows; k++) {
				if (platform[k][j] != '.') break;
			}

			if (i == --k) continue;

			platform[k][j] = 'O';
			platform[i][j] = '.';
		}
	}

	//printf("\nSouth: \n\n");

	//for (i = 0; i < num_rows; i++) printf("%s\n", platform[i]);
	
	// East
	for (i = 0; i < num_rows; i++) {
		for (j = width - 1; j >= 0; j--) {
			if (platform[i][j] != 'O') continue;

			for (k = j + 1; platform[i][k]; k++) {
				if (platform[i][k] != '.') break;
			}

			if (j == --k) continue;

			platform[i][k] = 'O';
			platform[i][j] = '.';
		}
	}

	//printf("\nEast: \n\n");

	//for (i = 0; i < num_rows; i++) printf("%s\n", platform[i]);
}

char *stringify(char **platform, int num_rows) {
	size_t str_len = strlen(platform[0]);
	char *string = malloc(str_len * num_rows + 1);

	for (unsigned i = 0; i < num_rows; i++) strcpy(&string[i * str_len], platform[i]);

	return string;
}

uint64_t score(char **platform, int num_rows) {
	uint64_t sum = 0;
	
	for (unsigned i = 0; i < num_rows; i++) {
		int val = num_rows - i;
		for (unsigned j = 0; platform[i][j]; j++) if (platform[i][j] == 'O') sum += val;
	}

	return sum;
}
