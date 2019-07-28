/* Copyright 2019 RoguePullRequest
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
#include QMK_KEYBOARD_H

typedef struct {
	bool is_press_action;
	int state;
} tap;

enum {
	ESC_REST = SAFE_RANGE
};

enum {
	SINGLE_TAP = 1,
	SINGLE_HOLD = 2,
};

int cur_dance (qk_tap_dance_state_t *state);

void esc_finished (qk_tap_dance_state_t *state);
void esc_reset (qk_tap_dance_state_t *state);

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT( /* Base */
	ESC_REST, KC_TAB, KC_EQUAL, KC_BSPC,

	MT(MOD_RSFT,KC_NUMLOCK), KC_KP_SLASH, KC_KP_ASTERISK, KC_KP_MINUS,
	KC_KP_7, KC_KP_8, KC_KP_9, 		KC_KP_PLUS,
	KC_KP_4, KC_KP_5, KC_KP_6,
	KC_KP_1, KC_KP_2, KC_KP_3,		KC_KP_ENTER,
	KC_KP_0,	KC_KP_DOT
  ),
};

static tap esctap_state = {
	.is_press_action = true,
	.state = 0
};

void esc_finished (qk_tap_dance_state_t *state, void *user_data) {
	esctap_state.state = cur_dance(state);
	switch (esctap_state.state) {
		case SINGLE_TAP: register_code16(KC_ESC): break;
		case SINGLE_HOLD: reset_keyboard(); break;
	}
}

void esc_reset (qk_tap_dance_state_t *state, void *user_data) {
	switch (esctap_state.state) {
		case SINGLE_TAP: unregister_code16(KC_ESC); break;
		case SINGLE_HOLD: reset_keyboard(); break;
	}
	esctap_state.state = 0;
}


// TAP Dance Definition
qk_tap_dance_action_t tap_dance_actions[] = { [ESC_REST] = ACTION_TAP_DANCE_DOUBLE(KC_ESC, RESET) };

void shutdown_user(void) { clear_keyboard(); }
