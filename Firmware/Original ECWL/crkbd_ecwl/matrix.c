/* Copyright 2020 sekigon-gonnoc
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "matrix.h"
#include "analog.h"
#include "quantum.h"
#include "split_util.h"
#include "transport.h"
#include "debounce.h"

#define POWER_PIN B4
#define APLEX_EN_PIN E6
#define OPA_SHDN_PIN F4
#define DISCHARGE_PIN B1
#define ANALOG_PORT F6

#define ERROR_TH 3000
#define HIGH_TH 300
#define LOW_TH 200

uint16_t sw_read[MATRIX_ROWS * MATRIX_COLS];

const uint8_t row_pins[] = {D4, C6, D7};
const uint8_t col_pins[] = {B3, B2, B6};
const uint8_t col_sels[] = {6, 4, 3, 0, 1, 2, 5};

#define LEN(x) (sizeof(x) / sizeof(x[0]))


#define ERROR_DISCONNECT_COUNT 5

#define ROWS_PER_HAND (MATRIX_ROWS / 2)

/* matrix state(1:on, 0:off) */
extern matrix_row_t raw_matrix[MATRIX_ROWS];  // raw values
extern matrix_row_t matrix[MATRIX_ROWS];      // debounced values

// row offsets for each hand
uint8_t thisHand, thatHand;

// user-defined overridable functions
__attribute__((weak)) void matrix_slave_scan_user(void) {}


void matrix_init_custom(void) {

    split_pre_init();

    // init row_pins
    for (int idx = 0; idx < LEN(row_pins); idx++) {
        setPinOutput(row_pins[idx]);
        writePinLow(row_pins[idx]);
    }

    // init col_pins
    for (int idx = 0; idx < LEN(col_pins); idx++) {
        setPinOutput(col_pins[idx]);
        writePinLow(col_pins[idx]);
    }

    // power_on
    setPinOutput(POWER_PIN);
    writePinHigh(POWER_PIN);

    // enable opeamp
    setPinOutput(OPA_SHDN_PIN);
    writePinHigh(OPA_SHDN_PIN);

    // enable multiplexer
    setPinOutput(APLEX_EN_PIN);
    writePinLow(APLEX_EN_PIN);

    // discharge pin
    writePinLow(DISCHARGE_PIN);
    setPinInput(DISCHARGE_PIN);

    // set analog reference
    analogReference(ADC_REF_POWER);

    thisHand = isLeftHand ? 0 : (ROWS_PER_HAND);
    thatHand = ROWS_PER_HAND - thisHand;

    split_post_init();
}

static void clear_all_row_pins(void) {
    for (int row = 0; row < LEN(row_pins); row++) {
        writePinLow(row_pins[row]);
    }
}

void matrix_post_scan(void) {
    if (is_keyboard_master()) {
        static uint8_t error_count;

        if (!transport_master(matrix + thatHand)) {
            error_count++;

            if (error_count > ERROR_DISCONNECT_COUNT) {
                // reset other half if disconnected
                for (int i = 0; i < ROWS_PER_HAND; ++i) {
                    matrix[thatHand + i] = 0;
                }
            }
        } else {
            error_count = 0;
        }

        matrix_scan_quantum();
    } else {
        transport_slave(matrix + thisHand);

        matrix_slave_scan_user();
    }
}


bool matrix_scan_custom(matrix_row_t current_matrix[]) {
    for (int col_sel_idx = 0; col_sel_idx < LEN(col_sels); col_sel_idx++) {
        // Discharge capacitor
        setPinOutput(DISCHARGE_PIN);

        writePinHigh(APLEX_EN_PIN);

        // select col
        for (int col_pin_idx = 0; col_pin_idx < LEN(col_pins); col_pin_idx++) {
            writePin(col_pins[col_pin_idx], col_sels[col_sel_idx] & (1 << col_pin_idx) ? 1 : 0);
        }

        writePinLow(APLEX_EN_PIN);

        // wait
        wait_us(30);

        for (int row = 0; row < sizeof(row_pins); row++) {
            // Discharge
            setPinOutput(DISCHARGE_PIN);
            wait_us(1);

            clear_all_row_pins();

            // critical section start
            cli();

            // drive lines
            setPinInput(DISCHARGE_PIN);
            writePinHigh(row_pins[row]);

            // read switch value
            // NOTE: sampling rate may be low to capture peak voltage
            sw_read[col_sel_idx + row * LEN(col_sels)] = analogReadPin(ANALOG_PORT);

            // critical section end
            sei();
        }
    }

    // Discharge
    setPinOutput(DISCHARGE_PIN);

    // select first col
    for (int col_pin_idx = 0; col_pin_idx < LEN(col_pins); col_pin_idx++) {
        writePin(col_pins[col_pin_idx], col_sels[0] & (1 << col_pin_idx) ? 1 : 0);
    }

    // clear all row
    clear_all_row_pins();

    uint8_t matrix_changed = 0;

    for (int row = 0; row < MATRIX_ROWS / 2; row++) {
        for (int col = 0; col < MATRIX_COLS; col++) {
            if (sw_read[col + row * LEN(col_sels)] > ERROR_TH) {
                // error value

                dprintf("ERROR(%d,%d):%d\n", row, col, sw_read[col + row * LEN(col_sels)]);

                sw_read[col + row * LEN(col_sels)] = 0;
            }

            if (current_matrix[row] & (1 << col)) {
                if (sw_read[col + row * LEN(col_sels)] < LOW_TH) {
                    current_matrix[row] &= ~(1 << col);
                    matrix_changed++;

                    dprintf("UP(%d,%d):%d\n", row, col, sw_read[col + row * LEN(col_sels)]);
                }
            } else {
                if (sw_read[col + row * LEN(col_sels)] > HIGH_TH) {
                    current_matrix[row] |= (1 << col);
                    matrix_changed++;
                    dprintf("DOWN(%d,%d):%d\n", row, col, sw_read[col + row * LEN(col_sels)]);
                }
            }
        }
    }

    static int cnt = 0;
    if (cnt++ == 300) {
        cnt = 0;
        for (int row = 0; row < sizeof(row_pins); row++) {
            for (int col_sel_idx = 0; col_sel_idx < LEN(col_sels);
                 col_sel_idx++) {
                dprintf("%5d", sw_read[col_sel_idx + row * LEN(col_sels)]);
            }
            dprintf("\n");
        }
        dprintf("\n");
    }

    return (matrix_changed > 0);
}

uint8_t matrix_scan(void) {
    bool changed = matrix_scan_custom(raw_matrix);

    debounce(raw_matrix, matrix + thisHand, ROWS_PER_HAND, changed);

    matrix_post_scan();
    return changed;
}
