/**
 * @file commands.h
 * @author Diogo Santos (ist1110262)
 * @brief This file contains the declarations for command functions.
 * @version 0.1
 * @date 10-03-2024
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef COMMANDS
#define COMMANDS

#include "headers.h"

/// @defgroup command_functions Command related function declarations
/// @{

/// Function to run the COMMAND_CREATE_OR_VIEW command, which creates a new
/// parking lot or lists existing ones
int run_p(char *args, park_index *parks);

/// Function to run the COMMAND_ADD_VEHICLE command, which registers the entry
/// of a vehicle
int run_e(char *args, park_index *parks, vehicle_index *vehicles);

void run_e_errochecking(
	park *parking, char *name, char *err, char *license_plate,
	vehicle_index *vehicles, date *timestamp
);

/// Function to run the COMMAND_REMOVE_VEHICLE command, which registers the
/// exit of a vehicle
int run_s(char *args, park_index *parks, vehicle_index *vehicles);

/// Function to run the COMMAND_VIEW_VEHICLE command, which shows the vehicle
/// information
int run_v(char *args, park_index *parks, vehicle_index *vehicles);

/// Function to run the COMMAND_PARK_BILLING command, which shows the billing of
/// a parking lot
int run_f(char *args, park_index *parks, vehicle_index *vehicles);

/// Function to run the COMMAND_REMOVE_PARK command, which removes a parking lot
/// from the system
int run_r(char *args, park_index *parks);

/// @}

void verify_date_registry(park *parking, char *err, date *timestamp);

#endif