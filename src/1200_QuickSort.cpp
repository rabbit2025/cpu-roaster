#include <stdio.h>
#include <iostream>
#include "util.h"
#include "Tracer.h"
 
#define N (2*1024*1024)
#define ITER_COUNT 2
//#define N 480
//#define ITER_COUNT 4

using namespace std;

// A utility function to swap two elements
template<typename T>
void swap(T* a, T* b)
{
    T t = *a;
    *a = *b;
    *b = t;
}
 
/* This function takes last element as pivot, places
   the pivot element at its correct position in sorted
    array, and places all smaller (smaller than pivot)
   to left of pivot and all greater elements to right
   of pivot */
template<typename T>
int partition (T arr[], int low, int high)
{
    T pivot = arr[high];    // pivot
    int i = (low - 1);  // Index of smaller element
 
    for (int j = low; j <= high- 1; j++)
    {
        // If current element is smaller than or
        // equal to pivot
        if (arr[j] <= pivot)
        {
            i++;    // increment index of smaller element
            swap(&arr[i], &arr[j]);
            if(arr[j].a%1000==0) Tracer::I()->meet(arr[j].v());
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}
 
/* The main function that implements QuickSort
 arr[] --> Array to be sorted,
  low  --> Starting index,
  high  --> Ending index */
template<typename T>
void quickSort(T arr[], int low, int high)
{
    if (low < high)
    {
        /* pi is partitioning index, arr[p] is now
           at right place */
        int pi = partition(arr, low, high);
 
        // Separately sort elements before
        // partition and after partition
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}
 
///* Function to print an array */
//void printArray(int arr[], int size)
//{
//    int i;
//    for (i=0; i < size; i++)
//        printf("%d ", arr[i]);
//    printf("n");
//}
// 
////// Driver program to test above functions
////int main()
//{
//    int arr[] = {10, 7, 8, 9, 1, 5};
//    int n = sizeof(arr)/sizeof(arr[0]);
//    quickSort<int>(arr, 0, n-1);
//    printf("Sorted array: n");
//    printArray(arr, n);
//    return 0;
//}
//
void run_QuickSort(uint8_t* seedIn, int seedSize) {
	Pair* arr=new Pair[N];
	PairA* arrA=(PairA*)arr;
	PairB* arrB=(PairB*)arr;
	fillPairArray(seedIn, seedSize, arr, N);
	//for(int j=0; j<N; j++) printf("init %d: %016llx %016llx\n",j,arr[j].a,arr[j].b);
	for(int i=0; i<ITER_COUNT; i++) {
		quickSort<PairA>(arrA, 0, N-1);
		mixArray<PairA>(arrA, N-1);
		//for(int j=0; j<N; j++) printf("%d-%d: %016llx %016llx\n",i,j,arr[j].a,arr[j].b);
		quickSort<PairB>(arrB, 0, N-1);
		mixArray<PairB>(arrB, N-1);
		//for(int j=0; j<N; j++) printf("%d-%d: %016llx %016llx\n",i,j,arr[j].a,arr[j].b);
	}
	Tracer::I()->sha3_update((unsigned char*)arr, 1024);
	delete[] arr;
}

#undef N
#undef ITER_COUNT

#ifdef SELF_TEST
int main() {
	char hello[100]="aer39invqbj43to;5j46354q34534999!@#%@#$%^&$&ADGSGWREF";
	int len=strlen(hello);
	for(int i=0; i<50; i++) {
		run_QuickSort((uint8_t*)hello,len);
	}
	return 0;
}
#endif
