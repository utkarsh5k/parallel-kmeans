#include<OpenCL/opencl.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <time.h>
using namespace std;

void write_results_to_file(float *dataframe, int nsamples)
{
    ofstream result_file;
    result_file.open("Results/OpenCL_results.csv");
    int i;
    result_file << "Point,Cluster\n";
    for(i = 0; i < nsamples; i++)
    {
        result_file<<i+1<<","<<dataframe[i*17]+1<<"\n";
    }
}

float* csv_to_float_matrix()
{
    ifstream csv_file;
    csv_file.open("data/pollution_new_small.csv", ios::in);
    char str[210];
    int start = 0, end, row_num = 0, col_num, first= 1;
    string row_values, substr, delim;
    int nsamples = 10000, dims = 17;
    float* feature_matrix = new float[nsamples * dims];
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
            feature_matrix[(row_num*dims) + col_num] = stof(substr);
            col_num++;
            start = end + delim.length();
            end = row_values.find(delim, start);
        }
        feature_matrix[(row_num*dims) + col_num] = stof(row_values.substr(start, end));
        row_num++;
        col_num++;
    }
    return feature_matrix;
}

float* max_values(float *feauture_matrix, int nsamples, int dims)
{
    //Returns a float[dims-1] array containing maximum dimension value in all the samples.
    float *max_vals = new float[dims-1];
    float max;
    for(int j = 1; j < dims; j++)
    {
        max = 0.0;
        for(int i = 0; i < nsamples; i++)
        {
            if(max < feauture_matrix[(i*dims) + j])
            {
                max = feauture_matrix[(i*dims) + j];
            }
        }
        max_vals[j-1] = max;
    }
    return max_vals;
}

void assign_clusters(float *dataframe, float *distances, int nsamples, int nclusters, int dims)
{
    int i, j, min, ind;
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
        dataframe[(i*dims)] = j;
    }
}

int main(int argc, char *argv[])
{
    int nsamples = 10000, dims = 17;
	float *dataframe = csv_to_float_matrix();
    int num_clusters = stof(argv[1]);
    float *rand_max = max_values(dataframe, nsamples, dims);
    float *clusters = new float[num_clusters * (dims-1)];
    int changed_arr[num_clusters];
    FILE *f;
	char *src_str;
	size_t s_size;

	f = fopen("init_cluster.cl", "r");
	if(f == NULL)
	{
		fprintf(stdout, "File not found: ");
		fflush(stdout);
		exit(0);
	}
	src_str = (char *)malloc(100000);
	s_size = fread(src_str, sizeof(char), 100000, f);
	fclose(f);

	cl_platform_id platform_id = NULL;
	cl_device_id device_id = NULL;
	cl_uint ret_num_devices, ret_num_platform;

	cl_int status = clGetPlatformIDs(1, &platform_id, &ret_num_platform);
	status = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, &ret_num_devices);
	cl_context context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &status);
	cl_command_queue cq = clCreateCommandQueue(context, device_id, NULL, &status);

    cl_mem a_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE, nsamples * dims * sizeof(float), NULL, &status);
	cl_mem b_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE, num_clusters * (dims-1) * sizeof(float), NULL, &status);
	cl_mem c_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE, (dims-1) * sizeof(float), NULL, &status);
    cl_mem d_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE, nsamples * num_clusters * sizeof(float), NULL, &status);
    cl_mem e_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE, num_clusters * sizeof(int), NULL, &status);

    status = clEnqueueWriteBuffer(cq, a_mem_obj, CL_TRUE, 0, nsamples * dims * sizeof(float), dataframe, 0, NULL, NULL);
    status = clEnqueueWriteBuffer(cq, c_mem_obj, CL_TRUE, 0, (dims-1) * sizeof(float), rand_max, 0, NULL, NULL);

	cl_program program = clCreateProgramWithSource(context, 1, (const char **)&src_str, (const size_t *)&s_size, &status);
    status = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
    cl_kernel kernel = clCreateKernel(program, "initialize_clusters", &status);
	size_t global_item_size = num_clusters;
	size_t local_item_size = 1;

	status = clSetKernelArg(kernel, 0, sizeof(cl_mem), &b_mem_obj);
	status = clSetKernelArg(kernel, 1, sizeof(cl_mem), &c_mem_obj);
	status = clEnqueueNDRangeKernel(cq, kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL, NULL);
	status = clFinish(cq);
	status = clEnqueueReadBuffer(cq, b_mem_obj, CL_TRUE, 0, num_clusters * (dims-1) * sizeof(float), clusters, 0, NULL, NULL);

    f = fopen("calc_dist.cl", "r");
	if(f == NULL)
	{
		fprintf(stdout, "File not found: ");
		fflush(stdout);
		exit(0);
	}
	src_str = (char *)malloc(100000);
	s_size = fread(src_str, sizeof(char), 100000, f);
	fclose(f);

    program = clCreateProgramWithSource(context, 1, (const char **)&src_str, (const size_t *)&s_size, &status);
    status = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
    cl_kernel kernel1 = clCreateKernel(program, "calculate_distance", &status);

    f = fopen("new_cluster.cl", "r");
	if(f == NULL)
	{
		fprintf(stdout, "File not found: ");
		fflush(stdout);
		exit(0);
	}
	src_str = (char *)malloc(100000);
	s_size = fread(src_str, sizeof(char), 100000, f);
	fclose(f);

    program = clCreateProgramWithSource(context, 1, (const char **)&src_str, (const size_t *)&s_size, &status);
    status = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
    cl_kernel kernel2 = clCreateKernel(program, "get_new_cluster", &status);

    int changed = 1;
    double run_time = clock();
    while(changed)
    {
        status = clSetKernelArg(kernel1, 0, sizeof(cl_mem), &a_mem_obj);
    	status = clSetKernelArg(kernel1, 1, sizeof(cl_mem), &b_mem_obj);
        status = clSetKernelArg(kernel1, 2, sizeof(cl_mem), &d_mem_obj);

        status = clEnqueueNDRangeKernel(cq, kernel1, 1, NULL, &global_item_size, &local_item_size, 0, NULL, NULL);
    	status = clFinish(cq);

        status = clSetKernelArg(kernel2, 0, sizeof(cl_mem), &a_mem_obj);
    	status = clSetKernelArg(kernel2, 1, sizeof(cl_mem), &d_mem_obj);
        status = clSetKernelArg(kernel2, 2, sizeof(cl_mem), &b_mem_obj);
        status = clSetKernelArg(kernel2, 3, sizeof(cl_mem), &e_mem_obj);

        status = clEnqueueNDRangeKernel(cq, kernel2, 1, NULL, &global_item_size, &local_item_size, 0, NULL, NULL);
    	status = clFinish(cq);

        status = clEnqueueReadBuffer(cq, e_mem_obj, CL_TRUE, 0, num_clusters * sizeof(int), changed_arr, 0, NULL, NULL);
        int i;
        changed = 0;
        for(i = 0; i < num_clusters; i++)
        {
            if(changed_arr[i])
            {
                changed = 1;
                break;
            }
        }
    }
    run_time = (clock() - run_time) / CLOCKS_PER_SEC;
    cout << run_time << endl;
    status = clEnqueueReadBuffer(cq, a_mem_obj, CL_TRUE, 0, nsamples * dims * sizeof(float), dataframe, 0, NULL, NULL);
    write_results_to_file(dataframe, nsamples);
	status = clFlush(cq);
	status = clReleaseKernel(kernel);
    status = clReleaseKernel(kernel1);
    status = clReleaseKernel(kernel2);
	status = clReleaseProgram(program);
	status = clReleaseMemObject(a_mem_obj);
	status = clReleaseMemObject(b_mem_obj);
	status = clReleaseMemObject(c_mem_obj);
    status = clReleaseMemObject(d_mem_obj);
    status = clReleaseMemObject(e_mem_obj);
	status = clReleaseCommandQueue(cq);
	status = clReleaseContext(context);

	return 0;
}
