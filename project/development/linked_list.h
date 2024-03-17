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

// Forward declarations
typedef struct park_struct park;
typedef struct vehicle_struct vehicle;

/// Structure to represent a date.
typedef struct {
	long int total_mins;
	int years, months, days, hours, minutes;
} date;

/// @defgroup registry_types Registration of entries and exit types.
/// @{

typedef struct {
	vehicle *vehicle_ptr;
	park **park_ptr;
	date timestamp;
} registry_enter;

typedef struct {
	vehicle *vehicle_ptr;
	park **park_ptr;
	date timestamp;
	float cost;
} registry_exit;

typedef union {
	registry_enter enter;
	registry_exit exit;
} registry_union;

typedef struct registration {
	registry_union registration;
	char type;
	struct registration *next;
} registry;

/// @}

/// @defgroup park_vehicle_types Parks and Vehicle types
/// @{

/// Structure to represent a parking lot.
typedef struct park_struct {
	char *name;
	unsigned long hashed_name;
	int capacity, free_spaces;
	float first_hour_value, value, day_value;
	registry registries;
	struct park_struct *next;
	struct park_struct *previous;
} park;

/// Structure to represent an index of parking lots.
typedef struct {
	park *first, *last;
	int park_num;
} park_index;

/// Structure to represent a vehicle.
typedef struct vehicle_struct {
	char license_plate[LICENSE_PLATE_SIZE + 1];
	unsigned long hashed_plate;
	registry registries;
	struct vehicle_struct *next;
} vehicle;

/// Structure to represent an index of vehicles.
typedef struct {
	vehicle *first, *last;
	int vehicle_num;
} vehicle_index;

/// @}

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

park *find_park(char *name, unsigned long park_hash, park_index *parks);

/// @}

/// Hashes a string.
unsigned long hash(char *str);

registry *get_last_registry(registry *reg);

/// @defgroup vehicle_operations Vehicle operations
/// @{

vehicle *add_vehicle(char *license_plate, vehicle_index *vehicles);

void remove_vehicle(vehicle_index *vehicles);

vehicle *find_vehicle(
	char *license_plate, unsigned long license_plate_hash,
	vehicle_index *vehicles
);

void register_entrance(
	char *license_plate, vehicle_index *vehicles, park **park_enter,
	date *timestamp
);

void register_exit(
	char *license_plate, vehicle_index *vehicles, park **park_enter,
	date *timestamp, float *cost
);

void add_entry(registry *reg, registry_union *entry, char type);

/// @}

#endif