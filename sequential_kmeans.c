#include <stdio.h>
#include <stdlib.h>

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


int main()
{
    //implement later
}
