#include <stdio.h>
#include <stdlib.h>

typedef struct COORD_T {
	long x;
	long y;
} COORD_T;

typedef struct MACHINE_T {
	COORD_T a;
	COORD_T b;
	COORD_T prize;
} MACHINE_T;

long calc_lcm(long, long);

int main(int argc, char **argv) {
	char *line_ptr = NULL, *end_ptr;
	size_t n = 0, num_machines = 0;
	MACHINE_T *machines = NULL;
	FILE *in_file = fopen(argv[1], "r");

	while (getline(&line_ptr, &n, in_file) != -1) {
		if (*line_ptr == '\n') continue;

		machines = realloc(machines, (num_machines + 1) * sizeof(*machines));
		machines[num_machines].a.x = strtol(line_ptr + 12, &end_ptr, 10);
		machines[num_machines].a.y = strtol(end_ptr + 4, NULL, 10);

		getline(&line_ptr, &n, in_file);

		machines[num_machines].b.x = strtol(line_ptr + 12, &end_ptr, 10);
		machines[num_machines].b.y = strtol(end_ptr + 4, NULL, 10);
		
		getline(&line_ptr, &n, in_file);

		machines[num_machines].prize.x = strtol(line_ptr + 9, &end_ptr, 10) + 10000000000000;
		machines[num_machines].prize.y = strtol(end_ptr + 4, NULL, 10) + 10000000000000;
	
		num_machines++;
	}

	fclose(in_file);
	free(line_ptr);

	long lcm, a_mult, b_mult, left_val, right_val, a_val, b_val, total = 0;
	COORD_T big, small;

	for (size_t i = 0; i < num_machines; i++) {
		lcm = calc_lcm(machines[i].a.x, machines[i].a.y);
		a_mult = lcm / machines[i].a.x;
		b_mult = lcm / machines[i].a.y;
		left_val = machines[i].b.y * b_mult - machines[i].b.x * a_mult;
		right_val = machines[i].prize.y * b_mult - machines[i].prize.x * a_mult;

		if (right_val % left_val) continue;

		b_val = right_val / left_val;
		right_val = machines[i].prize.x - b_val * machines[i].b.x;

		if (right_val % machines[i].a.x) continue;

		total += 3 * (right_val / machines[i].a.x) + b_val;
	}

	free(machines);

	printf("Total = %ld\n", total);

	return 0;
}

long calc_lcm(long x, long y) {
	long max = (x > y) ? x : y;
	
	for ( ; ; ) {
		if (max % x == 0 && max % y == 0) return max;

		max++;
	}
}
