#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	long dst;
	long src;
	long rng;
} MAP_T;

MAP_T *read_map(FILE *, unsigned *); 
long get_num(MAP_T *, unsigned, long);

int main(int argc, char **argv) {
	FILE *infile = fopen(argv[1], "r");

	char *line = NULL;
	size_t n = 0;

	getline(&line, &n, infile);

	char *str_ptr = line;

	strsep(&str_ptr, ":");
	str_ptr++;

	char *num_ptr = NULL;
	long *seeds = NULL;
	unsigned seed_cnt = 0;

	while (str_ptr) {
		num_ptr = strsep(&str_ptr, " ");
		seeds = realloc(seeds, ++seed_cnt * sizeof(long));
		seeds[seed_cnt - 1] = strtol(num_ptr, NULL, 10);
	}

	getline(&line, &n, infile);
	getline(&line, &n, infile);

	unsigned s_to_s_cnt, s_to_f_cnt, f_to_w_cnt, w_to_l_cnt, l_to_t_cnt, t_to_h_cnt, h_to_l_cnt;
	MAP_T *seed_to_soil = read_map(infile, &s_to_s_cnt);
	getline(&line, &n, infile);
	MAP_T *soil_to_fertilizer = read_map(infile, &s_to_f_cnt);
	getline(&line, &n, infile);
	MAP_T *fertilizer_to_water = read_map(infile, &f_to_w_cnt);
	getline(&line, &n, infile);
	MAP_T *water_to_light = read_map(infile, &w_to_l_cnt);
	getline(&line, &n, infile);
	MAP_T *light_to_temperature = read_map(infile, &l_to_t_cnt);
	getline(&line, &n, infile);
	MAP_T *temperature_to_humidity = read_map(infile, &t_to_h_cnt);
	getline(&line, &n, infile);
	MAP_T *humidity_to_location = read_map(infile, &h_to_l_cnt);
	
	fclose(infile);
	free(line);
	
	MAP_T map;
	long number, smallest = INT_MAX;

	for (unsigned i = 0; i < seed_cnt; i++) {
		number = get_num(seed_to_soil, s_to_s_cnt, seeds[i]);
		printf(" - Seed %ld, soil %ld, ", seeds[i], number);
		number = get_num(soil_to_fertilizer, s_to_f_cnt, number);
		printf("fertilizer %ld, ", number);
		number = get_num(fertilizer_to_water, f_to_w_cnt, number);
		printf("water %ld, ", number);
		number = get_num(water_to_light, w_to_l_cnt, number);
		printf("light %ld, ", number);
		number = get_num(light_to_temperature, l_to_t_cnt, number);
		printf("temperature %ld, ", number);
		number = get_num(temperature_to_humidity, t_to_h_cnt, number);
		printf("humidity %ld, ", number);
		number = get_num(humidity_to_location, h_to_l_cnt, number);
		printf("location %ld.\n", number);

		if (number < smallest) smallest = number;
	}

	free(seeds);
	free(seed_to_soil);
	free(soil_to_fertilizer);
	free(fertilizer_to_water);
	free(water_to_light);
	free(light_to_temperature);
	free(temperature_to_humidity);
	free(humidity_to_location);

	printf("\nLowest location: %ld\n", smallest);

	return 0;
}

MAP_T *read_map(FILE *infile, unsigned *count) {
	char *line = NULL, *str_ptr, *num_ptr;
	size_t n = 0;

	MAP_T *mapping = NULL;
	unsigned map_count = 0;

	while (getline(&line, &n, infile) > 1) {
		str_ptr = line;
		mapping = realloc(mapping, (map_count + 1) * sizeof(MAP_T));
		num_ptr = strsep(&str_ptr, " ");
		mapping[map_count].dst = strtol(num_ptr, NULL, 10);
		num_ptr = strsep(&str_ptr, " ");
		mapping[map_count].src = strtol(num_ptr, NULL, 10);
		mapping[map_count].rng = strtol(str_ptr, NULL, 10);
		map_count++;
	}

	free(line);

	*count = map_count;
	return mapping;
}

long get_num(MAP_T *mapping, unsigned count, long number) {
	for (unsigned i = 0; i < count; i++) {
		if (mapping[i].src <= number && mapping[i].src + mapping[i].rng >= number) {
			return mapping[i].dst + number - mapping[i].src;
		}
	}

	return number;
}
