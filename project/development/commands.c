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
	vehicle *temp_vehicle;

	// Get the necessary arguments
	name_size = str_size(&args);
	name = parse_string(args, &args, &name_size);
	args = parse_license_plate(args, license_plate);
	args = parse_date(args, &timestamp);
	args = parse_time(args, &timestamp);
	timestamp.total_mins = date_to_minutes(&timestamp);
	parking = find_park(name, hash(name), parks);
	temp_vehicle = find_vehicle(license_plate, vehicles);

	// Error checking
	run_e_errochecking(
		parking, name, err, license_plate, &timestamp, temp_vehicle
	);
	if (err[0] != '\0') {
		printf("%s", err);
		free(name);
		return UNEXPECTED_INPUT;
	}

	register_entrance(
		license_plate, vehicles, parking, &timestamp, temp_vehicle
	);
	printf("%s %i\n", name, parking->free_spaces);

	free(name);
	return SUCCESSFUL;
}

void run_e_errochecking(
	park *parking, char *name, char *err, char *license_plate, date *timestamp,
	vehicle *temp_vehicle
) {
	registry *last_vehicle_registry;

	if (parking == NULL) {
		sprintf(err, "%s: no such parking.\n", name);
		return;
	} else if (parking->free_spaces == 0) {
		sprintf(err, "%s: parking is full.\n", name);
		return;
	} else if (!is_licence_plate(license_plate)) {
		sprintf(err, "%s: invalid licence plate.\n", license_plate);
		return;
	}

	if (temp_vehicle != NULL) {
		last_vehicle_registry = temp_vehicle->last_reg;
		if (last_vehicle_registry == NULL ||
			last_vehicle_registry->type != EXIT) {
			sprintf(err, "%s: invalid vehicle entry.\n", license_plate);
			return;
		}
	}

	verify_date_registry(parking->last_reg, err, timestamp);
}

int run_s(char *args, park_index *parks, vehicle_index *vehicles) {
	char *name, license_plate[LICENSE_PLATE_SIZE + 1], err[MAX_LINE_BUFF] = {};
	date timestamp, start_timestamp;
	park *parking;
	vehicle *temp_vehicle;
	float cost;

	// Get the necessary arguments
	run_s_args(&args, &name, license_plate, &timestamp, &parking, parks);
	temp_vehicle = find_vehicle(license_plate, vehicles);

	// Error checking
	run_s_errochecking(
		parking, name, err, license_plate, &timestamp, temp_vehicle
	);

	if (err[0] != '\0') {
		printf("%s", err);
		free(name);
		return UNEXPECTED_INPUT;
	}

	start_timestamp = temp_vehicle->last_reg->registration->enter.timestamp;
	cost = calculate_cost(&start_timestamp, &timestamp, parking);
	register_exit(parking, &timestamp, temp_vehicle, &cost);

	printf(
		"%s %02d-%02d-%04d %02d:%02d %02d-%02d-%04d %02d:%02d %.2f\n",
		license_plate, start_timestamp.days, start_timestamp.months,
		start_timestamp.years, start_timestamp.hours, start_timestamp.minutes,
		timestamp.days, timestamp.months, timestamp.years, timestamp.hours,
		timestamp.minutes, cost
	);

	free(name);
	return SUCCESSFUL;
}

void run_s_args(
	char **args, char **name, char *license_plate, date *timestamp,
	park **parking, park_index *parks
) {
	int name_size;
	name_size = str_size(args);
	*name = parse_string(*args, args, &name_size);
	*args = parse_license_plate(*args, license_plate);
	*args = parse_date(*args, timestamp);
	*args = parse_time(*args, timestamp);
	timestamp->total_mins = date_to_minutes(timestamp);
	*parking = find_park(*name, hash(*name), parks);
}

void run_s_errochecking(
	park *parking, char *name, char *err, char *license_plate, date *timestamp,
	vehicle *temp_vehicle
) {
	registry *last_vehicle_registry;

	if (parking == NULL) {
		sprintf(err, "%s: no such parking.\n", name);
		return;
	} else if (!is_licence_plate(license_plate)) {
		sprintf(err, "%s: invalid licence plate.\n", license_plate);
		return;
	}

	if (temp_vehicle != NULL) {
		last_vehicle_registry = temp_vehicle->last_reg;
	} else {
		sprintf(err, "%s: invalid vehicle exit.\n", license_plate);
		return;
	}

	if (last_vehicle_registry == NULL || last_vehicle_registry->type != ENTER)
		sprintf(err, "%s: invalid vehicle exit.\n", license_plate);
	else if (last_vehicle_registry->registration->enter.park_ptr != parking)
		sprintf(err, "%s: invalid vehicle exit.\n", license_plate);

	verify_date_registry(parking->last_reg, err, timestamp);
}

int run_v(char *args, vehicle_index *vehicles) {
	char license_plate[9], err[MAX_LINE_BUFF] = {};
	license_plate[LICENSE_PLATE_SIZE] = '\0';
	vehicle *temp_vehicle;
	registry **non_null_regs = malloc(sizeof(registry *) * DEFAULT_CHUNK_SIZE);
	int count = 0;

	parse_license_plate(args, license_plate);
	temp_vehicle = find_vehicle(license_plate, vehicles);

	if (!is_licence_plate(license_plate)) {
		sprintf(err, "%s: invalid licence plate.\n", license_plate);
	} else if (temp_vehicle == NULL) {
		sprintf(err, "%s: no entries found in any parking.\n", license_plate);
	} else {
		count =
			get_non_null_registries(temp_vehicle->registries, &non_null_regs);
		if (count == 0)
			sprintf(
				err, "%s: no entries found in any parking.\n", license_plate
			);
	}

	if (err[0] != '\0') {
		printf("%s", err);
		free(non_null_regs);
		return UNEXPECTED_INPUT;
	}

	merge_sort(non_null_regs, 0, count - 1);
	show_all_regs(non_null_regs, temp_vehicle->last_reg, &count);

	free(non_null_regs);
	return SUCCESSFUL;
}

int run_f(char *args, park_index *parks) {
	char *name;
	date timestamp;
	park *parking;
	int name_size = str_size(&args);

	name = parse_string(args, &args, &name_size);
	parking = find_park(name, hash(name), parks);
	args = remove_whitespaces(args);

	if (parking == NULL) {
		printf("%s: no such parking.\n", name);
		free(name);
		return UNEXPECTED_INPUT;
	}

	if (*args == '\0') {
		show_billing(parking);
		free(name);
		return SUCCESSFUL;
	}

	args = parse_date(args, &timestamp);
	show_billing_day(parking, &timestamp);

	free(name);
	return SUCCESSFUL;
}

int run_r(char *args, park_index *parks) {
	int name_size, count, i;
	char *name, **names = malloc(sizeof(char *) * DEFAULT_CHUNK_SIZE);
	park *parking;

	name_size = str_size(&args);
	name = parse_string(args, &args, &name_size);
	parking = find_park(name, hash(name), parks);

	if (parking == NULL) {
		printf("%s: no such parking.\n", name);
		free(name);
		free(names);
		return UNEXPECTED_INPUT;
	}

	remove_park(parking, parks);

	count = get_park_names(parks, &names);
	merge_sort_names(names, 0, count - 1);

	for (i = 0; i < count; i++) {
		printf("%s\n", names[i]);
	}

	free(name);
	free(names);
	return SUCCESSFUL;
}

void verify_date_registry(registry *last_reg, char *err, date *timestamp) {
	if (last_reg != NULL) {
		if ((last_reg->type == ENTER &&
			 last_reg->registration->enter.timestamp.total_mins >
				 timestamp->total_mins) ||
			(last_reg->type == EXIT &&
			 last_reg->registration->exit.timestamp.total_mins >
				 timestamp->total_mins) ||
			!is_valid_date(timestamp)) {
			sprintf(err, "invalid date.\n");
		}
	}
}