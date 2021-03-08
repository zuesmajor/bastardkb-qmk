#include QMK_KEYBOARD_H
#include "sethBarberee.h"

extern backlight_config_t backlight_config;

enum layers {
    _QWERTY,
    _LOWER,
    _RAISE,
    _ADJUST
};

enum custom_keycodes {
  QWERTY = SAFE_RANGE,
  LOWER,
  RAISE,
  ADJUST,
};

#define KC_LOWR LOWER
#define KC_RASE RAISE
#define KC_RST RESET
#define KC_BL_S BL_STEP
#define KC_RTOG RGB_TOG
#define KC_RMOD RGB_MOD
#define KC_RHUI RGB_HUI
#define KC_RHUD RGB_HUD
#define KC_RSAI RGB_SAI
#define KC_RSAD RGB_SAD
#define KC_RVAI RGB_VAI
#define KC_RVAD RGB_VAD
#define KC_VK VLK_TOG

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [_QWERTY] = LAYOUT(
  //,----+----+----+----+----+----.              ,----+----+----+----+----+----.
     KC_ECAP, KC_1  , KC_2  , KC_3  , KC_4  , KC_5  ,                KC_6  , KC_7  , KC_8  , KC_9  , KC_0  ,KC_MINS,
  //|----+----+----+----+----+----|              |----+----+----+----+----+----|
     KC_TAB , KC_Q  , KC_W  , KC_E  , KC_R  , KC_T  ,                KC_Y  , KC_U  , KC_I  , KC_O  , KC_P  ,KC_DEL ,
  //|----+----+----+----+----+----|              |----+----+----+----+----+----|
     KC_LSFT, KC_A  , KC_S  , KC_D  , KC_F  , KC_G  ,                KC_H  , KC_J  , KC_K  , KC_L  ,KC_SCLN,KC_QUOT,
  //|----+----+----+----+----+----+----.    ,----|----+----+----+----+----+----|
     KC_LCTL, KC_Z  , KC_X  , KC_C  , KC_V  , KC_B  , KC_SPC,      KC_RASE, KC_N  , KC_M  ,KC_COMM,KC_DOT ,KC_SLSH,KC_RSFT,
  //`----+----+----+--+-+----+----+----/    \----+----+----+----+----+----+----'
                       KC_LGUI,KC_LOWR, KC_SPC,         KC_BSPC ,KC_ENT,KC_LALT
  //                  `----+----+----'        `----+----+----'
  ),

  [_LOWER] = LAYOUT(
  //,----+----+----+----+----+----.              ,----+----+----+----+----+----.
     KC_TILD,KC_EXLM, KC_AT ,KC_HASH,KC_DLR ,KC_PERC,               KC_CIRC,KC_AMPR,KC_ASTR,KC_LPRN,KC_RPRN,KC_BSPC,
  //|----+----+----+----+----+----|              |----+----+----+----+----+----|
     KC_RST ,_______,_______, KC_UP ,_______,_______,               KC_GRV , KC_P7 , KC_P8 , KC_P9 ,_______,_______,
  //|----+----+----+----+----+----|              |----+----+----+----+----+----|
     KC_DEL ,_______,KC_LEFT,KC_DOWN,KC_RGHT,KC_LBRC,               KC_RBRC, KC_P4 , KC_P5 , KC_P6 ,KC_PLUS,KC_PIPE,
  //|----+----+----+----+----+----+----.    ,----|----+----+----+----+----+----|
     KC_BL_S,_______,_______,_______,_______,KC_LCBR,KC_LPRN,     KC_RPRN,KC_RCBR, KC_P1 , KC_P2 , KC_P3 ,KC_MINS,_______,
  //`----+----+----+--+-+----+----+----/    \----+----+----+----+----+----+----'
                       _______,_______,KC_LPRN ,         KC_DEL ,_______, KC_P0
  //                  `----+----+----'        `----+----+----'
  ),

  [_RAISE] = LAYOUT(
  //,----+----+----+----+----+----.              ,----+----+----+----+----+----.
     KC_F12 , KC_F1 , KC_F2 , KC_F3 , KC_F4 , KC_F5 ,                KC_F6 , KC_F7 , KC_F8 , KC_F9 ,KC_F10 ,KC_F11 ,
  //|----+----+----+----+----+----|              |----+----+----+----+----+----|
     _______,_______,_______,_______,_______,_______,              _______, _______,_______,_______,_______,_______,
  //|----+----+----+----+----+----|              |----+----+----+----+----+----|
     _______,KC_MPRV,KC_MNXT,KC_VOLU,KC_PGUP,KC_UNDS,               KC_EQL ,KC_HOME,_______,_______,_______,KC_BSLS,
  //|----+----+----+----+----+----+----.    ,----|----+----+----+----+----+----|
     KC_MUTE,KC_MSTP,KC_MPLY,KC_VOLD,KC_PGDN,KC_MINS,_______,     _______,KC_PLUS,KC_END ,_______,_______,_______,_______,
  //`----+----+----+--+-+----+----+----/    \----+----+----+----+----+----+----'
                       _______,_______,_______,         _______,_______, _______
  //                  `----+----+----'        `----+----+----'
  ),

  [_ADJUST] = LAYOUT(
  //,----+----+----+----+----+----.              ,----+----+----+----+----+----.
     _______,_______,_______,_______,_______,_______,               _______,_______,_______,_______,_______,_______,
  //|----+----+----+----+----+----|              |----+----+----+----+----+----|
     KC_RTOG,KC_RMOD,KC_RHUI,KC_RSAI,KC_RVAI,_______,               _______,_______,_______,_______,_______,_______,
  //|----+----+----+----+----+----|              |----+----+----+----+----+----|
     KC_VK,  _______,KC_RHUD,KC_RSAD,KC_RVAD,_______,               _______,_______,_______,_______,_______,_______,
  //|----+----+----+----+----+----+----.    ,----|----+----+----+----+----+----|
     KC_BL_S,KC_RST ,_______,_______,_______,_______,_______,     _______,_______,_______,_______,_______,_______,_______,
  //`----+----+----+--+-+----+----+----/    \----+----+----+----+----+----+----'
                       _______,_______,_______,         _______,_______, _______
  //                  `----+----+----'        `----+----+----'
  )

};

void keyboard_pre_init_user(void) {
    // Make sure the red LEDs don't light
    setPinOutput(D5);
    writePinHigh(D5);

    setPinOutput(B0);
    writePinHigh(B0);
}

void keyboard_post_init_user(void){
    rgblight_enable_noeeprom(); // enable the RGBs
    rgblight_sethsv_noeeprom_red(); // set to red
    rgblight_mode_noeeprom(RGBLIGHT_MODE_BREATHING + 3); // set to breathing
}

void suspend_power_down_user(void){
    backlight_config.enable = false; // disable LED backlight
}

void suspend_wakeup_init_user(void){
    backlight_config.enable = true; // enable LED backlight
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case QWERTY:
      if (record->event.pressed) {
        set_single_persistent_default_layer(_QWERTY);
      }
      return false;
      break;
    case LOWER:
      if (record->event.pressed) {
        layer_on(_LOWER);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_LOWER);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      }
      return false;
      break;
    case RAISE:
      if (record->event.pressed) {
        layer_on(_RAISE);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      } else {
        layer_off(_RAISE);
        update_tri_layer(_LOWER, _RAISE, _ADJUST);
      }
      return false;
      break;
    case ADJUST:
      if (record->event.pressed) {
        layer_on(_ADJUST);
      } else {
        layer_off(_ADJUST);
      }
      return false;
      break;
  }
  return true;
}

uint32_t layer_state_set_user(uint32_t state){
    switch(biton32(state)) {
      case _QWERTY:
        rgblight_sethsv_noeeprom(HSV_RED);
        break;
      case _LOWER:
        rgblight_sethsv_noeeprom(HSV_GREEN);
        break;
      case _RAISE:
        rgblight_sethsv_noeeprom(HSV_BLUE);
        break;
      case _ADJUST:
        rgblight_sethsv_noeeprom(HSV_ORANGE);
        break;
      default:
        rgblight_mode_noeeprom(RGBLIGHT_MODE_STATIC_GRADIENT + 3);
        break;

    }
    return state;
}
