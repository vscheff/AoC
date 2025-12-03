from re import findall
from sys import argv


def main():
    with open(argv[1], "r") as in_file:
        memory = in_file.read()

    total = 0;

    for match in findall(r"mul\(\d+,\d+\)", memory):
        x, y = (int(i) for i in match.strip("mul()").split(","))

        total += x * y

    print(f"Total = {total}")


if __name__ == "__main__":
    main()
