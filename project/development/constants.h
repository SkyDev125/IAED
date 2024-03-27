/**
 * @file constants.h
 * @author Diogo Santos (ist1110262)
 * @brief Constants used throughout the program.
 * @version 1
 * @date 27-03-2024
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef CONSTANTS
#define CONSTANTS

/// @defgroup buffer_constants Buffer related constants.
/// @{

/// Buffer size for reading input from the command line.
#define MAX_LINE_BUFF 8192

/// Chunk size to allocate memory in most situations.
#define CHUNK_SIZE 100

/// Hash-map default size.
#define HASH_SIZE 100

/// @}

/// @defgroup command_constants Command related constants.
/// @{

/// Valid commands.
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

/// @defgroup park_vehicle_constants Park & Vehicle related constants.
/// @{

/// Maximum number of parks.
#define MAX_PARKS 20

/// Maximum size of License plates.
#define LICENSE_PLATE_SIZE 8

/// Maximum size of date.
#define DATE_READ_SIZE 10

/// Maximum size of hour.
#define HOUR_READ_SIZE 5

/// Step in minutes for counting the cost.
#define PARK_PAY_STEP 15

/// Blocks of time that will be counted at a different price.
#define TOTAL_INITIAL_BLOCKS 4

/// @}

/// @defgroup Registry related constants.
/// @{

/// Valid Registry types.
typedef enum registry_types_e { ENTER, EXIT, UNDEFINED } registry_types;

/// @}

/// @defgroup date_constants Date related constants.
/// @{

/// Days per month.
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

/// Total days in a year.
#define DAYS_IN_YEAR 365

/// Total minutes in a day.
#define MINS_PER_DAY 24 * 60

/// @}

/// @defgroup error_codes Error codes constants.
/// @{

/// error codes.
typedef enum error_codes_e {
	SUCCESSFUL = 0,
	SUCCESSFUL_EXIT = 1,
	UNEXPECTED = 2,
	UNEXPECTED_INPUT = 3
} error_codes;

/// @}

/// @defgroup boolean constants.
/// @{

/// True value for comparisons.
#define TRUE 1

/// False value for comparisons.
#define FALSE 0

/// Alias for boolean.
typedef char bool;

/// @}

#endif