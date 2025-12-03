typedef struct COORD_T {
	size_t x;
	size_t y;
} COORD_T;
	       
typedef enum DIR_E {North, East, South, West} DIR_E;

typedef struct PATH_T { 
	COORD_T *pos;
	size_t len;
	DIR_E dir;
	size_t cost;
} PATH_T;

typedef struct CHECK_T {
	COORD_T pos;
	DIR_E dir;
} CHECK_T;

size_t get_parent(size_t i) { return (i - 1) / 2; }

size_t get_left(size_t i) { return 2 * i + 1; }

size_t get_right(size_t i) { return 2 * i + 2; }

void swap(PATH_T *arr, size_t a, size_t b) {
	PATH_T temp = arr[a];
	arr[a] = arr[b];
	arr[b] = temp;
}

void heapify(PATH_T *arr, size_t arr_len, size_t index) {
	unsigned left = get_left(index), right = get_right(index), smallest = index;

	if (left < arr_len && arr[left].cost < arr[smallest].cost)
		smallest = left;
	if (right < arr_len && arr[right].cost < arr[smallest].cost)
		smallest = right;
	
	if (smallest != index) {
		swap(arr, index, smallest);
		heapify(arr, arr_len, smallest);
	}
}

void insert(PATH_T **arr, size_t *arr_len, size_t x, size_t y, DIR_E dir, size_t cost, PATH_T old_node) {
	size_t i = *arr_len, j = get_parent(*arr_len);
	*arr = realloc(*arr, ++(*arr_len) * sizeof(**arr));
	PATH_T new_node = { malloc((old_node.len + 1) * sizeof(*old_node.pos)), old_node.len + 1, dir, cost};
	memcpy(new_node.pos, old_node.pos, old_node.len * sizeof(*new_node.pos));
	new_node.pos[old_node.len].x = x;
	new_node.pos[old_node.len].y = y;
	(*arr)[*arr_len - 1] = new_node;

	while (i != 0 && (*arr)[i].cost < (*arr)[j].cost) {
		swap(*arr, i, j);
		i = j;
		j = get_parent(j);
	}
}

PATH_T pop_root(PATH_T **arr, size_t *arr_len) {
	if (*arr_len == 0) {
		fprintf(stderr, "ERROR: Heap Empty! Nothing to pop.\n");
		PATH_T bad_node = { NULL, 0, 0, 0};
		return bad_node;
	}

	if (*arr_len == 1) {
		*arr_len = 0;
		return **arr;
	}

	PATH_T root_node = **arr, tail_node = (*arr)[--(*arr_len)];
	**arr = tail_node;
	heapify(*arr, *arr_len, 0);

	return root_node;
}
