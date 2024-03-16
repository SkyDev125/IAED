/**
 * @file commands.c
 * @author Diogo Santos (ist1110262)
 * @brief Contains the implementation of the commands for a parking lot system.
 * @version 0.1
 * @date 10-03-2024
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "headers.h"

/**
 * @brief Runs the COMMAND_CREATE_OR_VIEW command, which creates a new parking
 * lot or lists existing ones.
 *
 * @param args An array of void pointers to the arguments of the command.
 * @return SUCCESSFUL if the command is executed successfully,
 * UNEXPECTED if an error occurs.
 */
int run_p(char *args, park_index *parks) {
	char *name, err[MAX_LINE_BUFF] = {};
	int name_size, capacity;
	float first_value, value, day_value;

	if (*args == '\0') {
		show_parks(parks);
		return SUCCESSFUL;
	}

	// Get the arguments necessary
	name_size = str_size(args);
	name = parse_string(args, &args, &name_size);
	capacity = strtol(args, &args, 0);
	first_value = strtof(args, &args);
	value = strtof(args, &args);
	day_value = strtof(args, &args);

	// Error checking
	if (exists_park(hash(name), parks)) {
		sprintf(err, "%s: parking already exists.\n", name);
	} else if (capacity <= 0) {
		sprintf(err, "%i: invalid capacity.\n", capacity);
	} else if (first_value <= 0 || first_value > value || value > day_value) {
		sprintf(err, "invalid cost.\n");
	} else if (parks->park_num == MAX_PARKS) {
		sprintf(err, "too many parks.\n");
	}

	if (err[0] != '\0') {
		printf("%s", err);
		return UNEXPECTED_INPUT;
	}

	add_park(name, &capacity, &first_value, &value, &day_value, parks);

	return SUCCESSFUL;
}