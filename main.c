#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

int create_array(int, int **);
void shuffle_array(int, int *);
int find_missing(int, int *);

int create_array(int size, int ** array_ptr)
{
	int * addr = malloc(sizeof(int) * size);
	printf("Allocated %d bytes at 0x%x\n", sizeof(int) * size, addr);

	// Populating array
	int i;
	for(i = 1; i <= size; i++)
	{
		addr[i-1] = i;
	}
	
	// Deleting random number
	srand(time(NULL));
	if(RAND_MAX < size) {
		printf("Error: MAX_RAND inferior to the size of the array\n");
		return -1;
	}
	int choosen = rand()%size;
	printf("Random %d (%d will be deleted)\n", choosen, addr[choosen]);
	addr[choosen] = 0;
	
	(*array_ptr) = addr;
	return 0;
}

void shuffle_array(int size, int * array_ptr)
{
	printf("Shuffling the array\n");
	int i;
	int tmp;
	int newpos;
	srand(time(NULL));
	for(i = 0; i < size; i++)
	{
		newpos = rand()%size;
		tmp = array_ptr[i];
		array_ptr[i] = array_ptr[newpos];
		array_ptr[newpos] = tmp;
	}
	printf("Shuffling done\n");
}

int find_missing(int size, int * array_ptr)
{
	// Creating a binary array for validation
	char * validarray = malloc(sizeof(char) * size);
	printf("Allocated %d bytes for validation array at 0x%x\n", \
		sizeof(size) * size, validarray);

	int i;
	int value = -1;
	// Initializing to zero
	for(i = 0; i < size; i++)
	{
		validarray[i] = 0;
	}
	
	// Populating validation array
	for(i = 0; i < size; i++)
	{
		validarray[array_ptr[i]-1] = 1;
	}
	
	// Research algorithm
	struct timeval start, stop;
	gettimeofday(&start, NULL);
	for(i = 0; i < size; i++)
	{
		if(validarray[i] == 0)
		{
			gettimeofday(&stop, NULL);
			value = i+1;
			printf("Found value %d in %i.%i seconds\n",\
				value, stop.tv_sec - start.tv_sec, stop.tv_usec - start.tv_usec);
			break;
		}
	}
	
	if(value == -1)
	{
		printf("Could not find deleted value\n");
	}
	
	free(validarray);
	return value;
}

int main(int argc, char* argv[])
{
	if(argc != 2)
	{
		printf("Usage: finder <number of elements>\n");
		exit(0);
	}
	
	int num_elements = atoi(argv[1]);
	
	int * array;
	int ret = create_array(num_elements, &array);
	if(ret != 0) {
		free(array);
		exit(0);
	}
	shuffle_array(num_elements, array);
	find_missing(num_elements, array);
	free(array);
	return 0;
}
