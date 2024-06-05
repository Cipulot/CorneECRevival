/*
Copyright 2022 Cipulot

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "config_common.h"

/* USB Device descriptor parameter */
#define VENDOR_ID    0xFEED
#define PRODUCT_ID   0x6B71
#define DEVICE_VER   0x0001
#define MANUFACTURER    Cipulot
#define PRODUCT         Corne EC Revival
#define DESCRIPTION     A split capacitive/true-wireless keyboard

/* key matrix size */
#define MATRIX_ROWS 6
#define MATRIX_COLS 7

/*
 * Keyboard Matrix Assignments
 *
 * Change this to how you wired your keyboard
 * COLS: AVR pins used for columns, left to right
 * ROWS: AVR pins used for rows, top to bottom
 * DIODE_DIRECTION: COL2ROW = COL = Anode (+), ROW = Cathode (-, marked on diode)
 *                  ROW2COL = ROW = Anode (+), COL = Cathode (-, marked on diode)
 *
 */
#define POWER_PIN B4
#define APLEX_EN_PIN E6
#define OPA_SHDN_PIN F4
#define DISCHARGE_PIN B1
#define ANALOG_PORT F6

#define MATRIX_ROW_PINS { D4, C6, D7 }
#define MATRIX_COL_CHANNELS { 6, 4, 3, 0, 1, 2, 5 }
#define MUX_SEL_PINS { B3, B2, B6 }

#define UNUSED_PINS
/*
#define MATRIX_ROW_PINS { 0 } // Use custom matrix.c
#define MATRIX_COL_PINS { 0 } // Use custom matrix.c
#define UNUSED_PINS
*/

/* COL2ROW, ROW2COL */
#define DIODE_DIRECTION COL2ROW

/*
 * Split Keyboard specific options, make sure you have 'SPLIT_KEYBOARD = yes' in your rules.mk, and define SOFT_SERIAL_PIN.
 */

#define USE_SERIAL
#define SOFT_SERIAL_PIN D2  // or D1, D2, D3, E6
#define MASTER_LEFT


/* Debounce reduces chatter (unintended double-presses) - set 0 if debouncing is not needed */
#define DEBOUNCE 5

/* Mechanical locking support. Use KC_LCAP, KC_LNUM or KC_LSCR instead in keymap */
#define LOCKING_SUPPORT_ENABLE
/* Locking resynchronize hack */
#define LOCKING_RESYNC_ENABLE

/* disable these deprecated features by default */
#define NO_ACTION_MACRO
#define NO_ACTION_FUNCTION
