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
	return hash(license_plate) % hash_size;
}

void resize_vehicle_index(vehicle_index *vehicles, int new_size) {
	vehicle **new_buckets = calloc(new_size, sizeof(vehicle *)),
			*current_vehicle, *next_vehicle;
	unsigned long new_hash;
	for (int i = 0; i < vehicles->size; i++) {
		current_vehicle = vehicles->buckets[i];
		while (current_vehicle != NULL) {
			next_vehicle = current_vehicle->next;
			new_hash = vehicle_hash(current_vehicle->license_plate, new_size);
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
void add_park(p_args *args, park_index *parks) {
	// Allocate memory for new park and initialize its fields
	park *new_park = malloc(sizeof(park));

	new_park->name = args->name;
	new_park->hashed_name = hash(args->name);
	new_park->capacity = args->capacity;
	new_park->free_spaces = args->capacity;
	new_park->first_hour_value = args->first_value;
	new_park->value = args->value;
	new_park->day_value = args->day_value;
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
	unsigned long hash;
	float load_factor;

	// Calculate the load factor
	load_factor = (float)vehicles->vehicle_num / vehicles->size;

	// If the load factor is greater than a certain threshold (e.g., 0.75),
	// resize the hashmap
	if (load_factor > 0.75) {
		resize_vehicle_index(vehicles, vehicles->size * 2);
	}

	// Compute the hash of the vehicle's license plate
	hash = vehicle_hash(license_plate, vehicles->size);

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
void register_entrance(e_args *args, vehicle_index *vehicles) {
	registry_union *entry = malloc(sizeof(registry_union));

	// create vehicle if it doesnt exist
	if (args->vehicle == NULL) {
		args->vehicle = add_vehicle(args->license_plate, vehicles);
	}

	entry->enter.park_ptr = args->park;
	entry->enter.vehicle_ptr = args->vehicle;
	entry->enter.timestamp = args->timestamp;

	add_entry(
		&(args->vehicle->registries), &(args->vehicle->last_reg), entry, ENTER
	);
	add_entry(
		&(args->park->registries), &(args->park->last_reg), entry, ENTER
	);
	(args->park->free_spaces)--;
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
	registry **reg, registry **last_reg, registry_union *entry,
	registry_types type
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

void show_all_regs(registry **regs, registry *last_reg, int *size) {
	date *timestamp;
	int i;

	if (last_reg->type == EXIT) {
		for (i = 0; i < *size; i++) {
			print_registry(regs[i]);
		}
	} else {
		for (i = 0; i < *size; i++) {
			if (regs[i] == last_reg) {
				timestamp = &(regs[i]->registration->enter.timestamp);
				printf(
					"%s %02d-%02d-%04d %02d:%02d\n",
					last_reg->registration->enter.park_ptr->name,
					timestamp->days, timestamp->months, timestamp->years,
					timestamp->hours, timestamp->minutes
				);
			} else
				print_registry(regs[i]);
		}
	}
}

void print_registry(registry *reg) {
	date *timestamp;

	// parque1 01-01-2024 08:00 01-01-2024 08:10
	if (reg->type == ENTER) {
		timestamp = &(reg->registration->enter.timestamp);
		printf(
			"%s %02d-%02d-%04d %02d:%02d",
			reg->registration->enter.park_ptr->name, timestamp->days,
			timestamp->months, timestamp->years, timestamp->hours,
			timestamp->minutes
		);
	} else {
		timestamp = &(reg->registration->exit.timestamp);
		printf(
			" %02d-%02d-%04d %02d:%02d\n", timestamp->days, timestamp->months,
			timestamp->years, timestamp->hours, timestamp->minutes
		);
	}
}

int get_non_null_registries(registry *first_reg, registry ***destination) {
	int count = 0;
	registry *current = first_reg;
	while (current != NULL) {
		if ((current->type == ENTER &&
			 current->registration->enter.park_ptr != NULL) ||
			(current->type == EXIT &&
			 current->registration->exit.park_ptr != NULL)) {
			// Resize array in chunks
			if (count % CHUNK_SIZE == 0) {
				*destination = realloc(
					*destination, (count + CHUNK_SIZE) * sizeof(registry *)
				);
			}
			(*destination)[count] = current;
			count++;
		}
		current = current->next;
	}
	return count;
}

registry *find_reg(registry *reg, registry_types type) {
	registry *current_reg = reg;
	while (current_reg != NULL) {
		if (current_reg->type == type) {
			return current_reg;
		}
		current_reg = current_reg->next;
	}
	return NULL;
}

void show_billing(park *parking) {
	registry *current_reg;
	date *old_date, *current_date;
	float total_cost = 0;

	current_reg = find_reg(parking->registries, EXIT);
	if (current_reg == NULL) return;
	old_date = &(current_reg->registration->exit.timestamp);

	while (current_reg != NULL) {
		if (current_reg->type == EXIT) {
			current_date = &(current_reg->registration->exit.timestamp);
			if (is_same_day(old_date, current_date)) {
				total_cost += current_reg->registration->exit.cost;
			} else {
				printf(
					"%02d-%02d-%04d %.2f\n", old_date->days, old_date->months,
					old_date->years, total_cost
				);
				total_cost = current_reg->registration->exit.cost;
				old_date = current_date;
			}
		}
		current_reg = current_reg->next;
	}

	printf(
		"%02d-%02d-%04d %.2f\n", old_date->days, old_date->months,
		old_date->years, total_cost
	);
}

void show_billing_day(park *parking, date *day) {
	registry *current_reg = parking->registries;
	registry_exit *current_exit;
	date *temp_date;

	while (current_reg != NULL) {
		if (current_reg->type == EXIT) {
			temp_date = &(current_reg->registration->enter.timestamp);
			if (is_same_day(day, temp_date)) break;
		}
		current_reg = current_reg->next;
	}

	while (current_reg != NULL) {
		if (current_reg->type == EXIT) {
			temp_date = &(current_reg->registration->enter.timestamp);

			if (is_same_day(day, temp_date)) {
				current_exit = &(current_reg->registration->exit);
				printf(
					"%s %02d:%02d %.2f\n",
					current_exit->vehicle_ptr->license_plate,
					current_exit->timestamp.hours,
					current_exit->timestamp.minutes, current_exit->cost
				);
			} else {
				break;
			}
		}
		current_reg = current_reg->next;
	}
}

int get_park_names(park_index *parks, char ***park_names) {
	park *current = parks->first;
	int count = 0;
	while (current != NULL) {
		if (count % 10 == CHUNK_SIZE) {
			*park_names =
				realloc(*park_names, (count + CHUNK_SIZE) * sizeof(char *));
		}
		(*park_names)[count] = current->name;
		count++;
		current = current->next;
	}
	return count;
}