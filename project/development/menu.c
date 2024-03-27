/**
 * @file menu.c
 * @author Diogo Santos (ist1110262)
 * @brief Contains the main menu loop and command execution functions
 * @version 0.1
 * @date 10-03-2024
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "headers.h"

/**
 * @brief Main loop function for the menu
 *
 * @return SUCCESSFUL if the menu executes successfully,
 * UNEXPECTED if an unexpected error occurs, and UNEXPECTED_INPUT if the
 * menu receives an input that it doesn't know how to handle.
 */
error_codes menu() {
	sys system = {
		.parks = {NULL, NULL, 0},
		.vehicles = {calloc(HASH_SIZE, sizeof(vehicle *)), HASH_SIZE, 0},
		.sysdate = {0, 0, 0, 0, 0, 0}};

	// Main menu loop
	while (TRUE) {
		if (fgets(system.buff, MAX_LINE_BUFF + 1, stdin) == NULL) {
			free_all(&system.parks, &system.vehicles);
			return UNEXPECTED_INPUT;
		}

		system.command = remove_whitespaces(system.buff);
		if (run_command(&system) == SUCCESSFUL_EXIT) break;
	}

	free_all(&system.parks, &system.vehicles);
	return SUCCESSFUL;
}

/**
 * @brief Executes the command specified by the 'command' parameter
 *
 * @param command A character representing a valid command specified in
 * VALID_COMMANDS[] in parsing.c.
 * @param args An array of pointers that contains the arguments for the
 * command.
 * @return SUCCESSFUL if the command executes successfully,
 * UNEXPECTED_INPUT if an unexpected command is received, and SUCCESSFUL_EXIT
 * if the 'q' command is received.
 */
error_codes run_command(sys *system) {
	// Point to args
	char *args = system->command;
	args = remove_whitespaces(++args);

	switch (*(system->command)) {
	case COMMAND_EXIT:
		return SUCCESSFUL_EXIT;
	case CREATE_OR_VIEW:
		return run_p(args, &(system->parks));
	case ADD_VEHICLE:
		return run_e(args, system);
	case REMOVE_VEHICLE:
		return run_s(args, system);
	case VIEW_VEHICLE:
		return run_v(args, &(system->vehicles));
	case PARK_BILLING:
		return run_f(args, system);
	case REMOVE_PARK:
		return run_r(args, &(system->parks));
	default:
		return UNEXPECTED_INPUT;
	}
	return SUCCESSFUL;
}
