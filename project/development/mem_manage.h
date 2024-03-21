/**
 * @file mem_manage.h
 * @author Diogo Santos (ist1110262)
 * @brief This file declares the functions for memory management.
 * @version 0.1
 * @date 10-03-2024
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef MEM_MANAGE
#define MEM_MANAGE

#include "headers.h"

/// @defgroup mem_management Memory Management Functions
/// @{

/// Frees all allocated memory.
void free_all(park_index *parks, vehicle_index *vehicles);

/// @}

#endif