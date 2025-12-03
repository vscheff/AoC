#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	char *line_ptr = NULL;
	size_t n = 0;
	FILE *in_file = fopen(argv[1], "r");
	unsigned vowel, dub_let, bad_str, total = 0;

	while (getline(&line_ptr, &n, in_file) != -1) {
		vowel = 0;
		dub_let = 0;
		bad_str = 1;

		for (unsigned i = 0; ; i++) {
			if (line_ptr[i] == 'a' || line_ptr[i] == 'e' || line_ptr[i] == 'i' || line_ptr[i] == 'o' || line_ptr[i] == 'u') vowel++;
			
			if (! line_ptr[i + 1]) break;

			if (line_ptr[i] == line_ptr[i + 1]) dub_let = 1;

			if ( (line_ptr[i] == 'a' && line_ptr[i + 1] == 'b') ||
			     (line_ptr[i] == 'c' && line_ptr[i + 1] == 'd') ||
			     (line_ptr[i] == 'p' && line_ptr[i + 1] == 'q') ||
			     (line_ptr[i] == 'x' && line_ptr[i + 1] == 'y') ){
				bad_str = 0;
				break;
			}
		}
		
		if (vowel > 2 && dub_let && bad_str) total++;
	}

	fclose(in_file);
	free(line_ptr);

	printf("Total = %u\n", total);

	return 0;
}
