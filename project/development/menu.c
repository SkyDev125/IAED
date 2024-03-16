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
int menu() {
	char buffer[MAX_LINE_BUFF + 1], *command;
	park_index parks = {NULL, NULL, 0};

	// Main menu loop
	while (TRUE) {
		if (fgets(buffer, MAX_LINE_BUFF + 1, stdin) == NULL)
			return UNEXPECTED_INPUT;

		command = remove_whitespaces(buffer);
		if (run_command(command, &parks) == SUCCESSFUL_EXIT) break;
	}

	free_all(&parks);
	return SUCCESSFUL;
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
int run_command(char *command, park_index *parks) {
	// Point to args
	char *args = command;
	args = remove_whitespaces(++args);

	switch (*command) {
	case EXIT:
		return SUCCESSFUL_EXIT;
	case CREATE_OR_VIEW:
		return run_p(args, parks);
	case ADD_VEHICLE:
		break;
	case REMOVE_VEHICLE:
		break;
	case VIEW_VEHICLE:
		break;
	case PARK_BILLING:
		break;
	case REMOVE_PARK:
		break;
	default:
		return UNEXPECTED_INPUT;
	}
	return SUCCESSFUL;
}