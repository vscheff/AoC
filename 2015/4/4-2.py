from hashlib import md5
from sys import argv

def main():
    i = 1
    while(True):
        if md5(bytes(f"{argv[1]}{i}", "utf-8")).hexdigest()[:6] == "000000":
            print(i)
            return

        i += 1


if __name__ == "__main__":
    main()
