#include<iostream>
#include<climits>
using namespace std;
#include "util.h"
#include "Tracer.h"

#define N (100*1024)
#define ITER_COUNT 44

int partition(int arr[], int l, int r);
 
// This function returns k'th smallest element in arr[l..r] using
// QuickSort based method.  ASSUMPTION: ALL ELEMENTS IN ARR[] ARE DISTINCT
int kthSmallest(int arr[], int l, int r, int k)
{
    // If k is smaller than number of elements in array
    if (k > 0 && k <= r - l + 1)
    {
        // Partition the array around last element and get
        // position of pivot element in sorted array
        int pos = partition(arr, l, r);
 
        // If position is same as k
        if (pos-l == k-1)
            return arr[pos];
        if (pos-l > k-1)  // If position is more, recur for left subarray
            return kthSmallest(arr, l, pos-1, k);
 
        // Else recur for right subarray
        return kthSmallest(arr, pos+1, r, k-pos+l-1);
    }
 
    // If k is more than number of elements in array
    return INT_MAX;
}
 
void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}
 
// Standard partition process of QuickSort().  It considers the last
// element as pivot and moves all smaller element to left of it
// and greater elements to right
int partition(int arr[], int l, int r)
{
    int x = arr[r], i = l;
    for (int j = l; j <= r - 1; j++)
    {
        if (arr[j] <= x)
        {
            swap(&arr[i], &arr[j]);
            i++;
        }
    }
    //if(arr[r]%1000==0) Tracer::I()->meet(arr[r]);
    swap(&arr[i], &arr[r]);
    return i;
}
 
//// Driver program to test above methods
//int main()
//{
//    int arr[] = {12, 3, 5, 7, 4, 19, 26};
//    int n = sizeof(arr)/sizeof(arr[0]), k = 3;
//    cout << "K'th smallest element is " << kthSmallest(arr, 0, n-1, k);
//    return 0;
//}

void run_KthSmallest(uint8_t* seedIn, int seedSize) {
	Pair* arr=new Pair[N/2+ITER_COUNT];
	int32_t* iarr=(int32_t*)arr;
	fillPairArray(seedIn, seedSize, arr, N/2+ITER_COUNT);
	Pair* startend = arr + N/2;

	for(int i=0; i<ITER_COUNT; i++) {
		int32_t start=startend[i].a%N;
		int32_t end=startend[i].b%N;
		if(start>end) {
			int32_t temp;
			temp=start; start=end; end=temp;
		}
		int32_t k=(startend[i].a>>16)%1024;
		if(k<(end-start)) k%=(end-start);
		int result=kthSmallest(iarr, start, end-1, k);
		Tracer::I()->meet(result);
		//cout<<"k-th smallest is "<<result<<endl;
	}
	delete[] arr;
}

#ifdef SELF_TEST
int main() {
	char hello[100]="aer39invqbj43o;5j46354q34534999!@#%@#$%^&$&ADGSGWREF";
	int len=strlen(hello);
	for(int i=0; i<50; i++) {
		run_KthSmallest((uint8_t*)hello,len);
	}
	return 0;
}
#endif
