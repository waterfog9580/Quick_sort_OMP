#include<iostream>
#include<stdlib.h>
#include<time.h>
#include<vector>
#include<omp.h>
#include<sys/time.h>
#define MAX_COUNT 10000

using namespace std;

int arr[MAX_COUNT];

void get_randArr();
void quickSort_parallel(int lenArray, int numThreads);
void quickSort_parallel_internal(int left, int right, int cutoff);

void get_randArr(){
	srand((unsigned int)time(NULL));

	for (int i = 0; i < MAX_COUNT; i++)
	{
		arr[i] = rand() % MAX_COUNT;
	}
}

void quickSort_parallel(int lenArray, int numThreads){

	int cutoff = 1000;

	#pragma omp parallel num_threads(numThreads)
	{	
		#pragma omp single nowait
		{
			quickSort_parallel_internal(0, MAX_COUNT-1, cutoff);	
		}
	}	

}



void quickSort_parallel_internal(int left, int right, int cutoff) 
{
	
	int i = left, j = right;
	int tmp;
	int pivot = arr[(left + right) / 2];
	
	{
	  	/* PARTITION PART */
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
		}

	}

	if ( ((right-left)<cutoff) ){
		if (left < j){ quickSort_parallel_internal(left, j, cutoff); }			
		if (i < right){ quickSort_parallel_internal(i, right, cutoff); }

	}else{
		#pragma omp task
		{ quickSort_parallel_internal(left, j, cutoff); }
		#pragma omp task
		{ quickSort_parallel_internal(i, right, cutoff); }		
	}

}

int main(){
    	float sum = 0;
	for (int i = 0; i < 10000; i++)
	{
		struct timeval stime,etime,gap;
		get_randArr();
		
		gettimeofday(&stime,NULL);
    		quickSort_parallel(MAX_COUNT,1024);
       		gettimeofday(&etime,NULL);

       		gap.tv_sec = etime.tv_sec - stime.tv_sec;
	   	gap.tv_usec = etime.tv_usec - stime.tv_usec;

       		if(gap.tv_usec < 0){
           		gap.tv_sec = gap.tv_sec-1; 
           		gap.tv_usec = gap.tv_usec + 1000000;
       		}
       		sum += gap.tv_usec;
    	}
    	cout << sum/10000 << endl;
}
