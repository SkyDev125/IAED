/**
 * @file mem_manage.c
 * @author Diogo Santos (ist1110262)
 * @brief Functions for memory management.
 * @version 1
 * @date 27-03-2024
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "headers.h"

/**
 * @brief Frees all allocated memory.
 *
 * @param parks Pointer to the park index.
 * @param vehicles Pointer to the vehicle index.
 */
void free_all(park_index *parks, vehicle_index *vehicles) {
	int park_num = parks->park_num, i;
	for (i = 0; i < park_num; i++) {
		remove_park(parks->first, parks);
	}
	remove_all_vehicles(vehicles);
	free(vehicles->buckets);
}

/**
 * @brief Cleans all registries associated with a park.
 *
 * @param reg Pointer to the first registry in the linked list.
 */
void clean_park_registries(registry *reg) {
	registry *temp_reg;

	if ((*reg).type == ENTER) {
		(*reg).registration->enter.park_ptr = NULL;
	} else {
		(*reg).registration->exit.park_ptr = NULL;
	}

	while ((*reg).next != NULL) {
		temp_reg = (*reg).next->next;

		if ((*reg).next->type == ENTER) {
			(*reg).next->registration->enter.park_ptr = NULL;
		} else {
			(*reg).next->registration->exit.park_ptr = NULL;
		}

		free((*reg).next);
		(*reg).next = temp_reg;
	}
	free(reg);
}

/**
 * @brief Cleans all registries associated with a vehicle.
 *
 * @param reg Pointer to the first registry in the linked list.
 */
void clean_vehicle_registries(registry *reg) {
	registry *next_reg = (*reg).next;
	registry *temp_reg;

	free((*reg).registration);

	while (next_reg != NULL) {
		temp_reg = next_reg->next;
		free(next_reg->registration);
		free(next_reg);
		next_reg = temp_reg;
	}
	free(reg);
}

/**
 * @brief Removes all vehicles from the vehicle index and frees associated
 * memory.
 *
 * @param vehicles Pointer to the vehicle index.
 */
void remove_all_vehicles(vehicle_index *vehicles) {
	int i;
	for (i = 0; i < vehicles->size; i++) {
		vehicle *current_vehicle = vehicles->buckets[i], *next_vehicle;

		while (current_vehicle != NULL) {
			next_vehicle = current_vehicle->next;

			// If the vehicle has registries, clean them
			if (current_vehicle->registries != NULL) {
				clean_vehicle_registries(current_vehicle->registries);
			}

			// Free the memory for the vehicle's registry and the vehicle
			// itself
			free(current_vehicle);
			vehicles->vehicle_num--;
			current_vehicle = next_vehicle;
		}
		vehicles->buckets[i] = NULL;
	}
}