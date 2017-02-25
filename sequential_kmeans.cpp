#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
using namespace std;

float euclidean_distance(float point[], float centre[], int d)
{
    int i;
    float distance = 0;
    for(i=0; i<d; i++)
    {
        distance = distance + ((point[i] - centre[i]) * (point[i] - centre[i]));
    }
    return distance;
}

int check_cluster(float distances[], int k)
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
        mat[i] = new float[17];
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

int main()
{
    char file_name[] = "data/pollution_new_small.csv";
    float **dataframe = csv_to_float_matrix(file_name);
    return 0;
}
