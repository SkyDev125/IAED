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

#include "headers.h"

/// @defgroup park_functions Parking Lot related functions.
/// @{

/// Add new park to parks list.
void add_park(p_args *args, park_index *parks);

/// Remove park from parks list.
void remove_park(park *parking, park_index *parks);

/// List all parks.
void show_parks(park_index *parks);

/// List the billing of a park for a specific day.
void show_billing_day(park *parking, date *day);

/// List the billing of a park generally.
void show_billing(park *parking);

/// Get all park names and store them in a vector.
int get_park_names(park_index *parks, char ***park_names);

/// @}

/// @defgroup vehicle_functions Vehicle related functions.
/// @{

/// Add a new vehicle to vehicles hashtable.
vehicle *add_vehicle(char *license_plate, vehicle_index *vehicles);

/// Register a new entrace of a vehicle in a park.
void register_entrance(e_args *args, vehicle_index *vehicles);

/// Register a new exit of a vehicle in a park.
void register_exit(s_args *args);

/// @}

/// @defgroup registry_functions Registry related functions.
/// @{

/// List registry.
void print_registry(registry *reg);

/// Get all registries and store them in a vector.
int get_non_null_registries(registry *first_reg, registry ***destination);

/// List all registries.
void show_all_regs(registry **regs, registry *last_reg, int *size);

/// Add the entry to the registry list of a park or vehicle.
void add_entry(
	registry **reg, registry **last_reg, registry_union *entry,
	registry_types type
);

/// @}

/// @defgroup search_functions Search related functions.
/// @{

/// Find a park by name.
park *find_park(char *name, unsigned long park_hash, park_index *parks);

/// Find a vehicle by its license_plate.
vehicle *find_vehicle(char *license_plate, vehicle_index *vehicles);

/// Find a registry by type.
registry *find_reg(registry *reg, registry_types type);

/// @}

/// @defgroup hash_functions Hash & hashtable related functions.
/// @{

/// Main hashing function - djb2.
unsigned long hash(char *str);

/// Computes hash for vehicles based on license plate.
unsigned long vehicle_hash(char *license_plate, int hash_size);

/// Resize vehicle hashtable.
void resize_vehicle_index(vehicle_index *index, int new_size);

/// @}

#endif