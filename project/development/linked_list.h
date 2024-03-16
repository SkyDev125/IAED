/**
 * @file linked_list.h
 * @author Diogo Santos (ist1110262)
 * @brief This file contains the definitions for a linked list of parking lots.
 * @version 0.1
 * @date 10-03-2024
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef LINKED_LIST
#define LINKED_LIST

/// Structure to represent a parking lot.
typedef struct parking {
	char *name;
	unsigned long hashed_name;
	int capacity, free_spaces;
	float first_hour_value, value, day_value;
	struct parking *next;
	struct parking *previous;
} park;

/// Structure to represent an index of parking lots.
typedef struct {
	park *first, *last;
	int park_num;
} park_index;

/// @defgroup park_operations Parking Lot Operations
/// @{

/// Adds a new parking lot to the list.
void add_park(
	char *name, int *capacity, float *first_hour_value, float *value,
	float *day_value, park_index *parks
);

/// Removes a parking lot from the list.
void remove_park(park *parking, park_index *parks);

/// Returns the total number of parking lots in the list.
void show_parks(park_index *parks);

bool exists_park(unsigned long park_hash, park_index *parks);

/// Hashes a string.
unsigned long hash(char *str);

/// @}

#endif