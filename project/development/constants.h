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
#define MAX_LINE_BUFF 8192

/// Size of chunk to allocate for a string when reading from the buffer
#define STRING_CHUNKS 100

/// @}

/// @defgroup date_constants Date related constants
/// @{

/// Days per month
enum months {
	JAN = 31,
	FEB = 28,
	MAR = 31,
	APR = 30,
	MAY = 31,
	JUN = 30,
	JUL = 31,
	AUG = 31,
	SEP = 30,
	OCT = 31,
	NOV = 30,
	DEC = 31,
};

/// Total days in a year
#define DAYS_IN_YEAR 365

/// Total minutes in a day
#define MINS_PER_DAY 24 * 60

/// @}

/// @defgroup command_constants Command related constants
/// @{

/// Maximum number of arguments that can be passed to a command in the command
/// line interface
#define MAX_ARGS_SIZE 5

/// Valid commands
enum commands {
	COMMAND_EXIT = 'q',
	CREATE_OR_VIEW = 'p',
	ADD_VEHICLE = 'e',
	REMOVE_VEHICLE = 's',
	VIEW_VEHICLE = 'v',
	PARK_BILLING = 'f',
	REMOVE_PARK = 'r'
};

/// @}

/// @defgroup park_constants Park related constants
/// @{

/// Maximum number of parks
#define MAX_PARKS 20

/// Maximum size of License plates
#define LICENSE_PLATE_SIZE 8

/// Maximum size of date
#define DATE_MAX_SIZE 10

/// Maximum size of hour
#define HOUR_MAX_SIZE 5

/// Step in minutes for counting the cost
#define PARK_PAY_STEP 15

/// Blocks of time that will be counted at a different price
#define TOTAL_INITIAL_BLOCKS 4

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

/// @defgroup boolean constants
/// @{

/// True value for comparisons
#define TRUE 1

/// False value for comparisons
#define FALSE 0

/// Alias for boolean
typedef char bool;

/// @}

/// @defgroup Registry related constants
/// @{

/// Valid Registry types
typedef enum registry_types_e { ENTER, EXIT, UNDEFINED } registry_types;

/// @}

#endif