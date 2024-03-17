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
int run_p(char *args, park_index *parks) {
	char *name, err[MAX_LINE_BUFF] = {};
	int name_size, capacity;
	float first_value, value, day_value;

	if (*args == '\0') {
		show_parks(parks);
		return SUCCESSFUL;
	}

	// Get the necessary arguments
	name_size = str_size(&args);
	name = parse_string(args, &args, &name_size);
	capacity = strtol(args, &args, 0);
	first_value = strtof(args, &args);
	value = strtof(args, &args);
	day_value = strtof(args, &args);

	// Error checking
	if (find_park(name, hash(name), parks) != NULL) {
		sprintf(err, "%s: parking already exists.\n", name);
	} else if (capacity <= 0) {
		sprintf(err, "%i: invalid capacity.\n", capacity);
	} else if (first_value <= 0 || first_value > value || value > day_value) {
		sprintf(err, "invalid cost.\n");
	} else if (parks->park_num == MAX_PARKS) {
		sprintf(err, "too many parks.\n");
	}

	if (err[0] != '\0') {
		printf("%s", err);
		return UNEXPECTED_INPUT;
	}

	add_park(name, &capacity, &first_value, &value, &day_value, parks);

	return SUCCESSFUL;
}

int run_e(char *args, park_index *parks, vehicle_index *vehicles) {
	int name_size;
	char *name, license_plate[LICENSE_PLATE_SIZE + 1], err[MAX_LINE_BUFF] = {};
	date timestamp;
	park *parking;

	// Get the necessary arguments
	name_size = str_size(&args);
	name = parse_string(args, &args, &name_size);
	args = parse_license_plate(args, license_plate);
	args = parse_date(args, &timestamp);
	args = parse_time(args, &timestamp);
	timestamp.total_mins = date_to_minutes(&timestamp);
	parking = find_park(name, hash(name), parks);

	// Error checking
	run_e_errochecking(parking, name, err, license_plate, vehicles, &timestamp);
	if (err[0] != '\0') {
		printf("%s", err);
		return UNEXPECTED_INPUT;
	}

	register_entrance(license_plate, vehicles, &parking, &timestamp);

	return SUCCESSFUL;
}

void run_e_errochecking(
	park *parking, char *name, char *err, char *license_plate,
	vehicle_index *vehicles, date *timestamp
) {
	vehicle *temp_vehicle;
	registry *last_registry;

	if (parking == NULL) {
		sprintf(err, "%s: no such parking.\n", name);
	} else if (parking->capacity == 0) {
		sprintf(err, "%s: parking is full.\n", name);
	} else if (is_licence_plate(license_plate)) {
		sprintf(err, "%s: invalid licence plate.\n", license_plate);
	}

	temp_vehicle = find_vehicle(license_plate, hash(license_plate), vehicles);
	if (temp_vehicle != NULL) {
		last_registry = get_last_registry(&temp_vehicle->registries);
		if (last_registry->type != EXIT) {
			sprintf(err, "%s: invalid vehicle entry.\n", license_plate);
		} else if (
			last_registry->registration.exit.timestamp.total_mins > 
			timestamp->total_mins
		) {
			sprintf(err, "invalid date.\n");
		}
	}
}

// int run_s(char *args, park_index *parks, vehicle_index *vehicles) {
// 	int name_size;
// 	char *name, license_plate[LICENSE_PLATE_SIZE + 1], err[MAX_LINE_BUFF] = {};
// 	date timestamp;
// 	park *parking;

// 	// Get the necessary arguments
// 	name_size = str_size(&args);
// 	name = parse_string(args, &args, &name_size);
// 	args = parse_license_plate(args, license_plate);
// 	args = parse_date(args, &timestamp);
// 	args = parse_time(args, &timestamp);
// 	timestamp.total_mins = date_to_minutes(&timestamp);
// 	parking = find_park(name, hash(name), parks);

// 	if ((parking = find_park(name, hash(name), parks)) == NULL) {
// 		sprintf(err, "%s: no such parking.\n", name);
// 	} else if (is_licence_plate(license_plate)) {
// 		sprintf(err, "%s: invalid licence plate.\n", license_plate);
// 	}
// 	// TODO: Check if vehicle plate is in this park
// 	// TODO: Check if date is older than most recent

// 	if (err[0] != '\0') {
// 		printf("%s", err);
// 		return UNEXPECTED_INPUT;
// 	}

// 	// TODO: Make vehicle leave the park

// 	return SUCCESSFUL;
// }

// int run_v(char *args, park_index *parks, vehicle_index *vehicles) {
// 	char license_plate[9], err[MAX_LINE_BUFF] = {};
// 	strncpy(license_plate, args, LICENSE_PLATE_SIZE);
// 	license_plate[LICENSE_PLATE_SIZE] = '\0';

// 	if (is_licence_plate(license_plate)) {
// 		sprintf(err, "%s: invalid licence plate.\n", license_plate);
// 	}

// 	if (err[0] != '\0') {
// 		printf("%s", err);
// 		return UNEXPECTED_INPUT;
// 	}
// 	// TODO: Check if the license plate exists

// 	// TODO: List all entries/exits, order by park name, and then by Date and
// 	// Hour.

// 	return SUCCESSFUL;
// }

// int run_f(char *args, park_index *parks, vehicle_index *vehicles) {
// 	int name_size = str_size(&args), read_num;
// 	char *name = parse_string(args, &args, &name_size), err[MAX_LINE_BUFF] = {};
// 	date timestamp;
// 	park *parking;

// 	read_num = sscanf(
// 		args, "%2d-%2d-%4d", &timestamp.days, &timestamp.months,
// 		&timestamp.years
// 	);

// 	if ((parking = find_park(name, hash(name), parks)) == NULL) {
// 		sprintf(err, "%s: no such parking.\n", name);
// 	}

// 	if (err[0] != '\0') {
// 		printf("%s", err);
// 		return UNEXPECTED_INPUT;
// 	}

// 	if (read_num != 3) {
// 		// TODO: Order by date
// 	} else {
// 		// TODO: Order by hour in date
// 	}

// 	return SUCCESSFUL;
// }

int run_r(char *args, park_index *parks) {
	int name_size = str_size(&args);
	char *name = parse_string(args, &args, &name_size);
	park *parking;

	parking = find_park(name, hash(name), parks);
	if (parking == NULL) {
		printf("%s: no such parking.\n", name);
		return UNEXPECTED_INPUT;
	}

	remove_park(parking, parks);

	// TODO: PRINT PARKS IN ALPHABETICAL ORDER

	return SUCCESSFUL;
}