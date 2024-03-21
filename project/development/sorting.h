/**
 * @file sorting.h
 * @author Diogo Santos (ist1110262)
 * @brief Sorting utilities
 * @version 0.1
 * @date 10-03-2024
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef SORTING
#define SORTING

#include "headers.h"

void merge(registry **regs, int low, int mid, int high);

void merge_sort(registry **regs, int low, int high);

int compare_regs_park(registry *a, registry *b);

void merge_names(char **names, int low, int mid, int high);

void merge_sort_names(char **names, int low, int high);

#endif