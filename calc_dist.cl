__kernel void calculate_distance(__global float *dataframe, __global float *clusters, __global float *distances)
{
    int i = get_global_id(0);
    int nclusters = get_global_size(0);
    int j, k;
    int cols = 17;
    int nsamples = 10000;
    for(j = 0; j < nsamples; j++)
    {
        float distance = 0;
        for(k = 1; k < cols; k++)
        {
            distance = distance + ((dataframe[(j*cols) + k] - clusters[(i*(cols-1)) + k - 1]) * (dataframe[(j*cols) + k] - clusters[(i*(cols-1)) + k - 1]));
        }
        distances[(j*nclusters) + i] = distance/cols;
    }
}
