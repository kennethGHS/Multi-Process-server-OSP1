import sys
import matplotlib.pyplot as plt
import math


def extract_info(filename):
    cumulative_times = []
    request_times = []
    with open(filename) as file:
        for line in file:
            values = line.split()
            request_times.append(float(values[0]))
            cumulative_times.append(float(values[1]))
    return cumulative_times, request_times


def calculate_metrics(cumulative_times, request_times):
    average = []
    total = 0
    counter = 1
    for time in request_times:
        total += time
        average.append(total / counter)
        counter += 1

    max_value = math.ceil(cumulative_times[-1])
    request_per_second = [0] * max_value
    for time in cumulative_times:
        request_per_second[math.floor(time)] += 1

    return average, request_per_second


if __name__ == '__main__':
    if len(sys.argv) != 4:
        print(
            "Incorrect number of arguments. Usage is: python main.py <Sequential file 1> <Heavy process file 2> <Pre heavy process file 3>")
        exit(1)

    # Read files
    cumulative_times1, request_times1 = extract_info(sys.argv[1])
    cumulative_times2, request_times2 = extract_info(sys.argv[2])
    cumulative_times3, request_times3 = extract_info(sys.argv[3])

    # Calculate metrics from times
    average1, request_per_seconds1 = calculate_metrics(cumulative_times1, request_times1)
    average2, request_per_seconds2 = calculate_metrics(cumulative_times2, request_times2)
    average3, request_per_seconds3 = calculate_metrics(cumulative_times3, request_times3)

    print(request_per_seconds1)

    # Plot the data
    fig, (ax1, ax2, ax3) = plt.subplots(1, 3, figsize=(15, 4))

    ax1.plot(cumulative_times1, color='r', label='Sequential')
    ax1.plot(cumulative_times2, color='g', label='Heavy Process')
    ax1.plot(cumulative_times3, color='b', label='Pre Heavy Process')
    ax1.set(xlabel="Cantidad de solicitudes")
    ax1.set(ylabel="Tiempo en ejecutar todas")

    ax2.plot(average1, color='r')
    ax2.plot(average2, color='g')
    ax2.plot(average3, color='b')
    ax2.set(xlabel="Cantidad de solicitudes")
    ax2.set(ylabel="Tiempo promedio")

    ax3.plot(request_per_seconds1, color='r')
    ax3.plot(request_per_seconds2, color='g')
    ax3.plot(request_per_seconds3, color='b')
    ax3.set(xlabel="Tiempo (s)")
    ax3.set(ylabel="Cantidad de solicitudes")

    handles, labels = ax1.get_legend_handles_labels()
    fig.legend(handles, labels, loc='upper center', ncol=len(labels))

    plt.show()
