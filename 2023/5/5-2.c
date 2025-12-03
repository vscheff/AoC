#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	uint64_t dst;
	uint64_t src;
	uint64_t rng;
} MAP_T;

MAP_T *read_map(FILE *, unsigned *); 
uint64_t get_num(MAP_T *, unsigned, uint64_t);

int main(int argc, char **argv) {
	FILE *infile = fopen(argv[1], "r");

	char *line = NULL;
	size_t n = 0;

	getline(&line, &n, infile);

	char *str_ptr = line;

	strsep(&str_ptr, ":");
	str_ptr++;

	char *num_ptr = NULL;
	uint64_t *seeds = NULL;
	unsigned seed_cnt = 0;

	while (str_ptr) {
		num_ptr = strsep(&str_ptr, " ");
		seeds = realloc(seeds, ++seed_cnt * sizeof(*seeds));
		seeds[seed_cnt - 1] = strtoul(num_ptr, NULL, 10);
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
	
	uint64_t number, smallest = UINT_MAX;

	for (unsigned i = 0; i < seed_cnt; i += 2) {
		printf("Starting seed %lu with range %lu\n", seeds[i], seeds[i+1]);	
		for (uint64_t j = seeds[i]; j < seeds[i] + seeds[i + 1]; j++) {
			number = get_num(seed_to_soil, s_to_s_cnt, j);
			number = get_num(soil_to_fertilizer, s_to_f_cnt, number);
			number = get_num(fertilizer_to_water, f_to_w_cnt, number);
			number = get_num(water_to_light, w_to_l_cnt, number);
			number = get_num(light_to_temperature, l_to_t_cnt, number);
			number = get_num(temperature_to_humidity, t_to_h_cnt, number);
			number = get_num(humidity_to_location, h_to_l_cnt, number);

			if (number < smallest) {
				printf("\tFound new smallest: %lu\n", number);
				smallest = number;
			}
		}
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
		mapping[map_count].dst = strtoul(num_ptr, NULL, 10);
		num_ptr = strsep(&str_ptr, " ");
		mapping[map_count].src = strtoul(num_ptr, NULL, 10);
		mapping[map_count].rng = strtoul(str_ptr, NULL, 10);
		map_count++;
	}

	free(line);

	*count = map_count;
	return mapping;
}

uint64_t get_num(MAP_T *mapping, unsigned count, uint64_t number) {
	for (unsigned i = 0; i < count; i++) {
		if (mapping[i].src <= number && mapping[i].src + mapping[i].rng > number) {
			return mapping[i].dst + number - mapping[i].src;
		}
	}

	return number;
}
