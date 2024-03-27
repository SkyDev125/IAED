/**
 * @file menu.h
 * @author Diogo Santos (ist1110262)
 * @brief Function declarations for the menu interface.
 * @version 0.1
 * @date 10-03-2024
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef MENU
#define MENU

/// @defgroup menu_functions Menu & command related functions.
/// @{

/// Displays the main menu and handles user input.
error_codes menu();

/// Executes the command specified by the user.
error_codes run_command(sys *system);

/// @}

#endif