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

char *remove_whitespaces(char *str) {
	char *current = str;
	while ((int)*current != '\0') {
		if (!isspace(*current)) break;
		current++;
	}

	return current;
}

char *delimit(char *str, char delimiter) {
	char *current = str;
	while ((int)*current != '\0') {
		if (*current == delimiter) break;
		current++;
	}

	return current;
}

char *parse_string(char *str_start, char **str_end, int *size) {
	char *str;

	if (str_start[0] == '"') (*size)++;
	str = malloc(sizeof(char) * (*size + 1));

	strncpy(str, str_start, *size);
	str[*size] = '\0';
	*str_end += *size + 1;

	return str;
}

int str_size(char *args) {
	char *end_args;
	if (args[0] != '"')
		end_args = delimit(args, ' ');
	else
		end_args = delimit(args + 1, '"');

	return end_args - args;
}