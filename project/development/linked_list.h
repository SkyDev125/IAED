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

// Forward declaration of vehicle
typedef struct vehicle_struct vehicle;

/// Structure to represent a date.
typedef struct {
	long int total_mins;
	int years, months, days, hours, minutes;
} date;

/// Structure to represent a parking lot.
typedef struct park_struct {
	char *name;
	unsigned long hashed_name;
	int capacity, free_spaces;
	float first_hour_value, value, day_value;
	vehicle *history_vehicles;
	date *history_dates;
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
	char *license_plate;
	unsigned long hashed_plate;
	park ***history_parks; // array of pointers that points to parks
	date *history_dates;
	struct vehicle_struct *next;
	struct vehicle_struct *previous;
} vehicle;

/// Structure to represent an index of vehicles.
typedef struct {
	vehicle *first, *last;
	int vehicle_num;
} vehicle_index;

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

bool exists_park(char *name, unsigned long name_hash, park_index *parks);

park *find_park(char *name, unsigned long park_hash, park_index *parks);

/// @}

/// Hashes a string.
unsigned long hash(char *str);

void remove_vehicle(vehicle *vehicle_del, vehicle_index *vehicles);

#endif