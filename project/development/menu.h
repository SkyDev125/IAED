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

/// Displays the main menu and handles user input.
int menu();

/// Executes the command specified by the user.
int run_command(char *command, park_index *parks) ;

#endif