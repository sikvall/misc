// This is an implementation of the well known Fisher-Yates shuffle of
// an array of integers.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// This function takes the pointer to an array of ints and the
// shuffles them around. It is expected that the random generator has
// been seeded properly before this, see the main() which is a test
// function here
void fyshuffle(int *array, size_t n)
{
	size_t i, j;  // Elements in array to swap
	int temp;     // Temp store
	
	// Set the random generator using current time
	srand((unsigned int)time(NULL));
	for (i = n - 1; i > 0; i--) {
		j = rand() % (i + 1);
		int temp = array[j];
		array[j] = array[i];
		array[i] = temp;
	}
}

int main(int argc, char *argv[])
{
	if (argc != 2) {
		fprintf(stderr,
			"ERR: Needs to supply one argument, size of array.\n"
			"     Reasonable sizes are 1-20 or so.\n");
		exit(20);
	}

	int num_elements = atoi(argv[1]);
	if (num_elements > 50) {
		printf("That's a lot of elements, try something below 50 or so.\n");
		exit(20);
	} else if (num_elements < 2) {
		printf("It does not really make sense to shuffle less than at least 2 elements.\n");
		exit(20);
	}
	  
	printf("Number of elements to shuffle: %d\n", num_elements);

	int *array = calloc(num_elements, sizeof(int));
	if(!array) {
		fprintf(stderr, "ERR: Unable to allocate memory.\n");
		exit(20);
	}

	// Fill the array with numbers
	for (int i = 0; i < num_elements; i++)
		array[i] = i;

	// Shuffle the array
	fyshuffle(array, num_elements);

	// Print the shuffled array
	for (int i = 0; i < num_elements; i++) {
		printf("%2d ", array[i]);
	}
	printf("\n");
	
	free(array);
}

