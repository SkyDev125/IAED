/**
 * @file sorting.c
 * @author Diogo Santos (ist1110262)
 * @brief Sorting utilities implementation.
 * @version 0.1
 * @date 10-03-2024
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "headers.h"

void merge(registry **regs, int low, int mid, int high) {
	int i, j, k, n1 = mid - low + 1, n2 = high - mid;
	registry **L = malloc(n1 * sizeof(registry *));
	registry **R = malloc(n2 * sizeof(registry *));

	// Copy data to temp arrays L[] and R[]
	for (i = 0; i < n1; i++)
		L[i] = regs[low + i];
	for (j = 0; j < n2; j++)
		R[j] = regs[mid + 1 + j];

	// Merge the temp arrays back into regs[low..high]
	i = 0, j = 0, k = low;
	while (i < n1 && j < n2) {
		if (compare_regs_park(L[i], R[j]) <= 0) {
			regs[k] = L[i];
			i++;
		} else {
			regs[k] = R[j];
			j++;
		}
		k++;
	}

	// Copy the remaining elements of L[], if there are any
	while (i < n1) {
		regs[k] = L[i];
		i++, k++;
	}

	// Copy the remaining elements of R[], if there are any
	while (j < n2) {
		regs[k] = R[j];
		j++, k++;
	}

	// Free memory
	free(L), free(R);
}

void merge_sort(registry **regs, int low, int high) {
	if (low < high) {
		// Same as (low+high)/2, but avoids overflow for large low and high
		int mid = low + (high - low) / 2;

		// Sort first and second halves
		merge_sort(regs, low, mid);
		merge_sort(regs, mid + 1, high);

		merge(regs, low, mid, high);
	}
}

int compare_regs_park(registry *a, registry *b) {
	char *a_park_name, *b_park_name;
	if (a->type == ENTER) {
		a_park_name = a->registration->enter.park_ptr->name;
	} else {
		a_park_name = a->registration->exit.park_ptr->name;
	}

	if (b->type == ENTER) {
		b_park_name = b->registration->enter.park_ptr->name;
	} else {
		b_park_name = b->registration->exit.park_ptr->name;
	}

	// Compare park names
	return strcmp(a_park_name, b_park_name);
}