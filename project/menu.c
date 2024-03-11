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
 * UNEXPECTED if an unexpected error occurs, and UNEXPECTED_INPUT if the menu
 * receives an input that it doesn't know how to handle.
 */
int menu() {
	char command;
	int error_code;

	// Initialize the array of pointers with NULL values
	// This array will be used to store command arguments
	void **args = malloc(MAX_ARGS_SIZE * sizeof(void *));
	for (int i = 0; i < MAX_ARGS_SIZE; i++) {
		args[i] = NULL;
	}

	// Main menu loop
	while (true) {
		// Error checking for command
		error_code = get_command(&command);
		if (error_code != SUCCESSFUL) {
			free_all(args);
			return error_code;
		}

		// Execute the command and handle the return code
		// If the command is 'q', exit the loop and return SUCCESSFUL
		// If an unexpected input is received, free the args array and return
		// UNEXPECTED_INPUT
		error_code = run_command(&command, args);
		if (error_code != SUCCESSFUL) {
			free_all(args);
			if (error_code == SUCCESSFUL_EXIT) {
				return SUCCESSFUL;
			}
			return error_code;
		}
	}

	// Free the memory allocated for the args array
	free_all(args);
	return UNEXPECTED;
}

/**
 * @brief Executes the command specified by the 'command' parameter
 *
 * @param command A character representing a valid command specified in
 * VALID_COMMANDS[] in parsing.c.
 * @param args An array of pointers that contains the arguments for the command.
 * @return SUCCESSFUL if the command executes successfully,
 * UNEXPECTED_INPUT if an unexpected command is received, and SUCCESSFUL_EXIT if
 * the 'q' command is received.
 */
int run_command(char *command, void **args) {
	switch (*command) {
	case COMMAND_EXIT:
		return SUCCESSFUL_EXIT;
	case COMMAND_CREATE_OR_VIEW:
		run_p(args);
		break;
	case COMMAND_ADD_VEHICLE:
		break;
	case COMMAND_REMOVE_VEHICLE:
		break;
	case COMMAND_VIEW_VEHICLE:
		break;
	case COMMAND_PARK_BILLING:
		break;
	case COMMAND_REMOVE_PARK:
		break;
	default:
		free_all(args);
		return UNEXPECTED_INPUT;
	}
	return SUCCESSFUL;
}
