/**
 * @file functions.h
 * @author Diogo Santos (ist1110262)
 * @brief
 * @version 0.1
 * @date 09-03-2024
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "headers.h"

int menu();

bool is_command(int *command);
int get_command(char *command);
int get_arguments(char *command, void **args, int *args_num);

int parse_int(int *arg);
int parse_float(float *arg);
int parse_string(char **arg);
int parse_quoted_string(char **arg);

char get_buff_char();
void unget_char(int *c);

void free_all(void **args);

void create_park();
void show_parks();

int grow_string_cap(char **string, size_t *string_size);
int shrink_string_cap(char **string, size_t *string_size);

int run_p(void **args, int *args_num);
int run_e();
int run_s();
int run_f();
int run_r();