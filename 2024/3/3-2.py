from re import findall
from sys import argv


def main():
    with open(argv[1], "r") as in_file:
        memory = in_file.read()

    total = 0;
    do = True

    for match in findall(r"mul\(\d+,\d+\)|do\(\)|don't\(\)", memory):
        if match == "do()":
            do = True
        elif match == "don't()":
            do = False
        elif do:
            x, y = (int(i) for i in match.strip("mul()").split(","))

            total += x * y

    print(f"Total = {total}")


if __name__ == "__main__":
    main()
