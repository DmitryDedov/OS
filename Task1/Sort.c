#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void read(char* nameInFile);
void sort(int left, int right);
void write(char* nameOutFile);
void threadCreate();
void threadStart();
int* inputArray;
int size;
pthread_t thread1, thread2;
bool b = false;
int leftGlobal, rightGlobal;

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		printf("Input correct name in/out files, bitches!\n");
		return 0;
	}
	read(argv[1]);

	struct timespec start1Thread;
    clock_gettime(1, &start1Thread);
    sort(0, size - 1);
    struct timespec stop1Thread;
    clock_gettime(1, &stop1Thread);
	printf("1 thread: %d sec %lu nanosec\n", abs(stop1Thread.tv_sec - start1Thread.tv_sec), labs(stop1Thread.tv_nsec - start1Thread.tv_nsec));

	read(argv[1]);

	struct timespec start2ThreadEnd;
	clock_gettime(1, &start2ThreadEnd);
    threadCreate();
    struct timespec stop2ThreadEnd;
    clock_gettime(1, &stop2ThreadEnd);
	printf("2 thread - end: %d sec %lu nanosec\n", abs(stop2ThreadEnd.tv_sec - start2ThreadEnd.tv_sec), labs(stop2ThreadEnd.tv_nsec - start2ThreadEnd.tv_nsec));

	write(argv[2]);
	delete[] inputArray;
	return 0;
}

static void *threadStart(void *vptr_args)
{
	int *i = (int *)vptr_args;
	if (*i == 1)
		sort(0, leftGlobal);
	if (*i == 2)
		sort(rightGlobal, size - 1);
}

void threadCreate()
{
	int par1 = 1;
	int par2 = 2;

	struct timespec start2ThreadSeparation;
	clock_gettime(1, &start2ThreadSeparation);
	b = true;
	sort(0, size - 1);
	b = false;
    struct timespec stop2ThreadSeparation;
    clock_gettime(1, &stop2ThreadSeparation);
	printf("2 thread - separation: %d sec %lu nanosec\n", abs(stop2ThreadSeparation.tv_sec - start2ThreadSeparation.tv_sec), labs(stop2ThreadSeparation.tv_nsec - start2ThreadSeparation.tv_nsec));

	pthread_create(&thread1, NULL, threadStart, &par1);
	pthread_create(&thread2, NULL, threadStart, &par2);
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
}

void read(char* nameInFile)
{
	FILE *fp;
	fp = fopen(nameInFile, "rt");
	fscanf(fp, "%d", &size);
	inputArray = new int[size];
	for(int i = 0; i <= size; i++)
	{
		fscanf(fp, "%d", &inputArray[i]);
	}
}

void sort(int left, int right)
{
	int i, j, x, temp;
	i = left;
	j = right;
	x = inputArray[(left + right) / 2];
	while(i < j)
    {
		while (inputArray[i] < x) 
    	{
			i++;
        }
		while (inputArray[j] > x) 
        {
			j--;
        }
		if (i <= j) 
        {
			temp = inputArray[i];
			inputArray[i] = inputArray[j];
			inputArray[j] = temp;
			i++;
			j--;
        }
    }
    if (b)
    {
    	printf("Left end: %d\n", j);
    	printf("Right end: %d\n", i);
		leftGlobal = j;
		rightGlobal = i;
    	return;
    }
    else
    {
		if (left < j) 
			sort(left, j);
		if (i < right) 
			sort(i, right); 
	}
}

void write(char* nameOutFile)
{
	FILE *fp;
	fp = fopen(nameOutFile, "wt");
	fprintf(fp, "%d", size);
	fprintf(fp, "\n");
	for(int i = 0; i < size; i++)
	{
		fprintf(fp, "%d", inputArray[i]);
		fprintf(fp, "\n");
	}
}