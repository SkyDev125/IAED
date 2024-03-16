/**
 * @file linked_list.c
 * @author Diogo Santos (ist1110262)
 * @brief This file contains the implementation of a linked list for managing
 * parks.
 * @version 0.1
 * @date 10-03-2024
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "headers.h"

/**
 * @brief Function to generate a hash value for a string.
 *
 * @param str The string to be hashed.
 * @return The hash value of the string.
 */
unsigned long hash(char *str) {
	unsigned long hash = 5381;
	int c;

	while ((c = *str++))
		hash = ((hash << 5) + hash) + c;

	return hash;
}

/**
 * @brief Function to add a new park to the linked list.
 *
 * @param name The name of the park.
 * @param capacity The capacity of the park.
 * @param first_hour_value The cost for the first hour of parking.
 * @param value The cost for subsequent hours of parking.
 * @param day_value The cost for a full day of parking.
 */
void add_park(
	char *name, int *capacity, float *first_hour_value, float *value,
	float *day_value, park_index *parks
) {
	// Allocate memory for new park and initialize its fields
	park *new_park = (park *)malloc(sizeof(park));

	new_park->name = name;
	new_park->hashed_name = hash(name);
	new_park->capacity = *capacity;
	new_park->free_spaces = *capacity;
	new_park->first_hour_value = *first_hour_value;
	new_park->value = *value;
	new_park->day_value = *day_value;
	new_park->next = NULL;

	// Add new park to the end of the linked list
	if (parks->park_num == 0) {
		parks->first = new_park;
		parks->last = new_park;
		new_park->previous = NULL;
	} else {
		new_park->previous = parks->last;
		parks->last->next = new_park;
		parks->last = new_park;
	}

	parks->park_num++;
}

/**
 * @brief Function to remove a park from the linked list.
 *
 * @param parking The park to be removed. This should be a pointer to a park
 * structure that is already in the list.
 */
void remove_park(park *parking, park_index *parks) {
	// If the park is NULL, there's nothing to remove
	if (parking == NULL) return;

	// If the park isnt the first update next
	if (parking->previous != NULL) {
		parking->previous->next = parking->next;
	} else {
		if (parking->next != NULL) {
			parking->next->previous = NULL;
		}
		parks->first = parking->next;
	}

	// If the park isnt the last update previous
	if (parking->next != NULL) {
		parking->next->previous = parking->previous;
	} else {
		if (parking->previous != NULL) {
			parking->previous->next = NULL;
		}
		parks->last = parking->previous;
	}

	// Free the memory allocated for the park's name and the park itself
	free(parking->name);
	free(parking);
	parks->park_num--;
}

/**
 * @brief Function to print all parks in the linked list.
 */
void show_parks(park_index *parks) {
	int i;
	park *current = parks->first;
	for (i = 0; i < parks->park_num; i++) {
		printf(
			"%s %i %i\n", current->name, current->capacity, current->free_spaces
		);
		current = current->next;
	}
}

bool exists_park(unsigned long park_hash, park_index *parks) {
	int i;
	park *current = parks->first;
	for (i = 0; i < parks->park_num; i++) {
		if (park_hash == current->hashed_name) return TRUE;
		current = current->next;
	}

	return FALSE;
}