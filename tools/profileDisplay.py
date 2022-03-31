
import sys


def main():
    print("Hello")
    print("Profile file {}", sys.argv[1])

    with open(sys.argv[1], "rt") as f:

        current_frame = 0

        for line in f:
            print(line, end='')

            components = line.split("@")
            if len(components) < 3:
                current_frame = int(components[0])
                continue

            label = components[0][1:]
            file = components[1]
            line = components[2]
            times = components[3][:-2].split(",")

            print(f"label: {label}\nfile: {file}\nline: {line}\n")
            for time in times:
                print(f"time: {time[:-1]}, ", end='')
            print('')





if __name__ == "__main__":
    main()
