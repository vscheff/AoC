#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RED_CNT 12
#define GRN_CNT 13
#define BLU_CNT 14

int main(int argc, char **argv) {
	FILE *infile = fopen(argv[1], "r");

	unsigned red, green, blue;

	char *line = NULL, *str_ptr = NULL, *game_ptr = NULL, *draw_ptr = NULL, *end_ptr = NULL;
	size_t n = 0, sum = 0;
	int game_id = 0;
	long draw_num = 0;

	while ( (getline(&line, &n, infile)) != -1) {
		game_id = atoi(&line[5]);

		str_ptr = line;
		strsep(&str_ptr, ":");
		str_ptr++;

		game_ptr = strsep(&str_ptr, ";");

		red = green = blue = 0;

		for (;;) {
			draw_ptr = strsep(&game_ptr, ",");

			for (;;) {
				draw_num = strtol(draw_ptr, &end_ptr, 10);
				end_ptr ++;
				switch (*end_ptr) {
					case 'r':
						if (draw_num > red) red = draw_num;
						break;
					case 'g':
						if (draw_num > green) green = draw_num;
						break;
					case 'b':
						if (draw_num > blue) blue = draw_num;
						break;
				}

				if (! game_ptr) break;
				
				game_ptr++;
				draw_ptr = strsep(&game_ptr, ",");
			}
			
			if (! str_ptr) break;

			str_ptr++;
			game_ptr = strsep(&str_ptr, ";");

		}

		if (red <= RED_CNT && green <= GRN_CNT && blue <= BLU_CNT) sum += game_id;

	}

	printf("Sum = %u\n", sum);

	fclose(infile);
	free(line);

	return 0;
}
