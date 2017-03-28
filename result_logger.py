import os
import matplotlib.pyplot as plt

def log_opencl_vs_sequential():
    for cluster in xrange(1, 21):
        command = "./sqkm %d >> Times/sequential_times" % (cluster)
        os.system(command)
        command = "./openclkm %d >> Times/opencl_times" % (cluster)
        os.system(command)

def log_sequential_vs_MPI():
    for i in xrange(1,21):
        command = "mpirun -n 4 mpikm >> Times/mpi_results"
        os.system(command)
        command = "./sqkm 4 >> Times/sequential_times_4"
        os.system(command)

def get_times_as_list(filename):
    result_file = open(filename, "r")
    time_taken = []
    for line in result_file:
        time = float(line)
        time_taken.append(time)
    return time_taken

def make_graphs():
    clusters = [i for i in xrange(1, 21)]
    seq_times = get_times_as_list('Times/sequential_times')
    cl_times = get_times_as_list('Times/opencl_times')
    seq_times_4 = get_times_as_list('Times/sequential_times_4')
    mpi_times = get_times_as_list('Times/mpi_results')
    plt.plot(clusters, seq_times, c = 'red')
    plt.plot(clusters, opencl_times, c = 'blue')
    mpi_times_new = []
    for s, s4, m, i in zip(seq_times, seq_times_4, mpi_times, xrange(1, 21)):
        new_time = (m / s4) * s * (i / 4)
        mpi_times_new.append(new_time)
    plt.plot(clusters, mpi_times_new, c = 'green')
    plt.xlabel("Number of clusters")
    plt.ylabel("Time taken")
    plt.title("Sequential vs MPI vs OpenCL")
    plt.plot(clusters, time_taken)
    plt.savefig("Performance/Comparison.png")
    plt.show()

log_opencl_vs_sequential()
