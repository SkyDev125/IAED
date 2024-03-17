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

const int days_in_month[] = {JAN, FEB, MAR, APR, MAY, JUN,
							 JUL, AUG, SEP, OCT, NOV, DEC};

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

	if ((int)*current == '\0') current--;

	return current;
}

char *parse_string(char *str_start, char **str_end, int *size) {
	char *str = malloc(sizeof(char) * (*size + 1));

	strncpy(str, str_start, *size);
	str[*size] = '\0';
	*str_end += *size + 1;

	return str;
}

char *parse_license_plate(char *str, char *license_plate) {
	str = remove_whitespaces(str);
	strncpy(license_plate, str, LICENSE_PLATE_SIZE);
	license_plate[LICENSE_PLATE_SIZE] = '\0';

	return str + LICENSE_PLATE_SIZE;
}

char *parse_date(char *str, date *timestamp) {
	str = remove_whitespaces(str);
	sscanf(
		str, "%2d-%2d-%4d", &(timestamp->days), &(timestamp->months),
		&(timestamp->years)
	);

	return str + DATE_MAX_SIZE;
}

char *parse_time(char *str, date *timestamp) {
	str = remove_whitespaces(str);
	sscanf(str, "%2d:%2d", &(timestamp->hours), &(timestamp->minutes));

	return str + HOUR_MAX_SIZE;
}

int str_size(char **args) {
	char *end_args;
	if ((*args)[0] != '"')
		end_args = delimit(*args, ' ');
	else
		end_args = delimit(++(*args), '"');

	return end_args - *args;
}

bool is_licence_plate(char *str) {
	bool number_pair = FALSE, letter_pair = FALSE;

	while (str < str + LICENSE_PLATE_SIZE) {
		if (*(str + 2) != '-' && str + 2 < str + LICENSE_PLATE_SIZE)
			return FALSE;

		if (isalpha(*str) && isalpha(*(str + 1))) {
			letter_pair = TRUE;
		} else if (isdigit(*str) && isdigit(*(str + 1))) {
			number_pair = TRUE;
		} else
			return FALSE;

		str += 3;
	}

	return number_pair && letter_pair;
}

long int date_to_minutes(date *d) {
	long int minutes;

	minutes = d->years * DAYS_IN_YEAR * MINS_PER_DAY;

	for (int i = 0; i < d->months - 1; i++) {
		minutes += days_in_month[i] * MINS_PER_DAY;
	}

	minutes += (d->days - 1) * MINS_PER_DAY;
	minutes += d->hours * 60;
	minutes += d->minutes;

	return minutes;
}