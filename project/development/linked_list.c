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

unsigned long vehicle_hash(char *license_plate, int hash_size) {
	unsigned long hash = 5381;
	int c;

	while ((c = *license_plate++))
		hash = ((hash << 5) + hash) + c;

	return hash % hash_size;
}

void resize_vehicle_index(vehicle_index *vehicles, int new_size) {
	vehicle **new_buckets = calloc(new_size, sizeof(vehicle *));
	for (int i = 0; i < vehicles->size; i++) {
		vehicle *current_vehicle = vehicles->buckets[i];
		while (current_vehicle != NULL) {
			vehicle *next_vehicle = current_vehicle->next;
			unsigned long new_hash =
				vehicle_hash(current_vehicle->license_plate, new_size);
			// Add the vehicle to the front of the list in the new bucket
			current_vehicle->next = new_buckets[new_hash];
			new_buckets[new_hash] = current_vehicle;
			current_vehicle = next_vehicle;
		}
	}
	free(vehicles->buckets);
	vehicles->buckets = new_buckets;
	vehicles->size = new_size;
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
	park *new_park = malloc(sizeof(park));

	new_park->name = name;
	new_park->hashed_name = hash(name);
	new_park->capacity = *capacity;
	new_park->free_spaces = *capacity;
	new_park->first_hour_value = *first_hour_value;
	new_park->value = *value;
	new_park->day_value = *day_value;
	new_park->registries = NULL;
	new_park->last_reg = NULL;
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

void remove_park(park *parking, park_index *parks) {
	// If the park is NULL, there's nothing to remove
	if (parking == NULL) return;

	if (parking->registries != NULL) {
		clean_park_registries(parking->registries);
	}

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
			"%s %i %i\n", current->name, current->capacity,
			current->free_spaces
		);
		current = current->next;
	}
}

park *find_park(char *name, unsigned long name_hash, park_index *parks) {
	int i;
	park *current = parks->first;
	for (i = 0; i < parks->park_num; i++) {
		if (name_hash == current->hashed_name) {
			if (strcmp(name, current->name) == 0) return current;
		}
		current = current->next;
	}

	return NULL;
}

vehicle *add_vehicle(char *license_plate, vehicle_index *vehicles) {
	// Allocate memory for new park and initialize its fields
	vehicle *new_vehicle = malloc(sizeof(vehicle));

	// Calculate the load factor
	float load_factor = (float)vehicles->vehicle_num / vehicles->size;

	// If the load factor is greater than a certain threshold (e.g., 0.75),
	// resize the hashmap
	if (load_factor > 0.75) {
		resize_vehicle_index(vehicles, vehicles->size * 2);
	}

	// Compute the hash of the vehicle's license plate
	unsigned long hash = vehicle_hash(license_plate, vehicles->size);

	memcpy(new_vehicle->license_plate, license_plate, LICENSE_PLATE_SIZE + 1);
	new_vehicle->hashed_plate = hash;
	new_vehicle->registries = NULL;
	new_vehicle->last_reg = NULL;

	// Add the vehicle to the appropriate bucket
	new_vehicle->next = vehicles->buckets[hash];
	vehicles->buckets[hash] = new_vehicle;

	vehicles->vehicle_num++;
	return new_vehicle;
}

void remove_all_vehicles(vehicle_index *vehicles) {
	for (int i = 0; i < vehicles->size; i++) {
		vehicle *current_vehicle = vehicles->buckets[i];
		while (current_vehicle != NULL) {
			vehicle *next_vehicle = current_vehicle->next;

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

vehicle *find_vehicle(char *license_plate, vehicle_index *vehicles) {
	unsigned long license_plate_hash =
		vehicle_hash(license_plate, vehicles->size);
	vehicle *current_vehicle = vehicles->buckets[license_plate_hash];

	while (current_vehicle != NULL) {
		if (strcmp(current_vehicle->license_plate, license_plate) == 0) {
			return current_vehicle;
		}
		current_vehicle = current_vehicle->next;
	}

	return NULL;
}
void register_entrance(
	char *license_plate, vehicle_index *vehicles, park *parking,
	date *timestamp, vehicle *reg_vehicle
) {
	registry_union *entry = malloc(sizeof(registry_union));

	// create vehicle if it doesnt exist
	if (reg_vehicle == NULL) {
		reg_vehicle = add_vehicle(license_plate, vehicles);
	}

	entry->enter.park_ptr = parking;
	entry->enter.vehicle_ptr = reg_vehicle;
	entry->enter.timestamp = *timestamp;

	add_entry(
		&(reg_vehicle->registries), &(reg_vehicle->last_reg), entry, ENTER
	);
	add_entry(&(parking->registries), &(parking->last_reg), entry, ENTER);
	(parking->free_spaces)--;
}

void register_exit(
	park *parking, date *timestamp, vehicle *reg_vehicle, float *cost
) {
	registry_union *entry = malloc(sizeof(registry_union));

	entry->exit.park_ptr = parking;
	entry->exit.vehicle_ptr = reg_vehicle;
	entry->exit.timestamp = *timestamp;
	entry->exit.cost = *cost;

	add_entry(
		&(reg_vehicle->registries), &(reg_vehicle->last_reg), entry, EXIT
	);
	add_entry(&(parking->registries), &(parking->last_reg), entry, EXIT);
	(parking->free_spaces)++;
}

void add_entry(
	registry **reg, registry **last_reg, registry_union *entry, char type
) {
	registry *temp_reg, *new_reg;

	new_reg = malloc(sizeof(registry));
	new_reg->next = NULL;
	new_reg->type = type;
	new_reg->registration = entry;

	if (*reg == NULL) {
		*reg = new_reg;
		*last_reg = new_reg;
	} else {
		temp_reg = *last_reg;
		temp_reg->next = new_reg;
		*last_reg = new_reg;
	}
}

void clean_park_registries(registry *reg) {
	registry *temp_reg;

	if ((*reg).type == ENTER) {
		(*reg).registration->enter.park_ptr = NULL;
	} else if ((*reg).type == EXIT) {
		(*reg).registration->exit.park_ptr = NULL;
	}

	while ((*reg).next != NULL) {
		temp_reg = (*reg).next->next;

		if ((*reg).next->type == ENTER) {
			(*reg).next->registration->enter.park_ptr = NULL;
		} else if ((*reg).next->type == EXIT) {
			(*reg).next->registration->exit.park_ptr = NULL;
		}

		free((*reg).next);
		(*reg).next = temp_reg;
	}
	free(reg);
}

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