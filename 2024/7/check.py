def find_expression(value, arr, index, current_val, expression):
    if index == len(arr):
        if current_val == value:
            print(expression)
        return

    current_num = arr[index]

    find_expression(value, arr, index + 1, current_val + current_num, f"{expression} + {current_num}")
    find_expression(value, arr, index + 1, current_val * current_num, f"{expression} * {current_num}")


value = 9792171027
array = [831, 6, 918, 9, 6, 708, 7, 3, 6]
find_expression(value, array, 1, array[0], str(array[0]))

value = 30308213418
array = [6, 7, 6, 615, 8, 2, 97, 7, 8, 3, 7, 6]
find_expression(value, array, 1, array[0], str(array[0]))

value = 45120753216
array = [4, 2, 281, 6, 6, 6, 44, 3, 919, 6]
find_expression(value, array, 1, array[0], str(array[0]))

value = 383943416264
array = [444, 6, 411, 769, 456, 8]
find_expression(value, array, 1, array[0], str(array[0]))

value = 71106222600
array = [630, 269, 1, 7, 740, 9, 3, 3]
find_expression(value, array, 1, array[0], str(array[0]))
