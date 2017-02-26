#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <time.h>
using namespace std;

float euclidean_distance(float point[], float centre[], int d)
{
    int i;
    float distance = 0;
    for(i=1; i<d; i++)
    {
        distance = distance + ((point[i] - centre[i-1]) * (point[i] - centre[i-1]));
    }
    return distance/d;
}

int assign_clusters(float distances[], int k)
{
    float min;
    min = distances[0];
    int i, cluster = 0;
    for(i=1; i<k; i++)
    {
        if(min > distances[i])
        {
            cluster = i;
            min = distances[i];
        }
    }
    return cluster;
}

float** init_float_matrix(int rows, int cols)
{
    float **mat;
    mat = new float*[rows];
    for(int i = 0; i<rows; i++)
    {
        mat[i] = new float[cols];
    }
    return mat;
}

float** csv_to_float_matrix(char file_name[])
{
    ifstream csv_file;
    csv_file.open("data/pollution_new_small.csv", ios::in);
    char str[210];
    int start = 0, end, row_num = 0, col_num, first= 1;
    string row_values, substr, delim;
    float** feature_matrix = init_float_matrix(100000, 17);
    while(csv_file.getline(str, 210))
    {
        if(first)
        {
            first = 0;
            continue;
        }
        row_values = str;
        delim = ",";
        start = 0;
        end = row_values.find(delim);
        col_num = 0;
        while(end != string::npos)
        {
            substr =  row_values.substr(start, end - start);
            feature_matrix[row_num][col_num++] = stof(substr);
            start = end + delim.length();
            end = row_values.find(delim, start);
        }
        feature_matrix[row_num++][col_num++] = stof(row_values.substr(start, end));
    }
    return feature_matrix;
}

float* max_values(float **feauture_matrix, int nsamples, int dims)
{
    //Returns a float[dims-1] array containing maximum dimension value in all the samples.
    float *max_vals = new float[dims-1];
    float max;
    for(int j = 1; j < dims; j++)
    {
        max = 0.0;
        for(int i = 0; i < nsamples; i++)
        {
            if(max < feauture_matrix[i][j])
            {
                max = feauture_matrix[i][j];
            }
        }
        max_vals[j-1] = max;
    }
    return max_vals;
}

float** init_clusters(float *rand_max, int num_clusters, int dims)
{
    float **clusters = init_float_matrix(num_clusters, dims-1);
    for(int i = 0; i < num_clusters; i++)
    {
        for(int j = 0; j < dims-1; j++)
        {
            clusters[i][j] = rand_max[j] >= 1.0 ? (float) (rand() % (int) rand_max[j]) : 0.0;
        }
    }
    return clusters;
}

void kmeans_clustering(float **dataframe, float **clusters, int nsamples, int num_clusters, int dims)
{
    int changed = 1, i, j, k, cluster, iter = 1;
    while(changed)
    {
        cout << "Iteration: " << iter++ << endl;
        float distances[num_clusters], point_summation[num_clusters][dims];
        for(i = 0; i < num_clusters; i++)
            for(j = 0; j < dims; j++)
                point_summation[i][j] = 0;
        for(int i = 0; i < nsamples; i++)
        {
            for(j = 0; j < num_clusters; j++)
            {
                distances[j] = euclidean_distance(dataframe[i], clusters[j], dims);
            }
            cluster = assign_clusters(distances, num_clusters);
            dataframe[i][0] = cluster;
            point_summation[cluster][0]++;
            for(j = 1; j < dims; j++)
                point_summation[cluster][j] += dataframe[i][j];
        }
        changed = 0;
        for(i = 0; i < num_clusters; i++)
        {
            for(j = 1; j < dims; j++)
            {
                float new_value = point_summation[i][0] != 0?  point_summation[i][j] / point_summation[i][0]: 100;
                if(new_value != clusters[i][j-1])
                    changed = 1;
                clusters[i][j-1] = new_value;
            }
            //cout<< point_summation[i][0]<<endl;
        }

    }
}

int main()
{
    char file_name[] = "data/pollution_new_small.csv";
    float **dataframe = csv_to_float_matrix(file_name);
    int nsamples = 100000, dims = 17, num_clusters = 100;
    double run_time = clock();
    float **cluster_centres = init_clusters(max_values(dataframe, nsamples, dims), num_clusters, dims);
    kmeans_clustering(dataframe, cluster_centres, nsamples, num_clusters, dims);
    run_time = (clock() - run_time) / CLOCKS_PER_SEC;
    cout<< "Total run time: "<< run_time<<endl;
    return 0;
}
