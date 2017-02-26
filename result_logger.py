import os
import matplotlib.pyplot as plt

def log_sequential_results():
    for cluster in xrange(1, 21):
        command = "./a.out %d >> results" % (cluster)
        os.system(command)

def make_graphs():
    result_file = open("results", "r")
    clusters = []
    time_taken = []
    num_cluster = 1
    for line in result_file:
        time = float(line)
        clusters.append(num_cluster)
        time_taken.append(time)
        num_cluster += 1
    plt.xlabel("Number of clusters")
    plt.ylabel("Time taken")
    plt.title("Number of clusters vs Time taken: Sequential k-means")
    plt.plot(clusters, time_taken)
    plt.savefig("Performance Graphs/Sequential.png")
    plt.show()

log_sequential_results()
make_graphs()
