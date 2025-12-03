#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

unsigned evaluate(unsigned long, unsigned long *, size_t, unsigned long);

int main(int argc, char **argv) {
	char *line_ptr = NULL, *end_ptr = NULL, *num_ptr = NULL;
	size_t n = 0, count;
	FILE *in_file = fopen(argv[1], "r");
	unsigned long val, *nums = NULL;
	uint64_t total = 0;
	unsigned line = 1;

	while (getline(&line_ptr, &n, in_file) != -1) {
		val = strtoul(line_ptr, &end_ptr, 10);

		count = 0;
		end_ptr++;

		while (*end_ptr != '\n' && *end_ptr) {
			nums = realloc(nums, ++count * sizeof(*nums));
			nums[count - 1] = strtoul(end_ptr + 1, &end_ptr, 10);
		}

		if (evaluate(val, nums + 1, count - 1, nums[0])) { printf("%u Valid\n", line); total += val; }
		else {
			printf("%u Invalid\n", line);
			printf("\tvalue = %lu\n", val);
			printf("\tarray = [");
			for (size_t i = 0; i < count; i++) printf(" %lu", nums[i]);
			printf(" ]\n");
		}

		free(nums);
		nums = NULL;
	
		line++;
	}

	fclose(in_file);
	free(line_ptr);

	printf("Total = %lu\n", total);

	return 0;
}

unsigned evaluate(unsigned long val, unsigned long *nums, size_t count, unsigned long result) {
	if (count == 1) {
		if (val == result + *nums) return 1;
		
		if (val == result * *nums) return 1;

		return 0;
	}

	if (evaluate(val, nums + 1, count - 1, result + *nums)) return 1;
	
	return evaluate(val, nums + 1, count - 1, result * *nums);
}
