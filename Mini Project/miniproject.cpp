#include <mpi.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

void quickSort(int arr[], int left, int right) {
    int i = left, j = right;
    int tmp;
    int pivot = arr[(left + right) / 2];

    /* partition */
    while (i <= j) {
        while (arr[i] < pivot)
            i++;
        while (arr[j] > pivot)
            j--;
        if (i <= j) {
            tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;
            i++;
            j--;
        }
    };

    /* recursion */
    if (left < j)
        quickSort(arr, left, j);
    if (i < right)
        quickSort(arr, i, right);
}

int main(int argc, char** argv) {
    int rank, size, n = 1000000;
    int* arr;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Divide the array into subarrays
    int subsize = n / size;
    int* subarr = new int[subsize];
    if (rank == 0) {
        arr = new int[n];
        srand(time(NULL));
        for (int i = 0; i < n; i++) {
            arr[i] = rand() % n;
        }
    }

    // Scatter the subarrays to all processes
    MPI_Scatter(arr, subsize, MPI_INT, subarr, subsize, MPI_INT, 0, MPI_COMM_WORLD);

    // Sort the subarrays
    quickSort(subarr, 0, subsize - 1);

    // Gather the sorted subarrays
    MPI_Gather(subarr, subsize, MPI_INT, arr, subsize, MPI_INT, 0, MPI_COMM_WORLD);

    // Merge the sorted subarrays
    if (rank == 0) {
        int* tmp = new int[n];
        for (int i = 0; i < n; i++) {
            tmp[i] = arr[i];
        }
        quickSort(tmp, 0, n - 1);
        delete[] arr;
        delete[] tmp;
    }

    MPI_Finalize();
    return 0;
}
