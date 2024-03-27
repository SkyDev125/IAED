/**
 * @file structs.h
 * @author Diogo Santos (ist1110262)
 * @brief This file includes all the structs for the project.
 * @version 0.1
 * @date 10-03-2024
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

/// @defgroup registry_types Registration of entries and exit types.
/// @{

typedef struct {
	vehicle *vehicle_ptr;
	park *park_ptr;
	date timestamp;
} registry_enter;

typedef struct {
	vehicle *vehicle_ptr;
	park *park_ptr;
	date timestamp;
	float cost;
} registry_exit;

typedef union {
	registry_enter enter;
	registry_exit exit;
} registry_union;

typedef struct registration {
	registry_union *registration;
	registry_types type;
	struct registration *next;
} registry;

/// @}

/// @defgroup park_vehicle_types Parks and Vehicle types
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

typedef struct {
	char buff[MAX_LINE_BUFF + 1], *command;
	park_index parks;
	vehicle_index vehicles;
	date sysdate;
} sys;

typedef struct {
	char *name, err[MAX_LINE_BUFF];
	int name_size, capacity;
	float first_value, value, day_value;
} p_args;

typedef struct {
	int name_size;
	char *name, license_plate[LICENSE_PLATE_SIZE + 1], err[MAX_LINE_BUFF];
	date timestamp;
	park *park;
	vehicle *vehicle;
} e_args;

typedef struct {
	char *name, license_plate[LICENSE_PLATE_SIZE + 1], err[MAX_LINE_BUFF];
	date start, end;
	park *park;
	vehicle *vehicle;
	float cost;
} s_args;

typedef struct {
	char err[MAX_LINE_BUFF], license_plate[LICENSE_PLATE_SIZE + 1];
	vehicle *vehicle;
	registry **non_null_regs;
	int count;
} v_args;

typedef struct {
	char *name, err[MAX_LINE_BUFF];
	date timestamp;
	park *park;
	int name_size;
} f_args;

typedef struct {
	int name_size, count, i;
	char *name, **names;
	park *park;
} r_args;

#endif