__kernel void get_new_cluster(__global float *dataframe, __global float *distances, __global float *clusters, __global int *changed)
{
    int i, j, min, ind, id, nclusters, count = 0, k;
    int nsamples = 10000, dims = 17;
    id = get_global_id(0);
    float new_cluster[16];
    for(i = 0; i < dims - 1; i++)
    {
        new_cluster[i] = 0;
    }
    nclusters = get_global_size(0);
    for(i = 0; i < nsamples; i++)
    {
        min = distances[(i*nclusters)];
        ind = 0;
        for(j = 0; j < nclusters; j++)
        {
            if(min > distances[(i*nclusters) + j])
            {
                min = distances[(i*nclusters) + j];
                ind = j;
            }
        }
        if(ind == id)
        {
            dataframe[(i*dims)] = ind;
            for(k = 1; k < dims; k++)
            {
                new_cluster[k-1] += dataframe[(i*dims) + k];
            }
            count++;
        }
    }
    int flag = 0;
    for(i = 0; i < dims - 1; i++)
    {
        new_cluster[i] /= count;
        if(new_cluster[i] != clusters[(id*(dims-1)) + i])
        {
            flag = 1;
        }
        clusters[(id*(dims-1)) + i] = new_cluster[i];
    }
    changed[id] = flag;
}
