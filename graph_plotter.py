import matplotlib.pyplot as plt
import pandas as pd
import os
import sys

colors = ['red', 'green', 'blue', 'yellow']

def make_initial_graph():
    data = pd.read_csv('data/pollution_new_small.csv', header = 0)
    data.rename(columns = {'Unnamed: 0' : 'ID'}, inplace = True)
    X = data['NO2 Mean']
    Y = data['SO2 Mean']
    plt.scatter(X, Y, c = 'black')
    plt.xlabel("NO2 Mean")
    plt.ylabel("SO2 Mean")
    plt.title("Initial Output")
    plt.savefig("Clustering_Images/Initial.png")

def make_MPI_graph():
    data = pd.read_csv('data/pollution_new_small.csv', header = 0)
    data.rename(columns = {'Unnamed: 0' : 'ID'}, inplace = True)
    X = data['NO2 Mean']
    Y = data['SO2 Mean']
    root = "Results/"
    iter_count = 0
    for files in os.listdir(root):
        if files[0:3] == 'MPI':
            file_path = os.path.join(root, files)
            curX, curY = [], []
            cluster_labels = pd.read_csv(file_path, header = 0)
            cluster_labels = cluster_labels['Point']
            for point in cluster_labels:
                curX.append(X[point-1])
                curY.append(Y[point-1])
            plt.scatter(curX, curY, c = colors[iter_count])
            iter_count += 1
    plt.xlabel("NO2 Mean")
    plt.ylabel("SO2 Mean")
    plt.title("MPI Output")
    plt.savefig("Clustering_Images/MPI.png")

def make_sequential_graph():
    data = pd.read_csv('data/pollution_new_small.csv', header = 0)
    data.rename(columns = {'Unnamed: 0' : 'ID'}, inplace = True)
    X = data['NO2 Mean']
    Y = data['SO2 Mean']
    root = "Results/"
    iter_count = 0
    files = 'Sequential_results.csv'
    cluster_output = pd.read_csv('Results/Sequential_results.csv', header = 0)
    for unique_val in cluster_output['Cluster'].unique():
        curX, curY = [], []
        for point, cluster in zip(cluster_output['Point'], cluster_output['Cluster']):
            if cluster == unique_val:
                curX.append(X[point-1])
                curY.append(Y[point-1])
        plt.scatter(curX, curY, c = colors[iter_count])
        iter_count += 1
    plt.xlabel("NO2 Mean")
    plt.ylabel("SO2 Mean")
    plt.title("Sequential Output")
    plt.savefig("Clustering_Images/Sequential.png")

def make_OpenCL_graph():
    data = pd.read_csv('data/pollution_new_small.csv', header = 0)
    data.rename(columns = {'Unnamed: 0' : 'ID'}, inplace = True)
    X = data['NO2 Mean']
    Y = data['SO2 Mean']
    iter_count = 0
    cluster_output = pd.read_csv('Results/OpenCL_results.csv', header = 0)
    for unique_val in cluster_output['Cluster'].unique():
        curX, curY = [], []
        for point, cluster in zip(cluster_output['Point'], cluster_output['Cluster']):
            if cluster == unique_val:
                curX.append(X[point-1])
                curY.append(Y[point-1])
        plt.scatter(curX, curY, c = colors[iter_count])
        iter_count += 1
    plt.xlabel("NO2 Mean")
    plt.ylabel("SO2 Mean")
    plt.title("OpenCL Output")
    plt.savefig("Clustering_Images/OpenCL.png")


#make_initial_graph()
#make_MPI_graph()
#make_sequential_graph()
make_OpenCL_graph()
