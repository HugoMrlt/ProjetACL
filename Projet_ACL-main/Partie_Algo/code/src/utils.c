#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ShowMessage(char * msg, int interrupt) {
	fprintf(stderr, "%s\n", msg);
	if (interrupt) {
		fprintf(stderr, "An error has occurred. The execution of the program will be interrupted!\n");
		exit(EXIT_FAILURE);
	}
}

void viewInt(const void * i) {
	if (i == NULL)
		printf("NULL");
	else
		printf("%d", *((int*) i));
}

void freeInt(void * i) {
	free((int*) i);
}

int intSmallerThan(const void * a, const void * b) {
	if (*((int*) a) < *((int*) b))
		return 1;
	return 0;
}

int intGreaterThan(const void * a, const void * b) {
	if (*((int*) a) > *((int*) b))
		return 1;
	return 0;
}

int intEqualTo(const void * a, const void * b) {
	if (*((int*) a) == *((int*) b))
		return 1;
	return 0;
}

void view_string(const void *data) {
	printf("\"%s\"", (const char *)data);
}

void free_string(void *data) {
	if (data) free(data);
}

int compare_int_asc(const void *a, const void *b) {
	const int *x = *(const int **)a;
	const int *y = *(const int **)b;
	return (*x > *y) - (*x < *y);
}

int compare_int_desc(const void *a, const void *b) {
	const int *x = *(const int **)a;
	const int *y = *(const int **)b;
	return (*x < *y) - (*x > *y);
}

int compare_long_asc(const void *a, const void *b) {
	const unsigned long x = *(const unsigned long *)a;
	const unsigned long y = *(const unsigned long *)b;
	return (x > y) - (x < y);
}

int compare_long_desc(const void *a, const void *b) {
	const unsigned long x = *(const unsigned long *)a;
	const unsigned long y = *(const unsigned long *)b;
	return (x < y) - (x > y);
}

int int_equal(const void *a, const void *b) {
	const int *x = *(const int **)a;
	const int *y = *(const int **)b;
	return *x - *y;
}

int compare_string(const void *a, const void *b) {
	return strcmp(*(char **)a, *(char **)b);
}