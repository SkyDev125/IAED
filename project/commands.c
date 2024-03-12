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
int run_p(void **args) {
	int error_code;

	// Check if the maximum number of parks has been reached
	if (get_parks().park_num == MAX_PARKS) {
		printf("too many parks.\n");
		return SUCCESSFUL;
	}

	// Parse the name of the park
	args[0] = malloc(sizeof(char *) * STRING_CHUNKS);
	error_code = parse_string((char **)&args[0]);
	if (error_code == UNEXPECTED_INPUT) {
		show_parks();
		free(args[0]);
		return SUCCESSFUL;
	} else if (error_code == UNEXPECTED) {
		return UNEXPECTED;
	}

	// TODO: CHECK IF PARK ALREADY EXISTS BY NAME

	if (parse_p_values(args) == SUCCESSFUL_EXIT) return SUCCESSFUL;

	add_park(
		(char *)args[0], (int *)args[1], (float *)args[2], (float *)args[3],
		(float *)args[4]
	);

	return SUCCESSFUL;
}

int parse_p_values(void **args) {

	// Parse the capacity of the park
	args[1] = malloc(sizeof(int));
	parse_int(args[1]);

	// Check if the capacity is valid
	if (*(int *)args[1] <= 0) {
		printf("%i: invalid capacity.\n", *(int *)args[1]);
		free(args[0]);
		free(args[1]);
		return SUCCESSFUL_EXIT;
	}

	// Parse the costs of the park
	args[2] = malloc(sizeof(float));
	args[3] = malloc(sizeof(float));
	args[4] = malloc(sizeof(float));
	parse_float(args[2]);
	parse_float(args[3]);
	parse_float(args[4]);

	// Check if the costs are valid
	if (*(float *)(args[2]) <= 0 || *(float *)(args[2]) > *(float *)(args[3]) ||
		*(float *)(args[3]) > *(float *)(args[4])) {
		printf("invalid cost.\n");
		free(args[0]);
		free(args[1]);
		free(args[2]);
		free(args[3]);
		free(args[4]);
		return SUCCESSFUL_EXIT;
	}

	return SUCCESSFUL;
}
