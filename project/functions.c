/**
 * @file functions.c
 * @author Diogo Santos (ist1110262)
 * @brief
 * @version 0.1
 * @date 09-03-2024
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "functions.h"
#include "constants.h"

char buff[MAX_BUFF];
int buff_index = 0;

int menu() {
	char command;
	int error_code;

	// Initialize the pointers at NULL
	int args_num = 0;
	void **args = malloc(MAX_ARGS_SIZE * sizeof(void *));
	for (int i = 0; i < MAX_ARGS_SIZE; i++) {
		args[i] = NULL;
	}

	while (true) {
		// Error checking for command
		error_code = get_command(&command);
		if (error_code != 0) {
			free_all(args);
			return error_code;
		}

		switch (command) {
		case 'q':
			if (args_num != 0) {
				free_all(args);
			}
			return SUCCESS;
		case 'p':
			run_p(args, &args_num);
			break;
		case 'e':
			break;
		case 's':
			break;
		case 'v':
			break;
		case 'f':
			break;
		case 'r':
			break;
		default:
			free_all(args);
			return 2;
		}
	}

	free_all(args);
	return UNEXPECTED;
}

bool is_command(int *command) {
	int i = 0;
	while (VALID_COMMANDS[i]) {
		if (*command == VALID_COMMANDS[i])
			return true;
		i++;
	}
	return false;
}

char get_buff_char() {
	char temp = buff[buff_index];
	buff[buff_index++] = '\0';
	return temp;
}

void unget_char(int *c) {
	if (buff_index + 1 != MAX_BUFF) {
		buff[buff_index++] = *c;
	}
}

int get_command(char *command) {
	int c;

	// Check in the buffer before reading stdin
	if (buff_index != 0) {
		buff_index = 0;
		while ((c = get_buff_char()) != '\0') {
			if (is_command(&c)) {
				*command = c;
				return 0;
			}
		}
		buff_index = 0;
	}

	// Reading from stdin
	while ((c = getchar()) != EOF) {
		if (is_command(&c)) {
			*command = c;
			return 0;
		}
	}

	return UNEXPECTED_INPUT;
}

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

	return SUCCESS;
}

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
		if (!dot && reading && is_digit) {
			*arg = *arg * 10 + (c - '0');
		} else if (dot && reading && is_digit) {
			*arg += (c - '0') / factor;
			factor *= 10;
		} else if (reading && c == '.') {
			dot = true;
		} else if (reading && is_blank) {
			break;
		}
	}

	return SUCCESS;
}

int parse_string(char **arg) {
	int current, next;
	bool reading = false;
	size_t string_size = 2;

	while (isspace((current = getchar())) && current != EOF)
		;

	if (current == '\"') {
		parse_quoted_string(arg);
		return SUCCESS;
	} else if (is_command(&current) && isspace((next = getchar()))) {
		// Return if command was found
		buff_index = 0;
		unget_char(&current);
		unget_char(&next);
		return UNEXPECTED_INPUT;
	} else if (isspace((next = getchar()))) {
		(*arg)[0] = current;
		return SUCCESS;
	}

	(*arg)[0] = current;
	(*arg)[1] = next;

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

	return SUCCESS;
}

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

	return SUCCESS;
}

void free_all(void **args) {
	for (int i = 0; i < MAX_ARGS_SIZE; i++) {
		if (args[i] != NULL) {
			free(args[i]);
		}
	}
	free(args);
}

int grow_string_cap(char **string, size_t *string_size) {
	if ((*string_size + 1) % STRING_CHUNKS == 0) {
		char *temp = realloc(*string, (*string_size + STRING_CHUNKS));
		if (temp == NULL) {
			return UNEXPECTED;
		} else {
			*string = temp;
		}
	}
	return 0;
};

int shrink_string_cap(char **string, size_t *string_size) {
	char *temp = realloc(*string, *string_size + 1);
	if (temp == NULL) {
		return UNEXPECTED;
	} else {
		*string = temp;
	}
	return 0;
};

int run_p(void **args, int *args_num) {
	int error_code;
	args[0] = malloc(sizeof(char *) * STRING_CHUNKS);
	error_code = parse_string((char **)&args[0]);
	if (error_code == UNEXPECTED_INPUT) {
		*args_num = 0;
		printf("show parks\n");
		return 0;
	} else if (error_code == UNEXPECTED) {
		return UNEXPECTED;
	}

	args[1] = malloc(sizeof(int));
	args[2] = malloc(sizeof(float));
	args[3] = malloc(sizeof(float));
	args[4] = malloc(sizeof(float));
	parse_int(args[1]);
	parse_float(args[2]);
	parse_float(args[3]);
	parse_float(args[4]);

	printf("create park\n");
	printf("%s\n", (char *)args[0]);
	printf("%i\n", *(int *)args[1]);
	printf("%.2f\n", *(float *)args[2]);
	printf("%.2f\n", *(float *)args[3]);
	printf("%.2f\n", *(float *)args[4]);

	return 0;
}