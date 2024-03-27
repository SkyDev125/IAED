/**
 * @file parsing.h
 * @author Diogo Santos (ist1110262)
 * @brief Declaration of parsing utilities for command line arguments.
 * @version 1
 * @date 27-03-2024
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef PARSING
#define PARSING

#include "headers.h"

/// @defgroup string_functions String related functions.
/// @{

/// Removes all whitespaces on a string and returns its pointer.
char *remove_whitespaces(char *str);

/// Tokenizes a string by returning the end pointer defined by a delimiter.
char *delimit(char *str, char delimiter);

/// @}

/// @defgroup argument_parsing Argument Parsing.
/// @{

/// Parse string from the given input.
char *parse_string(char *str_start, char **str_end, int *size);

/// Parse license plate form a string.
char *parse_license_plate(char *str, char *license_plate);

/// Parse date form a string.
char *parse_date(char *str, date *timestamp);

/// Parse time form a string.
char *parse_time(char *str, date *timestamp);

/// Finds the size of a string using its pointers.
int str_size(char **args);

/// @}

/// @defgroup argument_validation Argument validation functions.
/// @{

/// Validates a licence plate.
bool is_licence_plate(char *str);

/// Validates a date.
bool is_valid_date(date *d);

/// Verifies if it is the same day.
bool is_same_day(date *d1, date *d2);

/// @}

/// @defgroup calculation_functions Calculation related functions.
/// @{

/// Transforms date into minutes.
long int date_to_minutes(date *d);

/// Calculates the cost of parking.
float calculate_cost(date *start, date *end, park *parking);

/// @}

#endif
