/**
 * @file commands.c
 * @author Diogo Santos (ist1110262)
 * @brief Implementation of the commands for a parking lot system.
 * @version 1
 * @date 27-03-2024
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "headers.h"

/**
 * @brief Creates a new parking lot or lists existing ones.
 *
 * @param buff Input buffer with park details.
 * @param parks Park index.
 * @return error_codes: SUCCESSFUL if park added, UNEXPECTED_INPUT if error.
 */
error_codes run_p(char *buff, park_index *parks) {
	p_args args = {.err = {}};

	// No arguments found.
	if (*buff == '\0') {
		show_parks(parks);
		return SUCCESSFUL;
	}

	// Get the necessary arguments.
	args.name_size = str_size(&buff);
	args.name = parse_string(buff, &buff, &(args.name_size));
	args.capacity = strtol(buff, &buff, 0);
	args.first_value = strtof(buff, &buff);
	args.value = strtof(buff, &buff);
	args.day_value = strtof(buff, &buff);

	// Error checking.
	if (find_park(args.name, hash(args.name), parks) != NULL) {
		sprintf(args.err, "%s: parking already exists.\n", args.name);
	} else if (args.capacity <= 0) {
		sprintf(args.err, "%i: invalid capacity.\n", args.capacity);
	} else if (args.first_value <= 0 // clang-format off.
				|| args.first_value > args.value 
				|| args.value > args.day_value) {
		sprintf(args.err, "invalid cost.\n"); // clang-format on.
	} else if (parks->park_num == MAX_PARKS) {
		sprintf(args.err, "too many parks.\n");
	}

	if (args.err[0] != '\0') {
		printf("%s", args.err);
		return UNEXPECTED_INPUT;
	}

	// Execute the command.
	add_park(&args, parks);

	return SUCCESSFUL;
}

/**
 * @brief Enters vehicle into park.
 *
 * @param buff Input buffer with vehicle details.
 * @param system System details structure.
 * @return error_codes: SUCCESSFUL if entrance registered, UNEXPECTED_INPUT if
 * error.
 */
error_codes run_e(char *buff, sys *system) {
	e_args args = {.err = {}};

	// Get the necessary arguments.
	args.name_size = str_size(&buff);
	args.name = parse_string(buff, &buff, &args.name_size);
	buff = parse_license_plate(buff, args.license_plate);
	buff = parse_date(buff, &(args.timestamp));
	buff = parse_time(buff, &(args.timestamp));
	args.timestamp.total_mins = date_to_minutes(&(args.timestamp));
	args.park = find_park(args.name, hash(args.name), &(system->parks));
	args.vehicle = find_vehicle(args.license_plate, &(system->vehicles));

	// Error checking.
	run_e_errochecking(&args, system);
	if (args.err[0] != '\0') {
		printf("%s", args.err);
		free(args.name);
		return UNEXPECTED_INPUT;
	}

	// Execute the command.
	system->sysdate = args.timestamp;
	register_entrance(&args, &(system->vehicles));
	printf("%s %i\n", args.name, (args.park)->free_spaces);

	free(args.name);
	return SUCCESSFUL;
}

/**
 * @brief Checks for errors in vehicle entry.
 *
 * @param args Arguments for the 'e' command.
 * @param system System details structure.
 */
void run_e_errochecking(e_args *args, sys *system) {
	registry *last_vehicle_reg;

	if (args->park == NULL) {
		sprintf(args->err, "%s: no such parking.\n", args->name);
		return;
	} else if (args->park->free_spaces == 0) {
		sprintf(args->err, "%s: parking is full.\n", args->name);
		return;
	} else if (!is_licence_plate(args->license_plate)) {
		sprintf(
			args->err, "%s: invalid licence plate.\n", args->license_plate
		);
		return;
	}

	if (args->vehicle != NULL) {
		last_vehicle_reg = args->vehicle->last_reg;
		if (last_vehicle_reg == NULL ||
			(last_vehicle_reg->type != EXIT &&
			 last_vehicle_reg->registration->enter.park_ptr != NULL)) {
			sprintf(
				args->err, "%s: invalid vehicle entry.\n", args->license_plate
			);
			return;
		}
	}

	verify_date_registry(&(system->sysdate), args->err, &(args->timestamp));
}

/**
 * @brief Registers vehicle exit from park.
 *
 * @param buff Input buffer with vehicle & park details.
 * @param system System details structure.
 * @return error_codes: SUCCESSFUL if exit registered, UNEXPECTED_INPUT if
 * error.
 */
error_codes run_s(char *buff, sys *system) {
	s_args args = {.err = {}};

	// Get the necessary arguments.
	run_s_args(&buff, &args, &(system->parks));
	args.vehicle = find_vehicle(args.license_plate, &(system->vehicles));

	// Error checking.
	run_s_errochecking(&args, &(system->sysdate));
	if (args.err[0] != '\0') {
		printf("%s", args.err);
		free(args.name);
		return UNEXPECTED_INPUT;
	}

	// Execute the command.
	args.start = args.vehicle->last_reg->registration->enter.timestamp;
	args.cost = calculate_cost(&args.start, &args.end, args.park);
	system->sysdate = args.end;
	register_exit(&args);

	printf(
		"%s %02d-%02d-%04d %02d:%02d %02d-%02d-%04d %02d:%02d %.2f\n",
		args.license_plate, args.start.days, args.start.months,
		args.start.years, args.start.hours, args.start.minutes, args.end.days,
		args.end.months, args.end.years, args.end.hours, args.end.minutes,
		args.cost
	);

	free(args.name);
	return SUCCESSFUL;
}

/**
 * @brief Parses arguments for vehicle exit.
 *
 * @param buff Input buffer with vehicle & park details.
 * @param args Arguments for the 's' command.
 * @param parks Park index.
 */
void run_s_args(char **buff, s_args *args, park_index *parks) {
	int name_size;
	name_size = str_size(buff);
	args->name = parse_string(*buff, buff, &name_size);
	*buff = parse_license_plate(*buff, args->license_plate);
	*buff = parse_date(*buff, &(args->end));
	*buff = parse_time(*buff, &(args->end));
	args->end.total_mins = date_to_minutes(&(args->end));
	args->park = find_park(args->name, hash(args->name), parks);
}

/**
 * @brief Checks for errors in vehicle exit.
 *
 * @param args Arguments for the 's' command.
 * @param sysdate System date.
 */
void run_s_errochecking(s_args *args, date *sysdate) {
	registry *last_vehicle_reg;

	if (args->park == NULL) {
		sprintf(args->err, "%s: no such parking.\n", args->name);
		return;
	} else if (!is_licence_plate(args->license_plate)) {
		sprintf(
			args->err, "%s: invalid licence plate.\n", args->license_plate
		);
		return;
	}

	if (args->vehicle != NULL) {
		last_vehicle_reg = args->vehicle->last_reg;
	} else {
		sprintf(args->err, "%s: invalid vehicle exit.\n", args->license_plate);
		return;
	}

	if (last_vehicle_reg == NULL || last_vehicle_reg->type != ENTER) {
		sprintf(args->err, "%s: invalid vehicle exit.\n", args->license_plate);
	} else if (last_vehicle_reg->registration->enter.park_ptr != args->park) {
		sprintf(args->err, "%s: invalid vehicle exit.\n", args->license_plate);
	} else {
		verify_date_registry(sysdate, args->err, &(args->end));
	}
}

/**
 * @brief Shows all registries for a vehicle.
 *
 * @param buff Input buffer with vehicle details.
 * @param vehicles Vehicle index.
 * @return error_codes: SUCCESSFUL on registries listed, UNEXPECTED_INPUT if
 * error.
 */
error_codes run_v(char *buff, vehicle_index *vehicles) {
	v_args args = {
		.err = {}, .non_null_regs = malloc(sizeof(registry *) * CHUNK_SIZE)};

	// Get the necessary arguments.
	parse_license_plate(buff, args.license_plate);
	args.vehicle = find_vehicle(args.license_plate, vehicles);

	// Error checking.
	run_v_errorchecking(&args);
	if (args.err[0] != '\0') {
		printf("%s", args.err);
		free(args.non_null_regs);
		return UNEXPECTED_INPUT;
	}

	// Execute the command.
	merge_sort(args.non_null_regs, 0, args.count - 1);
	show_all_regs(args.non_null_regs, args.vehicle->last_reg, &(args.count));

	free(args.non_null_regs);
	return SUCCESSFUL;
}

/**
 * @brief Checks for errors in listing vehicle registries.
 *
 * @param args Arguments for the 'v' command.
 */
void run_v_errorchecking(v_args *args) {
	if (!is_licence_plate(args->license_plate)) {
		sprintf(
			args->err, "%s: invalid licence plate.\n", args->license_plate
		);
	} else if (args->vehicle == NULL) {
		sprintf(
			args->err, "%s: no entries found in any parking.\n",
			args->license_plate
		);
	} else {
		args->count = get_non_null_registries(
			args->vehicle->registries, &(args->non_null_regs)
		);
		if (args->count == 0)
			sprintf(
				args->err, "%s: no entries found in any parking.\n",
				args->license_plate
			);
	}
}

/**
 * @brief Shows billing for a park generally or on specific day.
 *
 * @param buff Input buffer with park details.
 * @param system System details structure.
 * @return error_codes: SUCCESSFUL on billing listed, UNEXPECTED_INPUT if
 * error.
 */
error_codes run_f(char *buff, sys *system) {
	f_args args = {.err = {}, .timestamp = {.minutes = 0, .hours = 0}};

	// Get the necessary arguments.
	args.name_size = str_size(&buff);
	args.name = parse_string(buff, &buff, &(args.name_size));
	args.park = find_park(args.name, hash(args.name), &(system->parks));
	buff = remove_whitespaces(buff);

	if (args.park == NULL) {
		printf("%s: no such parking.\n", args.name);
		free(args.name);
		return UNEXPECTED_INPUT;
	}

	// No more arguments found.
	if (*buff == '\0') {
		show_billing(args.park);
		free(args.name);
		return SUCCESSFUL;
	}

	buff = parse_date(buff, &args.timestamp);
	args.timestamp.total_mins = date_to_minutes(&args.timestamp);

	// Error checking.
	if (!is_valid_date(&(args.timestamp)) ||
		args.timestamp.total_mins > system->sysdate.total_mins) {
		printf("invalid date.\n");
		free(args.name);
		return UNEXPECTED_INPUT;
	}

	// Execute the command.
	show_billing_day(args.park, &args.timestamp);

	free(args.name);
	return SUCCESSFUL;
}

/**
 * @brief Removes a park and lists remaining parks.
 *
 * @param buff Input buffer with park details.
 * @param parks Park index.
 * @return error_codes: SUCCESSFUL on park removed, UNEXPECTED_INPUT if error.
 */
error_codes run_r(char *buff, park_index *parks) {
	r_args args = {.names = malloc(sizeof(char *) * CHUNK_SIZE)};

	// Get the necessary arguments.
	args.name_size = str_size(&buff);
	args.name = parse_string(buff, &buff, &args.name_size);
	args.park = find_park(args.name, hash(args.name), parks);

	// Error checking.
	if (args.park == NULL) {
		printf("%s: no such parking.\n", args.name);
		free(args.name);
		free(args.names);
		return UNEXPECTED_INPUT;
	}

	// Execute the command.
	remove_park(args.park, parks);
	args.count = get_park_names(parks, &args.names);
	merge_sort_names(args.names, 0, args.count - 1);
	for (args.i = 0; args.i < args.count; args.i++) {
		printf("%s\n", args.names[args.i]);
	}

	free(args.name);
	free(args.names);
	return SUCCESSFUL;
}

/**
 * @brief Verifies if a date of a registry is valid and not in the past.
 *
 * @param sysdate System date.
 * @param err Error message.
 * @param timestamp Date to verify.
 */
void verify_date_registry(date *sysdate, char *err, date *timestamp) {
	if (!is_valid_date(timestamp) ||
		sysdate->total_mins > timestamp->total_mins) {
		sprintf(err, "invalid date.\n");
	}
}