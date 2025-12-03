#include <search.h>
#include <stdio.h>
#include <stdlib.h>

#define _GNU_SOURCE

typedef struct COORD_T {
	unsigned x;
	unsigned y;
} COORD_T;

void print_map(char **, size_t);
unsigned check_move(char **, COORD_T, char, COORD_T **, size_t *);

int compare_sort(const void *a, const void *b, void *arg) {
	COORD_T *a_box = (COORD_T *)a, *b_box = (COORD_T *)b;
	char dir = *(char *)arg;

	switch (*(char *)arg)
	{
	case '^':
		if (a_box->y < b_box->y) return -1;
		if (a_box->y > b_box->y) return 1;
		if (a_box->x < b_box->x) return -1;
		if (a_box->x > b_box->x) return 1;

		return 0;
	case '>':
		if (a_box->x < b_box->x) return 1;
		if (a_box->x > b_box->x) return -1;
		if (a_box->y < b_box->y) return -1;
		if (a_box->y > b_box->y) return 1;

		return 0;
	case 'v':
		if (a_box->y < b_box->y) return 1;
		if (a_box->y > b_box->y) return -1;
		if (a_box->x < b_box->x) return -1;
		if (a_box->x > b_box->x) return 1;

		return 0;
	case '<':
		if (a_box->x < b_box->x) return -1;
		if (a_box->x > b_box->x) return 1;
		if (a_box->y < b_box->y) return -1;
		if (a_box->y > b_box->y) return 1;

		return 0;
	}

}

int main(int argc, char **argv) {
	char *line_ptr = NULL, **map = NULL;
	size_t n = 0, num_char, rows = 0;
	FILE *in_file = fopen(argv[1], "r");

	while ( (num_char = getline(&line_ptr, &n, in_file)) > 1) {
		map = realloc(map, (rows + 1) * sizeof(*map));
		map[rows] = calloc((num_char - 1) * 2 + 1, sizeof(**map));
		
		for (size_t i = 0; i < num_char; i++) {
			switch (line_ptr[i])
			{
			case '#':
				map[rows][2 * i] = map[rows][2 * i + 1] = '#';
				break;
			case 'O':
				map[rows][2 * i] = '[';
				map[rows][2 * i + 1] = ']';
				break;
			case '.':
				map[rows][2 * i] = map[rows][2 * i + 1] = '.';
				break;
			case '@':
				map[rows][2 * i] = '@';
				map[rows][2 * i + 1] = '.';
			default:
			}
		}

		rows++;
	}

	//printf("Initital state\n");
	//print_map(map, rows);

	COORD_T pos;

	for (size_t i = 0; i < rows; i++) {
		for (size_t j = 0; map[i][j]; j++) {
			if (map[i][j] == '@') {
				pos.x = j;
				pos.y = i;
			}
		}
	}

	unsigned x, y;
	size_t num_boxes;
	COORD_T *boxes = NULL, check_pos;

	while (getline(&line_ptr, &n, in_file) != -1) {
		for (size_t i = 0; line_ptr[i] > '\n'; i++) {
			num_boxes = 0;
			check_pos.x = pos.x;
			check_pos.y = pos.y;

			switch (line_ptr[i])
			{
			case '^':
				check_pos.y--;
				break;
			case '>':
				check_pos.x++;
				break;
			case 'v':
				check_pos.y++;
				break;
			case '<':
				check_pos.x--;
			}

			//printf("Move %c:\n", line_ptr[i]);
			
			if (! check_move(map, check_pos, line_ptr[i], &boxes, &num_boxes)) {
				//print_map(map, rows);

				continue;
			}

			qsort_r(boxes, num_boxes, sizeof(*boxes), &compare_sort, line_ptr + i);

			switch (line_ptr[i])
			{
			case '^':
				for (size_t j = 0; j < num_boxes; j++) {
					map[boxes[j].y - 1][boxes[j].x] = '[';
					map[boxes[j].y - 1][boxes[j].x + 1] = ']';
					map[boxes[j].y][boxes[j].x] = '.';
					map[boxes[j].y][boxes[j].x + 1] = '.';
				}
			
				map[pos.y][pos.x] = '.';
				pos.y--;
				map[pos.y][pos.x] = '@';

				break;
			case '>':
				for (size_t j = 0; j < num_boxes; j++) {
					map[boxes[j].y][boxes[j].x + 1] = '[';
					map[boxes[j].y][boxes[j].x + 2] = ']';
					map[boxes[j].y][boxes[j].x] = '.';
				}

				map[pos.y][pos.x] = '.';
				pos.x++;
				map[pos.y][pos.x] = '@';

				break;
			case 'v':
				for (size_t j = 0; j < num_boxes; j++) {
					map[boxes[j].y + 1][boxes[j].x] = '[';
					map[boxes[j].y + 1][boxes[j].x + 1] = ']';
					map[boxes[j].y][boxes[j].x] = '.';
					map[boxes[j].y][boxes[j].x + 1] = '.';
				}

				map[pos.y][pos.x] = '.';
				pos.y++;
				map[pos.y][pos.x] = '@';

				break;
			case '<':
				for (size_t j = 0; j < num_boxes; j++) {
					map[boxes[j].y][boxes[j].x - 1] = '[';
					map[boxes[j].y][boxes[j].x] = ']';
					map[boxes[j].y][boxes[j].x + 1] = '.';
				}

				map[pos.y][pos.x] = '.';
				pos.x--;
				map[pos.y][pos.x] = '@';
			}
			
			//print_map(map, rows);
		}
	}

	fclose(in_file);
	free(line_ptr);
	free(boxes);

	size_t total = 0;

	for (size_t i = 0; i < rows; i++) {
		for (size_t j = 0; map[i][j]; j++) {
			if (map[i][j] == '[') total += 100 * i + j;
		}

		free(map[i]);
	}

	free(map);

	printf("Total = %lu\n", total);

	return 0;
}

int compare(const void *a, const void *b) {
	COORD_T *a_box = (COORD_T *)a, *b_box = (COORD_T *)b;

	if (a_box->x < b_box->x) return -1;
	if (a_box->x > b_box->x) return 1;
	if (a_box->y < b_box->y) return -1;
	if (a_box->y > b_box->y) return 1;

	return 0;
}

unsigned add_box(COORD_T **boxes, size_t *num_boxes, unsigned x, unsigned y) {
	COORD_T search_box = {x, y};

	if (lfind(&search_box, *boxes, num_boxes, sizeof(**boxes), &compare)) return 0;

	*boxes = realloc(*boxes, ++(*num_boxes) * sizeof(**boxes));
	(*boxes)[*num_boxes - 1] = search_box;

	return 1;
}

unsigned check_move(char **map, COORD_T pos, char dir, COORD_T **boxes, size_t *num_boxes) {
	unsigned x, y;
	
	switch (dir)
	{
	case '^':
		for (y = pos.y; ; y--) {
			switch (map[y][pos.x])
			{
			case '.': 
				return 1;
			case '#':
				return 0;
			case '[':
				if (! add_box(boxes, num_boxes, pos.x, y)) continue;

				COORD_T right_pos = {pos.x + 1, y - 1};
				
				if (! check_move(map, right_pos, dir, boxes, num_boxes)) return 0;

				break;
			case ']':
				if (! add_box(boxes, num_boxes, pos.x - 1, y)) continue;

				COORD_T left_pos = {pos.x - 1, y - 1};
				
				if (! check_move(map, left_pos, dir, boxes, num_boxes)) return 0;
			}
		}
	case '>':
		for (x = pos.x; ; x++) {
			switch (map[pos.y][x])
			{
			case '.': 
				return 1;
			case '#':
				return 0;
			case '[':
				*boxes = realloc(*boxes, ++(*num_boxes) * sizeof(**boxes));
				(*boxes)[*num_boxes - 1].x = x;
				(*boxes)[*num_boxes - 1].y = pos.y;
				x++;
			default:
			}
		}
	case 'v':
		for (y = pos.y; ; y++) {
			switch (map[y][pos.x])
			{
			case '.': 
				return 1;
			case '#':
				return 0;
			case '[':
				if (! add_box(boxes, num_boxes, pos.x, y)) continue;

				COORD_T right_pos = {pos.x + 1, y + 1};
				
				if (! check_move(map, right_pos, dir, boxes, num_boxes)) return 0;

				break;
			case ']':
				if (! add_box(boxes, num_boxes, pos.x - 1, y)) continue;

				COORD_T left_pos = {pos.x - 1, y + 1};
				
				if (! check_move(map, left_pos, dir, boxes, num_boxes)) return 0;
			}
		}
	case '<':
		for (x = pos.x; ; x--) {
			switch (map[pos.y][x])
			{
			case '.': 
				return 1;
			case '#':
				return 0;
			case '[':
				*boxes = realloc(*boxes, ++(*num_boxes) * sizeof(**boxes));
				(*boxes)[*num_boxes - 1].x = x;
				(*boxes)[*num_boxes - 1].y = pos.y;
			default:
			}
		}
	}

}

void print_map(char **map, size_t rows) {
	for (size_t i = 0; i < rows; i++) printf("%s\n", map[i]);

	printf("\n");
}
