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
error_codes run_p(char *args, park_index *parks);

/// Function to run the COMMAND_ADD_VEHICLE command, which registers the entry
/// of a vehicle
error_codes
run_e(char *args, park_index *parks, vehicle_index *vehicles, date *sysdate);

void run_e_errochecking(
	park *parking, char *name, char *err, char *license_plate, date *timestamp,
	vehicle *temp_vehicle, date *sysdate
) ;

/// Function to run the COMMAND_REMOVE_VEHICLE command, which registers the
/// exit of a vehicle
error_codes
run_s(char *args, park_index *parks, vehicle_index *vehicles, date *sysdate);

void run_s_args(
	char **args, char **name, char *license_plate, date *timestamp,
	park **parking, park_index *parks
);

void run_s_errochecking(
	park *parking, char *name, char *err, char *license_plate, date *timestamp,
	vehicle *temp_vehicle, date *sysdate
);

/// Function to run the COMMAND_VIEW_VEHICLE command, which shows the vehicle
/// information
error_codes run_v(char *args, vehicle_index *vehicles);

/// Function to run the COMMAND_PARK_BILLING command, which shows the billing
/// of a parking lot
error_codes run_f(char *args, park_index *parks, date *sysdate);

/// Function to run the COMMAND_REMOVE_PARK command, which removes a parking
/// lot from the system
error_codes run_r(char *args, park_index *parks);

/// @}

void verify_date_registry(date *sysdate, char *err, date *timestamp);

#endif