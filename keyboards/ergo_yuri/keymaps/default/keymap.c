// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include "color.h"
#include "qp.h"
#include "generated/np2.qgf.h"
#include QMK_KEYBOARD_H

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_ortho_doxclone(
        KC_ESC,    KC_1, KC_2, KC_3, KC_4, KC_5,   KC_6,
        KC_TAB,    KC_Q, KC_W, KC_E, KC_R, KC_T,   KC_Y,
        KC_LALT,   KC_A, KC_S, KC_D, KC_F, KC_G,   KC_F4,
        KC_LSFT,   KC_Z, KC_X, KC_C, KC_V, KC_B,      KC_BSPC, MO(1),
        KC_LEFT, KC_DOWN, KC_UP, KC_RGHT, KC_LCTL,  KC_SPC, KC_ENT
    ),
    [1] = LAYOUT_ortho_doxclone(
        KC_ESC,    KC_F1, KC_F2, KC_F3, KC_F4, KC_F5,   KC_F6,
        KC_TAB,    KC_Q, KC_W, KC_E, KC_R, KC_T,   KC_Y,
        KC_LALT,   KC_A, KC_S, KC_D, KC_F, KC_G,   KC_H,
        KC_LSFT,   KC_Z, KC_X, KC_C, KC_V, KC_B,      KC_BSPC, XXXXXXX,
        KC_LEFT, KC_DOWN, KC_UP, QK_BOOT, EE_CLR,  XXXXXXX, XXXXXXX
    )
};

static painter_device_t display;
static painter_image_handle_t my_image;
static deferred_token my_anim;

void keyboard_post_init_kb(void) {
    display = qp_gc9a01_make_spi_device(240, 240, LCD_CS_PIN, LCD_DC_PIN, LCD_RST_PIN, 4, 0);         // Create the display
    qp_init(display, QP_ROTATION_0);   // Initialise the display
    qp_clear(display);
    my_image = qp_load_image_mem(gfx_np2);
    my_anim = qp_drawimage(display, 0, 0, my_image);
    // my_anim = qp_animate(display, 0, 0, my_image);
}