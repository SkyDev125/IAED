/**
 * @file parsing.h
 * @author Diogo Santos (ist1110262)
 * @brief Parsing utilities for command line arguments
 * @version 0.1
 * @date 10-03-2024
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef PARSING
#define PARSING

#include "headers.h"

/// @defgroup string_operations Operations on strings
/// @{

/// Removes all whitespaces on a string and returns its pointer
char *remove_whitespaces(char *str);

/// Tokenizes a string by returning the end pointer defined by a delimiter
char *delimit(char *str, char delimiter);

/// @}

/// @defgroup argument_parsing Argument Parsing
/// @{

/// Parses a string from the given input
char *parse_string(char *str_start, char **str_end, int *size);

/// Finds the size of a string through its pointers
int str_size(char **args);

/// Validates a licence plate
bool is_licence_plate(char *str);

/// @}

#endif