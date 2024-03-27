/**
 * @file sorting.h
 * @author Diogo Santos (ist1110262)
 * @brief Sorting utilities declarations.
 * @version 1
 * @date 27-03-2024
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef SORTING
#define SORTING

#include "headers.h"

/// Comparison function typedef.
typedef int (*comp_func)(const void *, const void *);

/// @defgroup sorting_functions Sort related functions.
/// @{

/// Merges two sorted arrays.
void merge(void **arr, int low, int mid, int high, comp_func cmp);

/// Sorts an array using merge sort.
void merge_sort(void **arr, int low, int high, comp_func cmp);

/// @}

/// @defgroup comp_functions Comparison related functions.
/// @{

/// Compares two registries by their parks.
int compare_regs_park(registry *a, registry *b);

/// @}

#endif