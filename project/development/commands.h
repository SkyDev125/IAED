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
error_codes run_p(char *buff, park_index *parks);

/// Function to run the COMMAND_ADD_VEHICLE command, which registers the entry
/// of a vehicle
error_codes run_e(char *buff, sys *system);

void run_e_errochecking(e_args *args, sys *system);

/// Function to run the COMMAND_REMOVE_VEHICLE command, which registers the
/// exit of a vehicle
error_codes run_s(char *buff, sys *system);

void run_s_args(char **buff, s_args *args, park_index *parks);

void run_s_errochecking(s_args *args, date *sysdate);

/// Function to run the COMMAND_VIEW_VEHICLE command, which shows the vehicle
/// information
error_codes run_v(char *buff, vehicle_index *vehicles);

void run_v_errorchecking(v_args *args);

/// Function to run the COMMAND_PARK_BILLING command, which shows the billing
/// of a parking lot
error_codes run_f(char *buff, sys *system);

/// Function to run the COMMAND_REMOVE_PARK command, which removes a parking
/// lot from the system
error_codes run_r(char *buff, park_index *parks);

/// @}

void verify_date_registry(date *sysdate, char *err, date *timestamp);

#endif