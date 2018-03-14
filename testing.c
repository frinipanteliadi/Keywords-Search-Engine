#include <stdio.h>
#include <stdlib.h>

int initializePointer(int** ptr){
	*ptr = (int*)malloc(sizeof(int));
	if(*ptr == NULL)
		return -1;
	return 0;
}

int main(void){
	int *a = NULL;
	printf("1: %x\n",a);
	if(initializePointer(&a) == -1)
		return -1;
	printf("2: %x\n",a);

	free(a);
	return 0;
}