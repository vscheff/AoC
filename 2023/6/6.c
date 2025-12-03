#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint64_t *parse_line(char *, unsigned *);

int main(int argc, char **argv) {
	FILE *infile = fopen(argv[1], "r");

	char *line = NULL;
	size_t n = 0;

	getline(&line, &n, infile);
	
	unsigned races;
	uint64_t *times = parse_line(line, &races);

	getline(&line, &n, infile);

	fclose(infile);
	
	uint64_t *distances = parse_line(line, &races);

	free(line);
	
	unsigned sum = 1, num, max_check;

	for (unsigned i = 0; i < races; i++) {
		num = 0;
		
		for (unsigned j = 1; j < times[i] - 1; j++) if (j * (times[i] - j) > distances[i]) num++;
		
		sum *= num;
	}

	printf("Sum: %u\n", sum);

	return 0;
}

uint64_t *parse_line(char *line, unsigned *count) {
	char *str_ptr = line, *end_ptr = line;
	strsep(&str_ptr, ":");
	uint64_t *times = NULL;
	*count = 0;
	
	while (*end_ptr != '\n') {
		while (*str_ptr == ' ') str_ptr++;
		times = realloc(times, ++*count * sizeof(*times));
		times[*count - 1] = strtoul(str_ptr, &end_ptr, 10);
		str_ptr = end_ptr;
	}

	return times;
}
