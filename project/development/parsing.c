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

	return str + DATE_READ_SIZE;
}

char *parse_time(char *str, date *timestamp) {
	str = remove_whitespaces(str);
	sscanf(str, "%2d:%2d", &(timestamp->hours), &(timestamp->minutes));

	return str + HOUR_READ_SIZE;
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
	char *original_str = str;

	while (str < original_str + LICENSE_PLATE_SIZE) {

		if (isalpha(*str) && isalpha(*(str + 1))) {
			letter_pair = TRUE;
		} else if (isdigit(*str) && isdigit(*(str + 1))) {
			number_pair = TRUE;
		} else
			return FALSE;

		if (*(str + 2) != '-') break;

		str += 3;
	}

	return number_pair && letter_pair;
}

long int date_to_minutes(date *d) {
	long int minutes;

	minutes = (long int)d->years * DAYS_IN_YEAR * MINS_PER_DAY;

	for (int i = 0; i < (d->months - 1); i++) {
		minutes += days_in_month[i] * MINS_PER_DAY;
	}

	minutes += (d->days - 1) * MINS_PER_DAY;
	minutes += d->hours * 60;
	minutes += d->minutes;

	return minutes;
}

bool is_valid_date(date *d) {
	if (d->months < 1 || d->months > 12) return FALSE;
	if (d->days > days_in_month[d->months - 1] || d->days < 1) return FALSE;
	if (d->hours >= 24 || d->hours < 0) return FALSE;
	if (d->minutes >= 60 || d->minutes < 0) return FALSE;

	return TRUE;
}

float calculate_cost(date *start, date *end, park *parking) {
	int minutes_to_pay = (end->total_mins - start->total_mins), i;
	float cost, day_cost = 0;

	// Pay for the days
	cost = (minutes_to_pay / (MINS_PER_DAY)) * parking->day_value;
	minutes_to_pay = minutes_to_pay % (MINS_PER_DAY);

	// Pay for the first hour
	for (i = 0; i < TOTAL_INITIAL_BLOCKS; i++) {
		if (minutes_to_pay == 0) {
			break;
		} else if (minutes_to_pay <= PARK_PAY_STEP) {
			day_cost += parking->first_hour_value;
			minutes_to_pay = 0;
			break;
		}
		day_cost += parking->first_hour_value;
		minutes_to_pay -= PARK_PAY_STEP;
	}

	// Pay for the remaining hours
	day_cost += (minutes_to_pay / PARK_PAY_STEP) * parking->value;
	minutes_to_pay = (minutes_to_pay % PARK_PAY_STEP);

	if (minutes_to_pay != 0) {
		day_cost += parking->value;
	}

	// Pay only to the daily maximum allowed.
	if (day_cost > parking->day_value) {
		cost += parking->day_value;
	} else {
		cost += day_cost;
	}

	return cost;
}

bool is_same_day(date *d1, date *d2) {
	if ((d1->days == d2->days) && (d1->months == d2->months) &&
		(d1->years == d2->years))
		return TRUE;
	return FALSE;
}