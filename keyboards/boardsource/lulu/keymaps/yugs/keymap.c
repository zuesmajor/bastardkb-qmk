// Copyright 2022 Cole Smith <cole@boadsource.xyz>
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

enum layers {
    _QWERTY,
    _RAISE,
    _LOWER,
    _ADJUST
};
#include "oled.c"

#define RAISE MO(_RAISE)
#define LOWER MO(_LOWER)
#define SEND_LOWER TG(_LOWER)
#define SEND_RAISE TG(_RAISE)

// Tap Dance declarations
enum {
    TD_SHIFT_CAPS = 0
};

// Tap Dance definitions
qk_tap_dance_action_t tap_dance_actions[] = {
    // Tap once for Escape, twice for Caps Lock
    [TD_SHIFT_CAPS] = ACTION_TAP_DANCE_DOUBLE(KC_LSFT, KC_CAPS),
};


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* QWERTY
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | ESC  |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  | BackSP|
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Tab  |   Q  |   W  |   E  |   R  |   T  |                    |   Y  |   U  |   I  |   O  |   P  |  -   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |LCTRL |   A  |   S  |   D  |   F  |   G  |-------.    ,-------|   H  |   J  |   K  |   L  |   ;  |  '   |
 * |------+------+------+------+------+------|   [   |    |    ]  |------+------+------+------+------+------|
 * |LShift|   Z  |   X  |   C  |   V  |   B  |-------|    |-------|   N  |   M  |   ,  |   .  |   /  |RShift|
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *                   | LAlt | SLOWER |LGUI | /Space  /       \Enter \  |SRAISE | RCTRL | RGUI |
 *                   |      |      |      |/       /         \      \ |      |      |      |
 *                   `----------------------------'           '------''--------------------'
 */

 [_QWERTY] = LAYOUT(
  KC_ESC,   KC_1,   KC_2,    KC_3,    KC_4,    KC_5,                     KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC,
  KC_TAB,   KC_Q,   KC_W,    KC_E,    KC_R,    KC_T,                     KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_MINS,
  KC_LCTL, KC_A,   KC_S,    KC_D,    KC_F,    KC_G,                     KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
  TD(TD_SHIFT_CAPS),  KC_Z,   KC_X,    KC_C,    KC_V,    KC_B, KC_LBRC,  KC_RBRC,  KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,  KC_RSFT,
                          KC_LALT,    SEND_LOWER, KC_LGUI, KC_SPC,  KC_ENT,   SEND_RAISE,   KC_RCTL, KC_RGUI
),
/* LOWER
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |   F1   |   F2   |   F3   |   F4   |  F5 |                    |   ^  |   \  |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |  F1  |  (   |  UP  |  )   |  F5  |  F6  |                    |  @   |  #   |  $   |   %  |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |   `  | LEFT | DOWN | RIGHT|   $  |   %  |-------.    ,-------|   `  |   (  |   )  |   ~  |      |      |
 * |------+------+------+------+------+------|   [   |    |    ]  |------+------+------+------+------+------|
 * |      |      |      |      |      |      |-------|    |-------|   =   |   +  |   <  |   >  |   }  |   |  |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *                   | LAlt | LGUI |LOWER | /Space  /       \Enter \  |      |      |      |
 *                   |      |      |      |/       /         \      \ |      |      |      |
 *                   `----------------------------'           '------''--------------------'
 */
[_LOWER] = LAYOUT(
  KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6,                               KC_CIRC, KC_BSLS, _______,_______, _______, _______,
  KC_F1,   KC_LPRN,   KC_UP,   KC_RPRN,   KC_F5,   KC_F6,                     KC_AT,   KC_HASH,   KC_DLR,   KC_PERC,  _______,  _______,
  KC_CIRC, KC_LEFT, KC_DOWN,   KC_RGHT, KC_DLR,  KC_PERC,                   KC_GRV, KC_LPRN, KC_RPRN, KC_TILD, _______, _______,
  _______, _______, _______, _______, _______, _______, _______, _______, KC_EQL, KC_PLUS, KC_LT, KC_GT, _______, _______,
                             _______, _______, SEND_LOWER, _______, _______,  _______, _______, _______
),
/* RAISE
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |MOD   | MODR |  S+  |  S-  |  B+  |  B-  |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |   `  |   (  |  UP  |   )  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |  LEFT  |  DOWN  |  RIGHT  |      |  F6  |             -------.    ,-------|      | Left | Down |  Up  |Right |      |
 * |------+------+------+------+------+------|   [  |    |    ]  |------+------+------+------+------+------|
 * |LShift |  F8  |  F9  | F10  | F11  | F12  |-------|    |-------|   +  |   -  |   =  |   [  |   ]  | RShift|
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *                   | LAlt | LLOCK|LOWER | /Space  /       \Enter \  |RAISE |BackSP| RGUI |
 *                   |      |      |      |/       /         \      \ |      |      |      |
 *                   `----------------------------'           '------''--------------------'
 */

[_RAISE] = LAYOUT(
  RGB_MOD, RGB_RMOD,RGB_SAI, RGB_SAD, RGB_VAI, RGB_VAD,                     _______, _______, _______, _______, _______, _______,
  KC_GRV,  KC_LPRN,    KC_UP,    KC_RPRN,    KC_4,    KC_5,                        KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    _______,
  _______,   KC_LEFT,   KC_DOWN,   KC_RGHT ,   _______,   KC_F6,                       XXXXXXX, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, XXXXXXX,
  KC_LSFT,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,   KC_LBRC, KC_RBRC,  KC_PLUS, KC_MINS, KC_EQL,  KC_LBRC, KC_RBRC, KC_RSFT,
                             _______, _______, _______,  _______, _______,  SEND_RAISE, _______, _______
),
/* ADJUST
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |      |      |      |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |      |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |      |      |      |      |-------.    ,-------|      |      |RGB ON| HUE+ | SAT+ | VAL+ |
 * |------+------+------+------+------+------|       |    |       |------+------+------+------+------+------|
 * |      |      |      |      |      |      |-------|    |-------|      |      | MODE | HUE- | SAT- | VAL- |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *                   | LAlt | LGUI |LOWER | /Space  /       \Enter \  |RAISE |BackSP| RGUI |
 *                   |      |      |      |/       /         \      \ |      |      |      |
 *                   `----------------------------'           '------''--------------------'
 */
  [_ADJUST] = LAYOUT(
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                             _______, _______, _______, _______, _______,  _______, _______, _______
  )
};

layer_state_t layer_state_set_user(layer_state_t state) {
   return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
}
