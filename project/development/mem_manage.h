/**
 * @file mem_manage.h
 * @author Diogo Santos (ist1110262)
 * @brief Declarations of the functions for memory management.
 * @version 1
 * @date 27-03-2024
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef MEM_MANAGE
#define MEM_MANAGE

#include "headers.h"

/// @defgroup mem_management Memory Management Functions.
/// @{

/// Frees all allocated memory.
void free_all(park_index *parks, vehicle_index *vehicles);

/// Clean the registries from a specific park.
void clean_park_registries(registry *reg);

/// Clean the registries from a specific vehicle.
void clean_vehicle_registries(registry *reg);

/// Remove all vehicles from hashtable.
void remove_all_vehicles(vehicle_index *vehicles);

/// @}

#endif