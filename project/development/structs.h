/**
 * @file structs.h
 * @author Diogo Santos (ist1110262)
 * @brief This file includes all the structs for the project.
 * @version 1
 * @date 27-03-2024
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef STRUCTS
#define STRUCTS

#include "headers.h"

// Forward declarations
typedef struct park_struct park;
typedef struct vehicle_struct vehicle;

/// Structure to represent a date.
typedef struct {
	long int total_mins;
	int years, months, days, hours, minutes;
} date;

/// @defgroup registry_structs Structures related to registries.
/// @{

/// Representation of an ENTRY
typedef struct {
	vehicle *vehicle_ptr;
	park *park_ptr;
	date timestamp;
} registry_enter;

/// Representation of an EXIT
typedef struct {
	vehicle *vehicle_ptr;
	park *park_ptr;
	date timestamp;
	float cost;
} registry_exit;

/// Union of registries
typedef union {
	registry_enter enter;
	registry_exit exit;
} registry_union;

/// Structure to represent a registry.
typedef struct registration {
	registry_union *registration;
	registry_types type;
	struct registration *next;
} registry;

/// @}

/// @defgroup park_vehicle_structs Parks and Vehicle related structures.
/// @{

/// Structure to represent a parking lot.
typedef struct park_struct {
	char *name;
	unsigned long hashed_name;
	int capacity, free_spaces;
	float first_hour_value, value, day_value;
	registry *registries;
	registry *last_reg;
	struct park_struct *next;
	struct park_struct *previous;
} park;

/// Structure to represent an index of parking lots.
typedef struct {
	park *first, *last;
	int park_num;
} park_index;

/// Structure to represent a vehicle.
typedef struct vehicle_struct {
	char license_plate[LICENSE_PLATE_SIZE + 1];
	unsigned long hashed_plate;
	registry *registries;
	registry *last_reg;
	vehicle *next;
} vehicle;

/// Structure to represent an index of vehicles.
typedef struct {
	vehicle **buckets;
	int size;
	int vehicle_num;
} vehicle_index;

/// @}

/// @defgroup command_execution_structs Structures of command arguments.
/// @{

/// Structure to represent the arguments of 'p' command.
typedef struct {
	char *name, err[MAX_LINE_BUFF];
	int name_size, capacity;
	float first_value, value, day_value;
} p_args;

/// Structure to represent the arguments of 'e' command.
typedef struct {
	int name_size;
	char *name, license_plate[LICENSE_PLATE_SIZE + 1], err[MAX_LINE_BUFF];
	date timestamp;
	park *park;
	vehicle *vehicle;
} e_args;

/// Structure to represent the arguments of 's' command.
typedef struct {
	char *name, license_plate[LICENSE_PLATE_SIZE + 1], err[MAX_LINE_BUFF];
	date start, end;
	park *park;
	vehicle *vehicle;
	float cost;
} s_args;

/// Structure to represent the arguments of 'v' command.
typedef struct {
	char err[MAX_LINE_BUFF], license_plate[LICENSE_PLATE_SIZE + 1];
	vehicle *vehicle;
	registry **non_null_regs;
	int count;
} v_args;

/// Structure to represent the arguments of 'f' command.
typedef struct {
	char *name, err[MAX_LINE_BUFF];
	date timestamp;
	park *park;
	int name_size;
} f_args;

/// Structure to represent the arguments of 'r' command.
typedef struct {
	int name_size, count, i;
	char *name, **names;
	park *park;
} r_args;

/// @}

/// Structure to represent the system vars.
typedef struct {
	char buff[MAX_LINE_BUFF + 1], *command;
	park_index parks;
	vehicle_index vehicles;
	date sysdate;
} sys;

#endif