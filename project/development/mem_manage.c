/**
 * @file mem_manage.c
 * @author Diogo Santos (ist1110262)
 * @brief This file implements the functions for memory management.
 * @version 0.1
 * @date 10-03-2024
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "headers.h"

/**
 * @brief Increases the capacity of a string.
 *
 * @param string A pointer to the string.
 * @param string_size A pointer to the size of the string.
 * @return SUCCESSFUL on success, UNEXPECTED on failure.
 */
int grow_string_cap(char **string, size_t *string_size) {
	if ((*string_size + 1) % STRING_CHUNKS == 0) {
		char *temp = realloc(*string, (*string_size + STRING_CHUNKS));
		if (temp == NULL) {
			return UNEXPECTED;
		} else {
			*string = temp;
		}
	}
	return SUCCESSFUL;
};

/**
 * @brief Decreases the capacity of a string.
 *
 * @param string A pointer to the string.
 * @param string_size A pointer to the size of the string.
 * @return SUCCESSFUL on success, UNEXPECTED on failure.
 */
int shrink_string_cap(char **string, size_t *string_size) {
	char *temp = realloc(*string, *string_size + 1);
	if (temp == NULL) {
		return UNEXPECTED;
	} else {
		*string = temp;
	}
	return SUCCESSFUL;
};

/**
 * @brief Frees all allocated memory.
 *
 * @param args A pointer to the memory to be freed.
 */
void free_all(park_index *parks, vehicle_index *vehicles) {
	int park_num = parks->park_num, i;
	for (i = 0; i < park_num; i++) {
		remove_park(parks->first, parks);
	}
	remove_all_vehicles(vehicles);
	free(vehicles->buckets);
}