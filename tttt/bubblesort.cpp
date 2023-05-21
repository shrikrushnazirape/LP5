#include <iostream>
#include <cstdlib>
#include <ctime>
#include <omp.h>

using namespace std;

void bubble_sort(int* arr, int n)
{
    for(int i=0; i<n; i++)
    {
        bool swapped = false;
        for(int j=0; j<n-i-1; j++)
        {
            if(arr[j] > arr[j+1])
            {
                swap(arr[j], arr[j+1]);
                swapped = true;
            }
        }
        if(!swapped) break;
    }
}

void parallel_bubble_sort(int* arr, int n)
{
    for(int i=0; i<n; i++)
    {
        bool swapped = false;
        #pragma omp parallel for shared(arr,swapped)
        for(int j=0; j<n-i-1; j++)
        {
            if(arr[j] > arr[j+1])
            {
                swap(arr[j], arr[j+1]);
                swapped = true;
            }
        }
        if(!swapped) break;
    }
}

int main()
{
    srand(time(0));

    const int N = 1000;
    int arr[N];
    for(int i=0; i<N; i++)
        arr[i] = rand()%N;

    int seq_start = clock();
    bubble_sort(arr, N);
    int seq_end = clock();

    int par_start = clock();
    parallel_bubble_sort(arr, N);
    int par_end = clock();

    double seq_time = (seq_end - seq_start) / (double) CLOCKS_PER_SEC;
    double par_time = (par_end - par_start) / (double) CLOCKS_PER_SEC;

    cout << "Sequential Time: " << seq_time << " seconds" << endl;
    cout << "Parallel Time: " << par_time << " seconds" << endl;

    double speedup = seq_time / par_time;
    double efficiency = speedup / omp_get_max_threads();
    double throughput = N / par_time;

    cout << "Speedup: " << speedup << endl;
    cout << "Efficiency: " << efficiency << endl;
    cout << "Throughput: " << throughput << endl;

    return 0;

}



/*

to run : 
g++ -fopenmp bubblesort.cpp -o bubblesort
./bubblesort



output : 

Sequential Time: 0.003 seconds
Parallel Time: 0.001 seconds
Speedup: 3
Efficiency: 0.375
Throughput: 1e+06


*/
