/**
 * @file commands.h
 * @author Diogo Santos (ist1110262)
 * @brief Declarations for command functions.
 * @version 1
 * @date 27-03-2024
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef COMMANDS
#define COMMANDS

#include "headers.h"

/// @defgroup command_functions Command execution related functions.
/// @{

/// Creates a new parking lot or lists existing ones.
error_codes run_p(char *buff, park_index *parks);

/// Registers an entrance of a vehicle in a parking lot.
error_codes run_e(char *buff, sys *system);

/// Registers an exit of a vehicle in a parking lot.
error_codes run_s(char *buff, sys *system);

/// Aux function to collect the args needed for exit.
void run_s_args(char **buff, s_args *args, park_index *parks);

/// Lists all the registries of a vehicle.
error_codes run_v(char *buff, vehicle_index *vehicles);

/// List all the registries of a parking lot.
error_codes run_f(char *buff, sys *system);

/// Deletes a parking lot.
error_codes run_r(char *buff, park_index *parks);

/// @}

/// @defgroup Error_checking Error checking related functions.
/// @{

/// Error checking for entrances.
void run_e_errochecking(e_args *buff, sys *system);

/// Error checking for exits.
void run_s_errochecking(s_args *args, date *sysdate);

/// Error checking for vehicle registries listing .
void run_v_errorchecking(v_args *args);

/// Aux date checking for registry validation.
void verify_date_registry(date *sysdate, char *err, date *timestamp);

/// @}

#endif