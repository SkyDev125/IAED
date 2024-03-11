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

/// @defgroup command_parsing Command Parsing
/// @{

/// Checks if the given command is valid
bool is_command(int *command);

/// Returns the command from the given string
int get_command(char *command);

/// @}

/// @defgroup argument_parsing Argument Parsing
/// @{

/// Parses an integer from the given string
int parse_int(int *arg);

/// Parses a float from the given string
int parse_float(float *arg);

/// Adds a digit to the float being parsed
int add_to_float(
	float *arg, bool *dot, float *factor, bool *reading, bool *is_digit,
	bool *is_blank, int *c
);

/// Parses a string from the given input
int parse_string(char **arg);

/// Parses a quoted string from the given input
int parse_quoted_string(char **arg);

/// Parses the initial characters of a string
int parse_initial_chars(char **arg, int *current, int *next);

/// @}

/// @defgroup buffer_management Buffer Management
/// @{

/// Returns the next character from the buffer
char get_buff_char();

/// Puts a character back into the buffer
void unget_char(int *c);

/// @}

#endif