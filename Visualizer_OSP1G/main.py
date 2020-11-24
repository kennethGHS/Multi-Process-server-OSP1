import sys
import matplotlib.pyplot as plt


if __name__ == '__main__':
    if len(sys.argv) != 4:
        print("Incorrect number of arguments. Usage is: python main.py <Serial file 1> <Continuous file 2> <Delegation file 3>")
        exit(1)

    request_times1 = []
    with open(sys.argv[1]) as file:
        for line in file:
            request_times1.append(float(line))

    request_times2 = []
    with open(sys.argv[2]) as file:
        for line in file:
            request_times2.append(float(line))

    request_times3 = []
    with open(sys.argv[3]) as file:
        for line in file:
            request_times3.append(float(line))

    # Accumulative and average request time
    accumulative1 = []
    average1 = []
    request_per_second1 = []
    total = 0
    counter = 1
    for time in request_times1:
        total += time
        accumulative1.append(total)
        average1.append(total / counter)
        counter += 1
        request_per_second1.append(counter / total)

    accumulative2 = []
    average2 = []
    request_per_second2 = []
    total = 0
    counter = 1
    for time in request_times2:
        total += time
        accumulative2.append(total)
        average2.append(total / counter)
        counter += 1
        request_per_second2.append(counter / total)

    accumulative3 = []
    average3 = []
    request_per_second3 = []
    total = 0
    counter = 1
    for time in request_times3:
        total += time
        accumulative3.append(total)
        average3.append(total / counter)
        counter += 1
        request_per_second3.append(counter / total)


    fig, (ax1, ax2, ax3) = plt.subplots(1, 3, figsize=(20, 5))

    ax1.plot(accumulative1, color='r', marker='^', label='Serial')
    ax1.plot(accumulative2, color='g', marker='o', label='Continuous')
    ax1.plot(accumulative3, color='b', marker='s', label='Delegation')
    ax1.set(xlabel="Cantidad de solicitudes")
    ax1.set(ylabel="Tiempo en ejecutar todas")

    ax2.plot(average1, color='r', marker='^')
    ax2.plot(average2, color='g', marker='o')
    ax2.plot(average3, color='b', marker='s')
    ax2.set(xlabel="Cantidad de solicitudes")
    ax2.set(ylabel="Tiempo promedio")

    ax3.plot(request_per_second1, color='r', marker='^')
    ax3.plot(request_per_second2, color='g', marker='o')
    ax3.plot(request_per_second3, color='b', marker='s')
    ax3.set(xlabel="Tiempo (s)")
    ax3.set(ylabel="Cantidad de solicitudes")

    handles, labels = ax1.get_legend_handles_labels()
    fig.legend(handles, labels, loc='upper center', ncol=len(labels))

    plt.show()

