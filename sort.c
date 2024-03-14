/*
Liam Hudson
COP3502
March 14, 2024
*/
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int extraMemoryAllocated;

void *Alloc(size_t sz)
{
	extraMemoryAllocated += sz;
	size_t* ret = malloc(sizeof(size_t) + sz);
	*ret = sz;
	printf("Extra memory allocated, size: %ld\n", sz);
	return &ret[1];
}

void DeAlloc(void* ptr)
{
	size_t* pSz = (size_t*)ptr - 1;
	extraMemoryAllocated -= *pSz;
	printf("Extra memory deallocated, size: %ld\n", *pSz);
	free((size_t*)ptr - 1);
}

size_t Size(void* ptr)
{
	return ((size_t*)ptr)[-1];
}

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int l, int r)
{
	//If the length of the array is 0 or 1, it is already sorted
	if(l == r || pData == NULL)
		printf("Already sorted.\n");
	
	
	if(l < r){
		// Declare variables
		int i, j, k;
		int mid = (l+r)/2;
		int n1 = mid - l + 1; // first sub-half
		int n2 = r - mid; // second sub-half

		// Create two temp arrays that will merge at the end of the function call
		int *left = Alloc(n1);
		int *right = Alloc(n2);

		// Recursively call the mergeSort function
		mergeSort(pData, l, mid);
		mergeSort(pData, mid, r);

		// Copy the values from pData into each temp array
		for(i = 0; i < n1; i++)
			left[i] = pData[l+i];
		for(j = 0; j < n2; j++)
			right[j] = pData[r+j];

		// Re-initialize the index values to 0 (the beginning of the arrays)
		i = 0;
		j = 0;
		k = l;
		// Sort the sub-halfs and merge them into pData array
		while(i < n1 && j < n2){
			if(left[i] <= right[j]){
				pData[k] = left[i];
				i++;
			}
			else{
				pData[k] = right[j];
				j++;
			}
			k++;
		}

		// Place any remaining values in left temp array into pData
		while(i < n1){
			pData[k] = left[i];
			i++;
			k++;
		}

		// Place any remaining values in right temp array into pData
		while(j < n2){
			pData[k] = right[j];
			j++;
			k++;
		}

		// Free the memory of the temp arrays
		DeAlloc(left);
		DeAlloc(right);
	}
}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	int i, n, *data;
	*ppData = NULL;
	
	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)malloc(sizeof(int) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i=0;i<dataSz;++i)
		{
			fscanf(inFile, "%d ",&n);
			data = *ppData + i;
			*data = n;
		}

		fclose(inFile);
	}
	
	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
    int i, sz = (dataSz > 100 ? dataSz - 100 : 0);
    int firstHundred = (dataSz < 100 ? dataSz : 100);
    printf("\tData:\n\t");
    for (i=0;i<firstHundred;++i)
    {
        printf("%d ",pData[i]);
    }
    printf("\n\t");
    
    for (i=sz;i<dataSz;++i)
    {
        printf("%d ",pData[i]);
    }
    printf("\n\n");
}

int main(void)
{
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = { "input1.txt", "input2.txt", "input3.txt", "input4.txt" };
	
	for (i=0;i<4;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)malloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		free(pDataCopy);
		free(pDataSrc);
	}
	
}
