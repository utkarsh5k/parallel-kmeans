__kernel void initialize_clusters(__global float *clusters, __global float *rand_max)
{
    int i, j, cols;
    i = get_global_id(0);
    cols = 16;
    int seed = i+1;
    for(j = 0; j < cols; j++)
    {
        clusters[(i*cols) + j] = (rand_max[j] * (seed^7)) / ((seed+1)^13);
    }
}
