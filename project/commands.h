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

/// @defgroup command_functions Command related function declarations
/// @{

/// Function to run the COMMAND_CREATE_OR_VIEW command, which creates a new
/// parking lot or lists existing ones
int run_p(void **args);

/// Function to get the remaining values of the COMMAND_CREATE_OR_VIEW Command
int parse_p_values(void **args);

/// Function to run the COMMAND_ADD_VEHICLE command, which registers the entry
/// of a vehicle
int run_e();

/// Function to run the COMMAND_REMOVE_VEHICLE command, which registers the exit
/// of a vehicle
int run_s();

/// Function to run the COMMAND_VIEW_VEHICLE command, which shows the vehicle
/// information
int run_v();

/// Function to run the COMMAND_PARK_BILLING command, which shows the billing of
/// a parking lot
int run_f();

/// Function to run the COMMAND_REMOVE_PARK command, which removes a parking lot
/// from the system
int run_r();

/// @}

#endif