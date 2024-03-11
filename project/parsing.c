/**
 * @file parsing.c
 * @author Diogo Santos (ist1110262)
 * @brief Parsing utilities for command line arguments
 * @version 0.1
 * @date 10-03-2024
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "headers.h"

/// Internal buffer for storing characters
char internal_buff[MAX_BUFF];

/// Index of the next character to be read from the internal buffer
int internal_buff_index = 0;

/// Valid commands
const char VALID_COMMANDS[] = {COMMAND_EXIT,		 COMMAND_CREATE_OR_VIEW,
							   COMMAND_ADD_VEHICLE,	 COMMAND_REMOVE_VEHICLE,
							   COMMAND_VIEW_VEHICLE, COMMAND_PARK_BILLING,
							   COMMAND_REMOVE_PARK,	 '\0'};

/**
 * @brief Get the next character from the internal buffer
 *
 * @return The next character from the buffer
 */
char get_buff_char() {
	char temp = internal_buff[internal_buff_index];
	internal_buff[internal_buff_index++] = '\0';
	return temp;
}

/**
 * @brief Put a character back into the internal buffer
 *
 * @param c The character to be put back
 */
void unget_char(int *c) {
	if (internal_buff_index + 1 != MAX_BUFF) {
		internal_buff[internal_buff_index++] = *c;
	}
}

/**
 * @brief Get a command from the internal buffer or stdin
 *
 * @param command The command to be filled
 * @return SUCCESSFUL on success, UNEXPECTED_INPUT on failure
 */
int get_command(char *command) {
	int c;

	// Check in the buffer before reading stdin
	if (internal_buff_index != 0) {
		internal_buff_index = 0;
		while ((c = get_buff_char()) != '\0') {
			if (is_command(&c)) {
				*command = c;
				return SUCCESSFUL;
			}
		}
		internal_buff_index = 0;
	}

	// Reading from stdin
	while ((c = getchar()) != EOF) {
		if (is_command(&c)) {
			*command = c;
			return SUCCESSFUL;
		}
	}

	return UNEXPECTED_INPUT;
}

/**
 * @brief Parse an integer from stdin
 *
 * @param arg The integer to be filled
 * @return SUCCESSFUL on success, UNEXPECTED_INPUT on failure (most likely found
 * a command)
 */
int parse_int(int *arg) {
	int c;
	bool reading = false;
	*arg = 0;

	while ((c = getchar()) != EOF) {

		bool is_digit = isdigit(c);
		bool is_blank = isblank(c);

		// Found unexpected value
		if (!is_blank && !is_digit) {
			ungetc(c, stdin);
			return UNEXPECTED_INPUT;
		}

		// State management
		if (is_digit && !reading) {
			reading = true;
			*arg = c - '0';
		} else if (reading && is_digit) {
			*arg = *arg * 10 + (c - '0');
		} else if (reading && is_blank) {
			break;
		}
	}

	return SUCCESSFUL;
}

/**
 * @brief Parse a float from stdin
 *
 * @param arg The float to be filled
 * @return SUCCESSFUL on success, UNEXPECTED_INPUT on failure (most likely found
 * a command)
 */
int parse_float(float *arg) {
	int c;
	bool reading = false, dot = false;
	*arg = 0;
	float factor = 10;

	while ((c = getchar()) != EOF) {

		bool is_digit = isdigit(c);
		bool is_blank = isblank(c);

		// Found unexpected value
		if (c != '.' && !is_blank && !is_digit) {
			ungetc(c, stdin);
			return UNEXPECTED_INPUT;
		}

		// State management
		if (!reading && is_digit) {
			reading = true;
		} else if (!reading && is_blank) {
			continue;
		}

		// Adding to number
		if (add_to_float(
				arg, &dot, &factor, &reading, &is_digit, &is_blank, &c
			) == SUCCESSFUL_EXIT) {
			break;
		}
	}

	return SUCCESSFUL;
}

/**
 * @brief Add a digit to a floating point number
 *
 * @param arg The float to be filled
 * @param dot A flag indicating if a decimal point has been encountered
 * @param factor The current factor for the decimal part
 * @param reading A flag indicating if a number is being read
 * @param is_digit A flag indicating if the current character is a digit
 * @param is_blank A flag indicating if the current character is a blank
 * @param c The current character
 * @return SUCCESSFUL on success, SUCCESSFUL_EXIT if a blank is encountered
 */
int add_to_float(
	float *arg, bool *dot, float *factor, bool *reading, bool *is_digit,
	bool *is_blank, int *c
) {
	if (!*dot && *reading && *is_digit) {
		*arg = *arg * 10 + (*c - '0');
	} else if (*dot && *reading && *is_digit) {
		*arg += (*c - '0') / *factor;
		*factor *= 10;
	} else if (*reading && *c == '.') {
		*dot = true;
	} else if (*reading && *is_blank) {
		return SUCCESSFUL_EXIT;
	}
	return SUCCESSFUL;
}

/**
 * @brief Parse a string from stdin
 *
 * @param arg The string to be filled
 * @return SUCCESSFUL on success, UNEXPECTED_INPUT on failure
 */
int parse_string(char **arg) {
	int current, next, error_code;
	bool reading = false;
	size_t string_size = 2;

	error_code = parse_initial_chars(arg, &current, &next);
	if (error_code != SUCCESSFUL) {
		return error_code;
	}

	while ((current = getchar()) != EOF) {

		bool is_blank = isblank(current);

		// State management
		if (!reading && !is_blank) {
			reading = true;
		} else if (!reading && is_blank) {
			continue;
		}

		// Get the string
		if (reading && !is_blank) {
			(*arg)[string_size++] = current;
		} else {
			break;
		}

		// Allocate extra memory if needed
		grow_string_cap(arg, &string_size);
	}

	// Shrink the vector to its real size
	shrink_string_cap(arg, &string_size);
	(*arg)[string_size] = '\0';

	return SUCCESSFUL;
}

/**
 * @brief Parse the initial characters of a string
 *
 * @param arg The string to be filled
 * @param current The current character
 * @param next The next character
 * @return SUCCESSFUL on success, UNEXPECTED_INPUT on failure
 */
int parse_initial_chars(char **arg, int *current, int *next) {
	while (isspace((*current = getchar())) && *current != EOF)
		;

	if (*current == '\"') {
		parse_quoted_string(arg);
		return SUCCESSFUL_EXIT;
	}

	bool next_is_space = isspace((*next = getchar()));

	if (next_is_space && is_command(current)) {
		// Return if command was found
		internal_buff_index = 0;
		unget_char(current);
		unget_char(next);
		return UNEXPECTED_INPUT;
	} else if (next_is_space) {
		(*arg)[0] = *current;
		return SUCCESSFUL_EXIT;
	}

	(*arg)[0] = *current;
	(*arg)[1] = *next;
	return SUCCESSFUL;
}

/**
 * @brief Parse a quoted string from stdin
 *
 * @param arg The string to be filled
 * @return SUCCESSFUL on success
 */
int parse_quoted_string(char **arg) {
	int c;
	size_t string_size = 0;

	while ((c = getchar()) != EOF && c != '\"') {

		(*arg)[string_size++] = c;

		// Allocate extra memory if needed
		grow_string_cap(arg, &string_size);
	}

	// Shrink the vector to its real size
	shrink_string_cap(arg, &string_size);
	(*arg)[string_size] = '\0';

	return SUCCESSFUL;
}

/**
 * @brief Check if a character is a valid command
 *
 * @param command The character to be checked
 * @return true if the character is a valid command, false otherwise
 */
bool is_command(int *command) {
	int i = 0;
	while (VALID_COMMANDS[i]) {
		if (*command == VALID_COMMANDS[i]) return true;
		i++;
	}
	return false;
}