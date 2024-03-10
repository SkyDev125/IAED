/**
 * @file constants.h
 * @author Diogo Santos (ist1110262)
 * @brief
 * @version 0.1
 * @date 09-03-2024
 *
 * @copyright Copyright (c) 2024
 *
 */

#define MAX_BUFF 3
#define MAX_ARGS_SIZE 5
#define STRING_CHUNKS 30
#define MAX_PARKS 20

#define SUCCESSFUL 0
#define SUCCESSFUL_EXIT 1
#define UNEXPECTED 2
#define UNEXPECTED_INPUT 3


const char VALID_COMMANDS[] = "qpesvfr";

typedef struct {
	char *name;
	int *capacity;
	float *first_hour_value, *value, day_value;
} park;
