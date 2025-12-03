#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int check_nums(int, int, int);

int main(int argc, char **argv) {
	char *line_ptr = NULL, *str_ptr = NULL;
	size_t n = 0, arr_len;
	int increasing, safe, dampened;
	int *arr = NULL;
	unsigned num_safe = 0;

	FILE *in_file = fopen(argv[1], "r");

	while (getline(&line_ptr, &n, in_file) != -1) {
		arr = malloc(sizeof(*arr));
		arr_len = 1;
		
		str_ptr = strtok(line_ptr, " ");
		arr[0] = atoi(str_ptr);
		
		while ( (str_ptr = strtok(NULL, " ")) ) {
			arr = realloc(arr, ++arr_len * sizeof(*arr));			
			arr[arr_len - 1] = atoi(str_ptr);
		}

		safe = 1;
		dampened = 0;

		unsigned inc = 0, dec = 0;

		for (size_t i = 1; i < arr_len; i++) {
			if (arr[i] > arr[i - 1]) inc++;
			else if (arr[i] < arr[i - 1]) dec++;
		}

		if (inc > 1 && dec > 1) { printf("Unsafe\n"); free(arr); continue; }

		if (inc > dec) increasing = 1;
		else increasing = 0;

		int *arr_cpy = malloc((arr_len - 1) * sizeof(*arr_cpy));

		unsigned index;

		for (size_t i = 0; i < arr_len; i++) {
			index = 0;
			safe = 1;

			for (size_t j = 0; j < arr_len; j++) {
				if (i == j) continue;
				
				arr_cpy[index] = arr[j];
				index++;
			}

			for (size_t j = 0; j < arr_len - 2; j++) {
				if (check_nums(arr_cpy[j], arr_cpy[j + 1], increasing)) continue;
				
				safe = 0;
				break;
			}

			if (safe) break;	
		}

		free(arr_cpy);

		if (safe) { printf("Safe\n"); num_safe++; }
		else printf("Unsafe\n");

		free(arr);
	}

	fclose(in_file);
	free(line_ptr);

	printf("Num Safe = %u\n", num_safe);

	return 0;
}

int check_nums(int a, int b, int increasing) {
	if (a > b) {
		if (increasing == 1) return 0;
		
		if (a - b > 3) return 0;
	}
	else if (a < b) {
		if (increasing == 0) return 0;

		if (b - a > 3) return 0;
	}
	else return 0;

	return 1;
}
