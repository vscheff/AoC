#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	char *line_ptr = NULL;
	size_t n = 0, str_num = 0, mem_num = 0;
	ssize_t num_char;
	FILE *in_file = fopen(argv[1], "r");

	while ( (num_char = getline(&line_ptr, &n, in_file)) > 0) {
		str_num += num_char - 1;

		mem_num += 6;

		for (ssize_t i = 1; i < num_char - 2; i++) {
			mem_num++;

			if (line_ptr[i] == '\\') {
				mem_num++;
				i++;

				if (line_ptr[i] == '\\' || line_ptr[i] == '"') mem_num += 2;
				else if (line_ptr[i] == 'x') { i += 2; mem_num += 3; }
			}
		}
		
		printf("%lu\n", mem_num);
	}

	fclose(in_file);
	free(line_ptr);

	printf("%lu - %lu = %lu\n", mem_num, str_num, mem_num - str_num);

	return 0;
}
