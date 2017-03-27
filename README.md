# parallel-kmeans
Parallel implementation of k-means clustering in MPI and OpenCL

## Data Description
The dataset being used for this project is [US Pollution Data](https://www.kaggle.com/sogun3/uspollution) obtained from [Kaggle](https://www.kaggle.com).

The original dataset 'pollution_us_2000_2016.csv' contains 17,46,661 samples. Each sample has 28 features:
`['State Code', 'County Code', 'Site Num', 'Address', 'State',
       'County', 'City', 'Date Local', 'NO2 Units', 'NO2 Mean',
       'NO2 1st Max Value', 'NO2 1st Max Hour', 'NO2 AQI', 'O3 Units',
       'O3 Mean', 'O3 1st Max Value', 'O3 1st Max Hour', 'O3 AQI',
       'SO2 Units', 'SO2 Mean', 'SO2 1st Max Value', 'SO2 1st Max Hour',
       'SO2 AQI', 'CO Units', 'CO Mean', 'CO 1st Max Value',
       'CO 1st Max Hour', 'CO AQI']`

'transform_data.py' removes some features and retains the following:
`['NO2 Mean', 'NO2 1st Max Value', 'NO2 1st Max Hour', 'NO2 AQI',
       'O3 Mean', 'O3 1st Max Value', 'O3 1st Max Hour', 'O3 AQI',
       'SO2 Mean', 'SO2 1st Max Value', 'SO2 1st Max Hour',
       'SO2 AQI', 'CO Mean', 'CO 1st Max Value',
       'CO 1st Max Hour', 'CO AQI']`

Then we write 1,00,000 samples of the modified dataframe to the file 'pollution_new_small.csv'.

The data used is for demonstration purposes only. Clustering accuracy is not a concern.

## Running MPI on Linux. (Ubuntu 14.04): 

Set up MPI on Ubuntu 14.04: `sudo apt-get install libcr-dev mpich2 mpich2-doc`

Compiling: `mpic++ -o <executable_name> mpi_kmeans.cpp -std=c++11`

Run: `mpirun -n <num_procs> <executable_name>`


## Performance

### Sequential k-means


![Graph](/Performance Graphs/Sequential.png)

## Checklist

- [x] Sequential k-means
- [ ] Parallel k-means (centre wise)
- [ ] Comparing performance
- [ ] Graphs
