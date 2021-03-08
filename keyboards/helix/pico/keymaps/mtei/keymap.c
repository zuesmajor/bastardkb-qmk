#include QMK_KEYBOARD_H
#include "bootloader.h"
#ifdef PROTOCOL_LUFA
#include "lufa.h"
#include "split_util.h"
#endif
#ifdef AUDIO_ENABLE
  #include "audio.h"
#endif
#ifdef SSD1306OLED
  #include "ssd1306.h"
#endif
#ifdef CONSOLE_ENABLE
  #include <print.h>
#endif

extern keymap_config_t keymap_config;

#ifdef RGBLIGHT_ENABLE
//Following line allows macro to read current RGB settings
extern rgblight_config_t rgblight_config;
#endif

extern uint8_t is_master;

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
enum layer_number {
    _QWERTY = 0,
    _COLEMAK,
    _DVORAK,
    _EUCALYN,
    _NUML,
    _NUMR,
    _LOWER,
    _RAISE,
    _ADJUST,
};

enum custom_keycodes {
  QWERTY = SAFE_RANGE,
  COLEMAK,
  DVORAK,
  EUCALYN,
  NUML,
  NUMR,
  KC_xEISU,
  KC_xKANA,
  KC_ZERO2,
  RGBRST
};

//Macros
#define KC_LOWER  MO(_LOWER)
#define KC_NUML   LT(_NUML,KC_SPC)
#define KC_NUMR   LT(_NUMR,KC_SPC)
#define KC_RABS   LT(_RAISE,KC_BSPC)
#define KC_RAEN   LT(_RAISE,KC_ENT)
#define KC_ADJ    MO(_ADJUST)
#define KC_LSMI   LSFT(KC_MINS)
#define KC_LSEQ   LSFT(KC_EQL)
#define KC_LSRB   LSFT(KC_RBRC)
#define KC_LSLB   LSFT(KC_LBRC)
#define KC_LSGR   LSFT(KC_GRV)
#define KC_LSQT   LSFT(KC_QUOT)
#define KC_S0     LSFT(KC_0)
#define KC_S1     LSFT(KC_1)
#define KC_S2     LSFT(KC_2)
#define KC_S3     LSFT(KC_3)
#define KC_S4     LSFT(KC_4)
#define KC_S5     LSFT(KC_5)
#define KC_S6     LSFT(KC_6)
#define KC_S7     LSFT(KC_7)
#define KC_S8     LSFT(KC_8)
#define KC_S9     LSFT(KC_9)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  /* Qwerty
   * ,-----------------------------------------.             ,-----------------------------------------.
   * | ESC  |   Q  |   W  |   E  |   R  |   T  |             |   Y  |   U  |   I  |   O  |   P  |  \   |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * | Ctrl |   A  |   S  |   D  |   F  |   G  |             |   H  |   J  |   K  |   L  |   ;  | Ctrl |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * | Shift|   Z  |   X  |   C  |   V  |   B  |             |   N  |   M  |   ,  |   .  |   /  | Shift|
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * |Lower | Lower| Caps |  Alt |  GUI | Space|  BS  | Enter| Space| GUI  | Alt  | Menu |Lower |Lower |
   * `-------------------------------------------------------------------------------------------------'
   */
  [_QWERTY] = LAYOUT( \
      KC_ESC,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                  KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,  KC_BSLS, \
      KC_LCTL,   KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                  KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN,  KC_RCTL, \
      KC_LSFT,   KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                  KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH,  KC_RSFT, \
      KC_LOWER,KC_LOWER, KC_CAPS, KC_LALT, KC_LGUI,KC_NUML, KC_RABS,   KC_RAEN,KC_NUMR, KC_RGUI, KC_RALT,  KC_APP,KC_LOWER,  KC_LOWER \
      ),

  /* Colemak
   * ,-----------------------------------------.             ,-----------------------------------------.
   * | ESC  |   Q  |   W  |   F  |   P  |   G  |             |   J  |   L  |   U  |   Y  |   ;  | \    |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * | Ctrl |   A  |   R  |   S  |   T  |   D  |             |   H  |   N  |   E  |   I  |   O  | Ctrl |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * | Shift|   Z  |   X  |   C  |   V  |   B  |             |   K  |   M  |   ,  |   .  |   /  | Shift|
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * |Lower | Lower| Caps |  Alt |  GUI | Space|  BS  | Enter| Space| GUI  | Alt  | Menu |Lower |Lower |
   * `-------------------------------------------------------------------------------------------------'
   */
  [_COLEMAK] = LAYOUT( \
      KC_ESC,    KC_Q,    KC_W,    KC_F,    KC_P,    KC_G,                  KC_J,    KC_L,    KC_U,    KC_Y, KC_SCLN,  KC_BSLS, \
      KC_LCTL,   KC_A,    KC_R,    KC_S,    KC_T,    KC_D,                  KC_H,    KC_N,    KC_E,    KC_I,    KC_O,  KC_RCTL, \
      KC_LSFT,   KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                  KC_K,    KC_M, KC_COMM,  KC_DOT, KC_SLSH,  KC_RSFT, \
      KC_LOWER,KC_LOWER, KC_CAPS, KC_LALT, KC_LGUI,KC_NUML, KC_RABS,   KC_RAEN,KC_NUMR, KC_RGUI, KC_RALT,  KC_APP,KC_LOWER,  KC_LOWER \
      ),

  /* Dvorak
   * ,-----------------------------------------.             ,-----------------------------------------.
   * | ESC  |   '  |   ,  |   .  |   P  |   Y  |             |   F  |   G  |   C  |   R  |   L  |  \   |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * | Ctrl |   A  |   O  |   E  |   U  |   I  |             |   D  |   H  |   T  |   N  |   S  | Ctrl |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * | Shift|   ;  |   Q  |   J  |   K  |   X  |             |   B  |   M  |   W  |   V  |   Z  | Shift|
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * |Lower | Lower| Caps |  Alt |  GUI | Space|  BS  | Enter| Space| GUI  | Alt  | Menu |Lower |Lower |
   * `-------------------------------------------------------------------------------------------------'
   */
  [_DVORAK] = LAYOUT( \
      KC_ESC, KC_QUOT, KC_COMM,  KC_DOT,    KC_P,    KC_Y,                  KC_F,    KC_G,    KC_C,    KC_R,    KC_L,  KC_BSLS, \
      KC_LCTL,   KC_A,    KC_O,    KC_E,    KC_U,    KC_I,                  KC_D,    KC_H,    KC_T,    KC_N,    KC_S,  KC_RCTL, \
      KC_LSFT, KC_SCLN,   KC_Q,    KC_J,    KC_K,    KC_X,                  KC_B,    KC_M,    KC_W,    KC_V,    KC_Z,  KC_RSFT, \
      KC_LOWER,KC_LOWER, KC_CAPS, KC_LALT, KC_LGUI,KC_NUML, KC_RABS,   KC_RAEN,KC_NUMR, KC_RGUI, KC_RALT,  KC_APP,KC_LOWER,  KC_LOWER \
      ),

  /* Eucalyn (http://eucalyn.hatenadiary.jp/entry/about-eucalyn-layout)
   * ,-----------------------------------------.             ,-----------------------------------------.
   * | ESC  |   Q  |   W  |   ,  |   .  |   ;  |             |   M  |   R  |   D  |   Y  |   P  |  \   |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * | Ctrl |   A  |   O  |   E  |   I  |   U  |             |   G  |   T  |   K  |   S  |   N  | Ctrl |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * | Shift|   Z  |   X  |   C  |   V  |   F  |             |   B  |   H  |   J  |   L  |   /  | Shift|
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * |Lower | Lower| Caps |  Alt |  GUI | Space|  BS  | Enter| Space| GUI  | Alt  | Menu |Lower |Lower |
   * `-------------------------------------------------------------------------------------------------'
   */
  [_EUCALYN] = LAYOUT( \
      KC_ESC,    KC_Q,    KC_W, KC_COMM,  KC_DOT, KC_SCLN,                  KC_M,    KC_R,    KC_D,    KC_Y,    KC_P,  KC_BSLS, \
      KC_LCTL,   KC_A,    KC_O,    KC_E,    KC_I,    KC_U,                  KC_G,    KC_T,    KC_K,    KC_S,    KC_N,  KC_RCTL, \
      KC_LSFT,   KC_Z,    KC_X,    KC_C,    KC_V,    KC_F,                  KC_B,    KC_H,    KC_J,    KC_L, KC_SLSH,  KC_RSFT, \
      KC_LOWER,KC_LOWER, KC_CAPS, KC_LALT, KC_LGUI,KC_NUML, KC_RABS,   KC_RAEN,KC_NUMR, KC_RGUI, KC_RALT,  KC_APP,KC_LOWER,  KC_LOWER \
      ),

  /* Num
   * ,-----------------------------------------.             ,-----------------------------------------.
   * |      |   !  |   @  |   #  |   $  |   %  |             |   ^  |   &  |   *  |   (  |   )  |      |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * |      |   1  |   2  |   3  |   4  |   5  |             |   6  |   7  |   8  |   9  |   0  | F12  |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * |      |  F1  |  F2  |  F3  |  F4  |  F5  |             |  F6  |  F7  |  F8  |  F9  |  F10 | F11  |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * |      |      |      |      |      |      |      |      |      |      |      |      |      |      |
   * `-------------------------------------------------------------------------------------------------'
   */
  [_NUML] = LAYOUT( \
      _______,   KC_S1,   KC_S2,   KC_S3,   KC_S4,   KC_S5,               KC_S6,   KC_S7,   KC_S8,   KC_S9,   KC_S0, _______, \
      _______,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                KC_6,    KC_7,    KC_8,    KC_9,    KC_0,  KC_F12, \
      _______,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,               KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10,  KC_F11, \
      _______, _______, _______, _______, _______, _______, _______, _______,  KC_SPC, _______, _______, _______, _______, _______ \
      ),

  [_NUMR] = LAYOUT( \
      _______,   KC_S1,   KC_S2,   KC_S3,   KC_S4,   KC_S5,               KC_S6,   KC_S7,   KC_S8,   KC_S9,   KC_S0, _______, \
      _______,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                KC_6,    KC_7,    KC_8,    KC_9,    KC_0,  KC_F12, \
      _______,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,               KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10,  KC_F11, \
      _______, _______, _______, _______, _______,  KC_SPC, _______, _______, _______, _______, _______, _______, _______, _______ \
      ),

  /* Lower
   * ,-----------------------------------------.             ,-----------------------------------------.
   * |      |      |Pause | ScrLk| Ins  |      |             |      | Ins  | ScrLk|Pause |      |      |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * |      | PgUp |      | Up   |Delete| Home |             | Home |Delete| Up   |      | PgUp |      |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * |      | PgDn | Left | Down | Right| End  |             | End  | Left | Down | Right| PgDn |      |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * |      |      | PrtSc|      |      |      |Adjust|Adjust|      |      |      | PrtSc|      |      |
   * `-------------------------------------------------------------------------------------------------'
   */
  [_LOWER] = LAYOUT( \
      XXXXXXX, XXXXXXX, KC_PAUS, KC_SLCK, KC_INS,  XXXXXXX,             XXXXXXX,  KC_INS, KC_SLCK, KC_PAUS, XXXXXXX, _______, \
      _______, KC_HOME, XXXXXXX, KC_UP,   KC_DEL,  KC_PGUP,             KC_PGUP,  KC_DEL,   KC_UP, XXXXXXX, KC_HOME, _______, \
      _______, KC_END,  KC_LEFT, KC_DOWN, KC_RGHT, KC_PGDN,             KC_PGDN, KC_LEFT, KC_DOWN, KC_RGHT,  KC_END, _______, \
      _______, _______, KC_PSCR, _______, _______, _______,  KC_ADJ,  KC_ADJ, _______, _______, _______, KC_PSCR, _______, _______ \
      ),

  /* Raise
   * ,-----------------------------------------.             ,-----------------------------------------.
   * | TAB  |      |      |      |  _   |  -   |             |  =   |  +   |      |      |      |      |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * | EISU |      |      |      |  {   |  [   |             |  ]   |  }   |      |      |      | KANA |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * |      |      |      |      |  ~   |  `   |             |  '   |   "  |      |      |      |      |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * |      |      |      |      |      |      |      |      |      |      |      |      |      |      |
   * `-------------------------------------------------------------------------------------------------'
   */
  [_RAISE] = LAYOUT( \
       KC_TAB, XXXXXXX, XXXXXXX, XXXXXXX, KC_LSMI, KC_MINS,              KC_EQL, KC_LSEQ, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
     KC_xEISU, XXXXXXX, XXXXXXX, XXXXXXX, KC_LSLB, KC_LBRC,             KC_RBRC, KC_LSRB, XXXXXXX, XXXXXXX, XXXXXXX, KC_xKANA, \
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_LSGR,  KC_GRV,             KC_QUOT, KC_LSQT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
      KC_ADJ,   KC_ADJ, XXXXXXX, _______, _______, XXXXXXX, _______, _______, XXXXXXX, _______, _______, XXXXXXX,  KC_ADJ,  KC_ADJ \
      ),

  /* Adjust (Lower + Raise)
   * ,-----------------------------------------.             ,-----------------------------------------.
   * |      | Reset|RGBRST|RGB ON|Aud on| Win  |             | Win  |      |Qwerty|Euclyn|Colemk|Dvorak|
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * | HUE+ | SAT+ | VAL+ |RGB md|Audoff| Mac  |             | Mac  |      |      |      |      |      |
   * |------+------+------+------+------+------|             |------+------+------+------+------+------|
   * | HUE- | SAT- | VAL- |      |      |      |             |      |      |      |      |      |      |
   * |------+------+------+------+------+------+------+------+------+------+------+------+------+------|
   * |      |      |      |      |      |      |      |      |      |      |      |      |      |      |
   * `-------------------------------------------------------------------------------------------------'
   */
  [_ADJUST] =  LAYOUT( \
      XXXXXXX, RESET,   RGBRST,  RGB_TOG,   AU_ON, AG_SWAP,          AG_SWAP, XXXXXXX, QWERTY,  EUCALYN, COLEMAK,  DVORAK, \
      RGB_HUI, RGB_SAI, RGB_VAI, RGB_MOD,  AU_OFF, AG_NORM,          AG_NORM, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
      RGB_HUD, RGB_SAD, RGB_VAD, XXXXXXX, XXXXXXX, XXXXXXX,          XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
      _______, _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______,_______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______, _______ \
   )
};

#ifdef AUDIO_ENABLE

float tone_qwerty[][2]     = SONG(QWERTY_SOUND);
float tone_dvorak[][2]     = SONG(DVORAK_SOUND);
float tone_colemak[][2]    = SONG(COLEMAK_SOUND);
float tone_plover[][2]     = SONG(PLOVER_SOUND);
float tone_plover_gb[][2]  = SONG(PLOVER_GOODBYE_SOUND);
float music_scale[][2]     = SONG(MUSIC_SCALE_SOUND);
#endif

static int current_default_layer;

uint32_t default_layer_state_set_kb(uint32_t state) {
    // 1<<_QWERTY  - 1 == 1 - 1 == _QWERTY (=0)
    // 1<<_COLEMAK - 1 == 2 - 1 == _COLEMAK (=1)
    current_default_layer = state - 1;
    // 1<<_DVORAK  - 2 == 4 - 2 == _DVORAK (=2)
    if ( current_default_layer == 3 ) current_default_layer -= 1;
    // 1<<_EUCALYN - 5 == 8 - 5 == _EUCALYN (=3)
    if ( current_default_layer == 7 ) current_default_layer -= 4;
    return state;
}

void update_base_layer(int base)
{
    if( current_default_layer != base ) {
        eeconfig_update_default_layer(1UL<<base);
        default_layer_set(1UL<<base);
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case QWERTY:
      if (record->event.pressed) {
        #ifdef AUDIO_ENABLE
          PLAY_SONG(tone_qwerty);
        #endif
        update_base_layer(_QWERTY);
      }
      return false;
      break;
    case COLEMAK:
      if (record->event.pressed) {
        #ifdef AUDIO_ENABLE
          PLAY_SONG(tone_colemak);
        #endif
        update_base_layer(_COLEMAK);
      }
      return false;
      break;
    case DVORAK:
      if (record->event.pressed) {
        #ifdef AUDIO_ENABLE
          PLAY_SONG(tone_dvorak);
        #endif
        update_base_layer(_DVORAK);
      }
      return false;
      break;
    case EUCALYN:
      if (record->event.pressed) {
        #ifdef AUDIO_ENABLE
          PLAY_SONG(tone_dvorak);
        #endif
        update_base_layer(_EUCALYN);
      }
      return false;
      break;
    case KC_xEISU:
      if (record->event.pressed) {
        if(keymap_config.swap_lalt_lgui==false){
          register_code(KC_LANG2);
        }else{
          SEND_STRING(SS_LALT("`"));
        }
      } else {
        unregister_code(KC_LANG2);
      }
      return false;
      break;
    case KC_xKANA:
      if (record->event.pressed) {
        if(keymap_config.swap_lalt_lgui==false){
          register_code(KC_LANG1);
        }else{
          SEND_STRING(SS_LALT("`"));
        }
      } else {
        unregister_code(KC_LANG1);
      }
      return false;
      break;
    case RGBRST:
      #ifdef RGBLIGHT_ENABLE
        if (record->event.pressed) {
          eeconfig_update_rgblight_default();
          rgblight_enable();
        }
      #endif
      break;
  }
  return true;
}

void matrix_init_user(void) {
    #ifdef AUDIO_ENABLE
        startup_user();
    #endif
    //SSD1306 OLED init, make sure to add #define SSD1306OLED in config.h
    #ifdef SSD1306OLED
        iota_gfx_init(!has_usb());   // turns on the display
    #endif
}


#ifdef AUDIO_ENABLE

void startup_user()
{
    _delay_ms(50); // gets rid of tick
}

void shutdown_user()
{
    _delay_ms(150);
    stop_all_notes();
}

void music_on_user(void)
{
    music_scale_user();
}

void music_scale_user(void)
{
    PLAY_SONG(music_scale);
}

#endif


//SSD1306 OLED update loop, make sure to add #define SSD1306OLED in config.h
#ifdef SSD1306OLED

void matrix_scan_user(void) {
     iota_gfx_task();  // this is what updates the display continuously
}

void matrix_update(struct CharacterMatrix *dest,
                          const struct CharacterMatrix *source) {
  if (memcmp(dest->display, source->display, sizeof(dest->display))) {
    memcpy(dest->display, source->display, sizeof(dest->display));
    dest->dirty = true;
  }
}

static void render_logo(struct CharacterMatrix *matrix) {

  static char logo[]={
    0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,0x90,0x91,0x92,0x93,0x94,
    0xa0,0xa1,0xa2,0xa3,0xa4,0xa5,0xa6,0xa7,0xa8,0xa9,0xaa,0xab,0xac,0xad,0xae,0xaf,0xb0,0xb1,0xb2,0xb3,0xb4,
    0xc0,0xc1,0xc2,0xc3,0xc4,0xc5,0xc6,0xc7,0xc8,0xc9,0xca,0xcb,0xcc,0xcd,0xce,0xcf,0xd0,0xd1,0xd2,0xd3,0xd4,
    0};
  matrix_write(matrix, logo);
#if defined(RGBLIGHT_ENABLE) && defined(RGBLIGHT_ANIMATIONS)
  char buf[30];
  if(rgblight_config.enable) {
      snprintf(buf, sizeof(buf), " LED %2d: %d,%d,%d ",
               rgblight_config.mode,
               rgblight_config.hue/RGBLIGHT_HUE_STEP,
               rgblight_config.sat/RGBLIGHT_SAT_STEP,
               rgblight_config.val/RGBLIGHT_VAL_STEP);
      matrix_write(matrix, buf);
  }
#endif
  //matrix_write_P(&matrix, PSTR(" Split keyboard kit"));
}

static const char Qwerty_name[]  PROGMEM = " Qwerty";
static const char Colemak_name[] PROGMEM = " Colemak";
static const char Dvorak_name[]  PROGMEM = " Dvorak";
static const char Eucalyn_name[] PROGMEM = " Eucalyn";

static const char NumL_name[]     PROGMEM = ":NumL";
static const char NumR_name[]     PROGMEM = ":NumR";
static const char Lower_name[]   PROGMEM = ":Func";
static const char Raise_name[]   PROGMEM = ":Extra";
static const char Adjust_name[]  PROGMEM = ":Adjust";

static const char *layer_names[] = {
    [_QWERTY]  = Qwerty_name,
    [_COLEMAK] = Colemak_name,
    [_DVORAK]  = Dvorak_name,
    [_EUCALYN] = Eucalyn_name,

    [_NUML]    = NumL_name,
    [_NUMR]    = NumR_name,
    [_LOWER]   = Lower_name,
    [_RAISE]   = Raise_name,
    [_ADJUST]  = Adjust_name
};

void render_status(struct CharacterMatrix *matrix) {

  // Render to mode icon
  static char logo[][2][3]={{{0x95,0x96,0},{0xb5,0xb6,0}},{{0x97,0x98,0},{0xb7,0xb8,0}}};
  if(keymap_config.swap_lalt_lgui==false){
    matrix_write(matrix, logo[0][0]);
    matrix_write_P(matrix, PSTR("\n"));
    matrix_write(matrix, logo[0][1]);
  }else{
    matrix_write(matrix, logo[1][0]);
    matrix_write_P(matrix, PSTR("\n"));
    matrix_write(matrix, logo[1][1]);
  }

  // Define layers here, Have not worked out how to have text displayed for each layer. Copy down the number you see and add a case for it below
  int name_num;
  uint32_t lstate;
  matrix_write_P(matrix, layer_names[current_default_layer]);
  matrix_write_P(matrix, PSTR("\n"));
  for( lstate = layer_state, name_num = 0;
       lstate && name_num < sizeof(layer_names)/sizeof(char *);
       lstate >>=1, name_num++ ) {
      if( (lstate & 1) != 0 ) {
          if( layer_names[name_num] ) {
              matrix_write_P(matrix, layer_names[name_num]);
          }
      }
  }

  // Host Keyboard LED Status
  char led[40];
    snprintf(led, sizeof(led), "\n%s  %s  %s",
             (host_keyboard_leds() & (1<<USB_LED_NUM_LOCK)) ? "NUMLOCK" : "       ",
             (host_keyboard_leds() & (1<<USB_LED_CAPS_LOCK)) ? "CAPS" : "    ",
             (host_keyboard_leds() & (1<<USB_LED_SCROLL_LOCK)) ? "SCLK" : "    ");
  matrix_write(matrix, led);
}


void iota_gfx_task_user(void) {
  struct CharacterMatrix matrix;

#if DEBUG_TO_SCREEN
  if (debug_enable) {
    return;
  }
#endif

  matrix_clear(&matrix);
  if(is_master){
    render_status(&matrix);
  }else{
    render_logo(&matrix);
  }
  matrix_update(&display, &matrix);
}

#endif
