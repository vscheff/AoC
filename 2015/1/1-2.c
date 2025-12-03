#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	char *line_ptr = NULL;
	size_t n = 0;
	FILE *in_file = fopen(argv[1], "r");

	while (getline(&line_ptr, &n, in_file) != -1) {
		int floor = 0;
		unsigned pos;
		
		for (unsigned i = 0; line_ptr[i]; i++) {
			if (line_ptr[i] == '(') floor++;
			else if (line_ptr[i] == ')') {
				floor--;
			
				if (floor == -1) { pos = i + 1; break; }
			}
		}

		printf("Position = %u\n", pos);
	}

	fclose(in_file);
	free(line_ptr);

	return 0;
}
