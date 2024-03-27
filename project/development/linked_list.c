/**
 * @file linked_list.c
 * @author Diogo Santos (ist1110262)
 * @brief Implementation of linked lists & hashmaps.
 * @version 1
 * @date 27-03-2024
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "headers.h"

/**
 * @brief Main hashing function - djb2.
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
 * @brief Computes hash value for a vehicle's license plate.
 *
 * @param license_plate Vehicle's license plate.
 * @param hash_size Size of the hash table.
 * @return Hash value.
 */
unsigned long vehicle_hash(char *license_plate, int hash_size) {
	return hash(license_plate) % hash_size;
}

/**
 * @brief Resizes the vehicle index hash table.
 *
 * @param vehicles Vehicle index.
 * @param new_size New size for the hash table.
 */
void resize_vehicle_index(vehicle_index *vehicles, int new_size) {
	vehicle **new_buckets = calloc(new_size, sizeof(vehicle *)),
			*current_vehicle, *next_vehicle;
	unsigned long new_hash;
	int i;

	// Iterate over the old hash table and rehash all vehicles.
	for (i = 0; i < vehicles->size; i++) {
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

	// Free the old hash table and update the vehicle index.
	free(vehicles->buckets);
	vehicles->buckets = new_buckets;
	vehicles->size = new_size;
}

/**
 * @brief Adds a new park to the park index.
 *
 * @param args Arguments for the 'p' command.
 * @param parks Park index.
 */
void add_park(p_args *args, park_index *parks) {
	park *new_park = malloc(sizeof(park));

	// Initialize the new park values.
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

/**
 * @brief Removes a park from the park index.
 *
 * @param parking Park to remove.
 * @param parks Park index.
 */
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
 * @brief Lists all parks in the park index.
 *
 * @param parks Park index.
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

/**
 * @brief Finds a park in the park index.
 *
 * @param name Name of the park.
 * @param name_hash Hashed name of the park.
 * @param parks Park index.
 * @return Pointer to the park if found, NULL otherwise.
 */
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

/**
 * @brief Adds a new vehicle to the vehicle index.
 *
 * @param license_plate License plate of the vehicle.
 * @param vehicles Vehicle index.
 * @return Pointer to the new vehicle.
 */
vehicle *add_vehicle(char *license_plate, vehicle_index *vehicles) {
	vehicle *new_vehicle = malloc(sizeof(vehicle));
	unsigned long hash;
	float load_factor;

	// Calculate the load factor
	load_factor = (float)vehicles->vehicle_num / vehicles->size;
	if (load_factor > 0.75) {
		resize_vehicle_index(vehicles, vehicles->size * 2);
	}

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

/**
 * @brief Finds a vehicle in the vehicle index.
 *
 * @param license_plate License plate of the vehicle.
 * @param vehicles Vehicle index.
 * @return Pointer to the vehicle if found, NULL otherwise.
 */
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

/**
 * @brief Registers the entrance of a vehicle to a park.
 *
 * @param args Arguments for the 'e' command.
 * @param vehicles Vehicle index.
 */
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

/**
 * @brief Registers the exit of a vehicle from a park.
 *
 * @param args Arguments for the 's' command.
 */
void register_exit(s_args *args) {
	registry_union *entry = malloc(sizeof(registry_union));

	entry->exit.park_ptr = args->park;
	entry->exit.vehicle_ptr = args->vehicle;
	entry->exit.timestamp = args->end;
	entry->exit.cost = args->cost;

	add_entry(
		&(args->vehicle->registries), &(args->vehicle->last_reg), entry, EXIT
	);
	add_entry(&(args->park->registries), &(args->park->last_reg), entry, EXIT);
	(args->park->free_spaces)++;
}

/**
 * @brief Adds a new entry to a registry.
 *
 * @param reg Pointer to the registry.
 * @param last_reg Pointer to the last registry.
 * @param entry Entry to add.
 * @param type Type of the registry (ENTER or EXIT).
 */
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

/**
 * @brief Prints all registries in the given array.
 *
 * @param regs Array of registries.
 * @param last_reg Last registry in the array.
 * @param size Size of the array.
 */
void show_all_regs(registry **regs, registry *last_reg, int *size) {
	date *timestamp;
	int i;

	if (last_reg->type == EXIT) {
		for (i = 0; i < *size; i++) {
			print_registry(regs[i]);
		}
		// Extra checking is needed for the last print with \n.
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

/**
 * @brief Prints a single registry.
 *
 * @param reg Registry to print.
 */
void print_registry(registry *reg) {
	date *timestamp;

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

/**
 * @brief Counts and returns non-null registries from a linked list.
 *
 * @param first_reg First registry in the linked list.
 * @param destination Pointer to the destination array.
 * @return Count of non-null registries.
 */
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

/**
 * @brief Finds a registry of a specific type in a linked list of registries.
 *
 * @param reg First registry in the linked list.
 * @param type Type of the registry to find.
 * @return Pointer to the found registry, or NULL if not found.
 */
registry *find_reg(registry *first_reg, registry_types type) {
	registry *current_reg = first_reg;
	while (current_reg != NULL) {
		if (current_reg->type == type) {
			return current_reg;
		}
		current_reg = current_reg->next;
	}
	return NULL;
}

/**
 * @brief Prints the total cost of all exits from a park for each day.
 *
 * @param parking Pointer to the park.
 */
void show_billing(park *parking) {
	registry *current_reg;
	date *old_date, *current_date;
	float total_cost = 0;

	// Find the first exit registry
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

/**
 * @brief Prints the cost of all exits from a park for a specific day.
 *
 * @param parking Pointer to the park.
 * @param day Date of the day to show billing for.
 */
void show_billing_day(park *parking, date *day) {
	registry *current_reg = parking->registries;
	registry_exit *current_exit;
	date *temp_date;

	// Find the first exit registry of the day
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

/**
 * @brief Get the names of all parks in a park index.
 *
 * @param parks Pointer to the park index.
 * @param park_names Pointer to the destination array for the park names.
 * @return Count of parks.
 */
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