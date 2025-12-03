#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PATH_LEN 16384

enum DIRS { North, East, South, West, None};
typedef struct {
	unsigned x;
	unsigned y;
} COORD_T;

int walk_path(char **, unsigned, unsigned, unsigned, int);

int compar(const void *, const void *); 

COORD_T *PATH = NULL;
unsigned STEPS = 0;

int main(int argc, char **argv) {
	FILE *infile = fopen(argv[1], "r");
	
	char **pipe_map = NULL;
	unsigned num_lines = 0;

	char *line = NULL;
	size_t n = 0;
	ssize_t num_char;

	while ( (num_char = getline(&line, &n, infile)) > 1) {
		pipe_map = realloc(pipe_map, (num_lines + 1) * sizeof(*pipe_map));
		pipe_map[num_lines] = malloc(num_char * sizeof(char));
		strncpy(pipe_map[num_lines], line, num_char);
		pipe_map[num_lines][num_char - 1] = '\0';
		num_lines++;
	}

	fclose(infile);
	free(line);

	unsigned x, y;

	for (y = 0; y < num_lines; y++) {
		for (x = 0; pipe_map[y][x]; x++) {
			if (pipe_map[y][x] == 'S') goto FoundStart;
		}
	}

FoundStart:

	PATH = malloc(PATH_LEN * sizeof(*PATH));

	walk_path(pipe_map, num_lines, x, y, None);

	qsort(PATH, STEPS, sizeof(*PATH), &compar);

	char is_inside;
	COORD_T search;
	unsigned sum = 0;

	for (unsigned i = 0; i < num_lines; i++) {
	       	is_inside = 0;
		search.y = i;
		for (unsigned j = 0; pipe_map[i][j]; j++) {
			search.x = j;	

			if (! bsearch(&search, PATH, STEPS, sizeof(*PATH), compar)) {
				if (is_inside) sum++;

				continue;
			}

			if (pipe_map[i][j] == 'F' || 
			    pipe_map[i][j] == '7' || 
			    pipe_map[i][j] == '|' || 
			    pipe_map[i][j] == 'S') {
				is_inside ^= 1;
			}
		}

		free(pipe_map[i]);
	}

	free(PATH);
	free(pipe_map);

	printf("Num Inside = %d\n", sum);

	return 0;
}

int compar(const void *a, const void *b) {
	COORD_T coord1 = *(COORD_T *)a;	
	COORD_T coord2 = *(COORD_T *)b;

	if (coord1.y != coord2.y) return coord1.y - coord2.y;
	
	return coord1.x - coord2.x;
}

int walk_path(char **map, unsigned lines, unsigned x, unsigned y, int prev) {
	int result = -1;

	switch (map[y][x]) {
		case 'S':	
			if (prev != None) { result = 0; break; }
			
			if (y && (map[y - 1][x] == '|' || map[y - 1][x] == '7' || map[y - 1][x] == 'F')) {
				if ( (result = walk_path(map, lines, x, y - 1, South)) != -1) break;
			}
			if (map[y][x+1] && (map[y][x+1] == '-' || map[y][x+1] == 'J' || map[y][x+1] == '7')) {
				if ( (result = walk_path(map, lines, x + 1, y, West)) != -1) break;
			}
			if (y < lines-1 && (map[y+1][x] == '|' || map[y+1][x] == 'L' || map[y+1][x] == 'J')) {
				if ( (result = walk_path(map, lines, x, y + 1, North)) != -1) break;
			}
			if (x && (map[y][x - 1] == '-' || map[y][x - 1] == 'L' || map[y][x - 1] == 'F')) {
				if ( (result = walk_path(map, lines, x - 1, y, East)) != -1) break;;
			}
			
			break;
		case '|':
			if (prev == North && y < lines - 1) result = walk_path(map, lines, x, y + 1, North);
			else if (prev == South && y) result = walk_path(map, lines, x, y - 1, South);
			
			break;	
		case '-':
			if (prev == East && x) result = walk_path(map, lines, x - 1, y, East);
			else if (prev == West && map[y][x + 1]) result = walk_path(map, lines, x + 1, y, West);
			
			break;
		case 'L':
			if (prev == North && map[y][x + 1]) result = walk_path(map, lines, x + 1, y, West);
			else if (prev == East && y) result = walk_path(map, lines, x, y - 1, South);

			break;
		case 'J':
			if (prev == North && x) result = walk_path(map, lines, x - 1, y, East);
			else if (prev == West && y) result = walk_path(map, lines, x, y - 1, South);
			
			break;
		case '7':
			if (prev == South && x) result = walk_path(map, lines, x - 1, y, East);
			else if (prev == West && y < lines - 1) result = walk_path(map, lines, x, y + 1, North);

			break;
		case 'F':
			if (prev == East && y < lines - 1) result = walk_path(map, lines, x, y + 1, North);
			else if (prev == South && map[y][x + 1]) result = walk_path(map, lines, x + 1, y, West);

			break;
	}

	if (result == -1) return result;

	PATH[STEPS].x = x;
	PATH[STEPS].y = y;
	STEPS++;

	if (! result) return result;

	return result + 1;
}
