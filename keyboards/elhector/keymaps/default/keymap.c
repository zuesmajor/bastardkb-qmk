#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_numpad_6x4(
        KC_LPRN,   KC_RPRN,   KC_TILDE,   KC_DLR,
        KC_SLASH,   KC_KP_ASTERISK,   KC_KP_PLUS,   KC_KP_MINUS,
        KC_P7,   KC_P8,   KC_P9,   KC_DELETE,
        KC_P4,   KC_P5,   KC_P6,   KC_KP_EQUAL,
        KC_P1,   KC_P2,   KC_P3,   KC_KP_ENTER,
        KC_P0,   KC_PDOT
    )
};
