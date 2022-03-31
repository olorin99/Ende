
import sys
import matplotlib
import matplotlib.pyplot as plt
matplotlib.use("TkAgg")

def main():
    print("Hello")
    print("Profile file {}", sys.argv[1])

    with open(sys.argv[1], "rt") as f:

        current_frame = 0

        labels = {"": []}
        frames = []

        for line in f:

            components = line.split("@")
            if len(components) < 3:
                current_frame = int(components[0])
                frames.append(current_frame)
                continue

            label = components[0][1:]
            file = components[1]
            line = components[2]
            times = components[3][:-2].split(",")

            total_time = 0
            print(f"label: {label}\nfile: {file}\nline: {line}")
            for time in times:
                total_time += int(time[:-1])
            a = labels.get(label)
            if a:
                a.append(total_time)
            else:
                labels[label] = [total_time]

        for label, times in labels.items():
            if not len(times) == len(frames):
                print(f"label: {label}, times: {len(times)}")
                continue
            plt.plot(frames, times, label=label)

        plt.xlabel("Frames")
        plt.ylabel("Execution Time (ns)")
        plt.legend()
        plt.show()





if __name__ == "__main__":
    main()
