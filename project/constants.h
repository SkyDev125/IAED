/**
 * @file constants.h
 * @author Diogo Santos (ist1110262)
 * @brief This file contains all the constants used throughout the program.
 * @version 0.1
 * @date 09-03-2024
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef CONSTANTS
#define CONSTANTS

/// @defgroup buffer_constants Buffer related constants
/// @{

/// Buffer size for reading input from the command line
#define MAX_BUFF 3
#define MAX_LINE_BUFF 8193

/// Size of chunk to allocate for a string when reading from the buffer
#define STRING_CHUNKS 100

/// @}

/// @defgroup command_constants Command related constants
/// @{

/// Maximum number of arguments that can be passed to a command in the command
/// line interface
#define MAX_ARGS_SIZE 5

/// Command to exit the program
#define COMMAND_EXIT 'q'

/// Command to create or view a park
#define COMMAND_CREATE_OR_VIEW 'p'

/// Command to add a vehicle to the park
#define COMMAND_ADD_VEHICLE 'e'

/// Command to remove a vehicle from the park
#define COMMAND_REMOVE_VEHICLE 's'

/// Command to view a vehicle's information
#define COMMAND_VIEW_VEHICLE 'v'

/// Command to view park's billing information
#define COMMAND_PARK_BILLING 'f'

/// Command to remove a park
#define COMMAND_REMOVE_PARK 'r'

/// @}

/// @defgroup park_constants Park related constants
/// @{

/// Maximum number of parks that can be created
#define MAX_PARKS 20

/// @}

/// @defgroup exit_codes Exit code constants
/// @{

/// Return code for successful operation
#define SUCCESSFUL 0

/// Return code for successful program exit
#define SUCCESSFUL_EXIT 1

/// Return code for unexpected error
#define UNEXPECTED 2

/// Return code for unexpected input
#define UNEXPECTED_INPUT 3

/// @}

#endif