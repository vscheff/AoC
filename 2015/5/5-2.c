#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	char *line_ptr = NULL;
	size_t n = 0;
	FILE *in_file = fopen(argv[1], "r");
	unsigned pair, repeat, total = 0;

	while (getline(&line_ptr, &n, in_file) != -1) {
		pair = 0; 
		repeat = 0;

		for (unsigned i = 0; ; i++) {
			if (pair && repeat || ! line_ptr[i + 2]) break;
			
			if (! pair) {
				for (unsigned j = i + 2; line_ptr[j + 1]; j++) {
					if (line_ptr[j] == line_ptr[i] && line_ptr[j + 1] == line_ptr[i + 1]) { pair = 1; break; }
				}
			}

			if (! repeat) {
				if (line_ptr[i] == line_ptr[i + 2]) repeat = 1;
			}
		}

		if (pair && repeat) total++;
	}

	fclose(in_file);
	free(line_ptr);

	printf("Total = %u\n", total);

	return 0;
}
