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