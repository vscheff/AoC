#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint64_t parse_line(char *);

int main(int argc, char **argv) {
	FILE *infile = fopen(argv[1], "r");

	char *line = NULL;
	size_t n = 0;

	getline(&line, &n, infile);
	
	uint64_t time = parse_line(line);

	getline(&line, &n, infile);

	fclose(infile);
	
	uint64_t distance = parse_line(line);

	free(line);
	
	unsigned sum = 0;

	for (unsigned i = 1; i < time - 1; i++) if (i * (time - i) > distance) sum++;

	printf("Sum: %u\n", sum);

	return 0;
}

uint64_t parse_line(char *line) {
	char *str_ptr = line, *end_ptr = line;
	strsep(&str_ptr, ":");
	
	char *str_buf = NULL;
	unsigned str_len = 0;

	while (*str_ptr != '\n') {
		if (*str_ptr >= '0' && *str_ptr <= '9') {
			str_buf = realloc(str_buf, ++str_len * sizeof(char));
			str_buf[str_len - 1] = *str_ptr;
		}
		
		str_ptr++;
	}

	str_buf = realloc(str_buf, (str_len + 1) * sizeof(char));
	str_buf[str_len] = '\0';

	return strtoul(str_buf, NULL, 10);
}
