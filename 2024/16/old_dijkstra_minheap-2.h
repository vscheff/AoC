typedef struct COORD_T {
	size_t x;
	size_t y;
} COORD_T;
	       
typedef enum DIR_E {North, East, South, West, None} DIR_E;

typedef struct NODE_T {
	size_t dist;
	struct NODE_T *parent;
	char sym;
	COORD_T pos;
	DIR_E dir;
} NODE_T;

size_t get_parent(size_t i) { return (i - 1) / 2; }

size_t get_left(size_t i) { return 2 * i + 1; }

size_t get_right(size_t i) { return 2 * i + 2; }

void swap(NODE_T **arr, size_t a, size_t b) {
	NODE_T *temp = arr[a];
	arr[a] = arr[b];
	arr[b] = temp;
}

void heapify(NODE_T **arr, size_t arr_len, size_t index) {
	unsigned left = get_left(index), right = get_right(index), smallest = index;

	if (left < arr_len && arr[left]->dist < arr[smallest]->dist)
		smallest = left;
	if (right < arr_len && arr[right]->dist < arr[smallest]->dist)
		smallest = right;
	
	if (smallest != index) {
		swap(arr, index, smallest);
		heapify(arr, arr_len, smallest);
	}
}

void insert(NODE_T ***arr, size_t *arr_len, NODE_T *node) {
	size_t i = *arr_len, j = get_parent(*arr_len);
	*arr = realloc(*arr, ++(*arr_len) * sizeof(**arr));
	(*arr)[i] = node;

	while (i != 0 && (*arr)[i]->dist < (*arr)[j]->dist) {
		swap(*arr, i, j);
		i = j;
		j = get_parent(j);
	}
}

void resort(NODE_T **arr, size_t arr_len) {
	for (size_t i = arr_len / 2 - 1; i < arr_len; i--) heapify(arr, arr_len, i);
}

NODE_T *pop_root(NODE_T ***arr, size_t *arr_len) {
	if (*arr_len == 0) {
		fprintf(stderr, "ERROR: Heap Empty! Nothing to pop.\n");
		return NULL;
	}

	if (*arr_len == 1) {
		*arr_len = 0;
		return **arr;
	}

	NODE_T *root_node = **arr, *tail_node = (*arr)[--(*arr_len)];
	**arr = tail_node;
	heapify(*arr, *arr_len, 0);

	return root_node;
}
