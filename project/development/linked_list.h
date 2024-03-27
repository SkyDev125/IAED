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

/// @defgroup park_operations Parking Lot Operations
/// @{

/// Adds a new parking lot to the list.
void add_park(p_args *args, park_index *parks);

/// Removes a parking lot from the list.
void remove_park(park *parking, park_index *parks);

/// Returns the total number of parking lots in the list.
void show_parks(park_index *parks);

park *find_park(char *name, unsigned long park_hash, park_index *parks);

/// @}

/// Hashes a string.
unsigned long hash(char *str);

unsigned long vehicle_hash(char *license_plate, int hash_size);

void resize_vehicle_index(vehicle_index *index, int new_size);

void print_registry(registry *reg);
int get_non_null_registries(registry *first_reg, registry ***destination);
void show_all_regs(registry **regs, registry *last_reg, int *size);
void show_billing_day(park *parking, date *day);
void show_billing(park *parking);
registry *find_reg(registry *reg, registry_types type);
int get_park_names(park_index *parks, char ***park_names);

/// @defgroup vehicle_operations Vehicle operations
/// @{

vehicle *add_vehicle(char *license_plate, vehicle_index *vehicles);

void remove_all_vehicles(vehicle_index *vehicles);

void clean_park_registries(registry *reg);

void clean_vehicle_registries(registry *reg);

vehicle *find_vehicle(char *license_plate, vehicle_index *vehicles);

void register_entrance(e_args *args, vehicle_index *vehicles);

void register_exit(
	park *park_enter, date *timestamp, vehicle *reg_vehicle, float *cost
);

void add_entry(
	registry **reg, registry **last_reg, registry_union *entry,
	registry_types type
);

/// @}

#endif