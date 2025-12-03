#include <stdio.h>
#include <stdlib.h>

ssize_t get_num(char*, ssize_t);

int main(int argc, char **argv) {
	FILE *infile = fopen(argv[1], "r");

	if (! infile) { fprintf(stderr, "Unable to open file!\n"); return -1; }

	char * line_ptr = NULL;
	size_t n = 0, sum = 0;
	ssize_t i = 0, num_char = 0, num_found = 0;

	while ( (num_char = getline(&line_ptr, &n, infile)) != -1 ) {
		for (i = 0; i < num_char; i++) {
			if ( (num_found = get_num(line_ptr, i)) != -1) { sum += num_found * 10; break; }
		}
		
		for (i = num_char - 1; i >= 0; i--) {
			if ( (num_found = get_num(line_ptr, i)) != -1) { sum += num_found; break; }
		}
	}

	free(line_ptr);

	printf("Sum = %u\n", sum);

	return 0;
}

ssize_t get_num(char *line_ptr, ssize_t i) {
	if (line_ptr[i] >= '0' && line_ptr[i] <= '9') return line_ptr[i] - '0';

	switch (line_ptr[i]) {
		case 'o':
			if (line_ptr[i + 1] == 'n' && line_ptr[i + 2] == 'e') return 1;
			break;
		case 't':
			if (line_ptr[i + 1] == 'w' && line_ptr[i + 2] == 'o') return 2;
			if (line_ptr[i + 1] == 'h' && line_ptr[i + 2] == 'r' && line_ptr[i + 3] == 'e' && line_ptr[i + 4] == 'e') return 3;
			break;
		case 'f':
			if (line_ptr[i + 1] == 'o' && line_ptr[i + 2] == 'u' && line_ptr[i + 3] == 'r') return 4;
			if (line_ptr[i + 1] == 'i' && line_ptr[i + 2] == 'v' && line_ptr[i + 3] == 'e') return 5;
			break;
		case 's':
			if (line_ptr[i + 1] == 'i' && line_ptr[i + 2] == 'x') return 6;
			if (line_ptr[i + 1] == 'e' && line_ptr[i + 2] == 'v' && line_ptr[i + 3] == 'e' && line_ptr[i + 4] == 'n') return 7;
			break;
		case 'e':
			if (line_ptr[i + 1] == 'i' && line_ptr[i + 2] == 'g' && line_ptr[i + 3] == 'h' && line_ptr[i + 4] == 't') return 8;
			break;
		case 'n':
			if (line_ptr[i + 1] == 'i' && line_ptr[i + 2] == 'n' && line_ptr[i + 3] == 'e') return 9;
		default:
			break;
	}

	return -1;
}
