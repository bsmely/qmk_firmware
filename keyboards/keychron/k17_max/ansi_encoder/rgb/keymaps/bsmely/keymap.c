/* Copyright 2024 @ Keychron (https://www.keychron.com)
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
#include "keychron_common.h"
#include "keychron_rgb_type.h"
#include "transport.h"

extern os_indicator_config_t os_ind_cfg; // Keychron's OS indicator settings (common/rgb/keychron_rgb.c)
extern void                  os_state_indicate(void);

//region tap-dance
#include "tap_dance_utils.c"

enum {
  TD16 = 0,
  TD17,
  TD18,
  TD19,
  //TDESC,
  TDPS,
  TDNUM,
};

tap_dance_action_t tap_dance_actions[] = {
  // Hold commands (Consumer usages 0x07-0x1F, see tap_dance_utils.c), read by AutoHotkey
  [TD16] = ACTION_TAP_DANCE_HOLD_TO_COMMAND(KC_F16, 0x16),
  [TD17] = ACTION_TAP_DANCE_HOLD_TO_COMMAND(KC_F17, 0x17),
  [TD18] = ACTION_TAP_DANCE_HOLD_TO_COMMAND(KC_F18, 0x18),
  [TD19] = ACTION_TAP_DANCE_HOLD_TO_COMMAND(KC_F19, 0x19),
  //[TDESC] = ACTION_TAP_DANCE_HOLD_TO_SHIFT(KC_ESC),
  [TDPS] = ACTION_TAP_DANCE_HOLD_TO_COMMAND(KC_PSCR, 0x10),
  [TDNUM] = ACTION_TAP_DANCE_HOLD_TO_COMMAND(KC_NUM, 0x11),
};
//endregion

enum layers{
  MAC_BASE,
  MAC_FN,
  WIN_BASE,
  WIN_FN,
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [MAC_BASE] = LAYOUT_104_ansi(
        KC_ESC,   KC_BRID,  KC_BRIU,  KC_MCTRL, KC_LNPAD, UG_VALD,  UG_VALU,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,    KC_VOLU,  KC_SNAP,  UG_NEXT,  KC_DEL,   KC_F13,   KC_F14,   KC_F15,   KC_MUTE,
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,    KC_EQL,   KC_BSPC,            KC_PGUP,  KC_NUM,   KC_PSLS,  KC_PAST,  KC_PMNS,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,    KC_RBRC,  KC_BSLS,            KC_PGDN,  KC_P7,    KC_P8,    KC_P9,    KC_PPLS,
        KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,              KC_ENT,             KC_HOME,  KC_P4,    KC_P5,    KC_P6,
        KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,              KC_RSFT,  KC_UP,              KC_P1,    KC_P2,    KC_P3,    KC_PENT,
        KC_LCTL,  KC_LOPTN, KC_LCMMD,                               KC_SPC,                                 KC_RCMMD, MO(MAC_FN), KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT,  KC_P0,              KC_PDOT           ),

    [MAC_FN] = LAYOUT_104_ansi(
        _______,  KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,     KC_F12,   _______,  UG_TOGG,  _______,  _______,  _______,  _______,  UG_TOGG,
        _______,  BT_HST1,  BT_HST2,  BT_HST3,  P2P4G,    _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______,  _______,  _______,  _______,  _______,
        UG_TOGG,  UG_NEXT,  UG_VALU,  UG_HUEU,  UG_SATU,  UG_SPDU,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______,  _______,  _______,  _______,  _______,
        _______,  UG_PREV, UG_VALD,  UG_HUED,  UG_SATD,  UG_SPDD,  _______,  _______,  _______,  _______,  _______,  _______,              _______,            KC_END,   _______,  _______,  _______,
        _______,            _______,  _______,  _______,  _______,  BAT_LVL,  NK_TOGG,  _______,  _______,  _______,  _______,              _______,  _______,            _______,  _______,  _______,  _______,
        _______,  _______,  _______,                                _______,                                _______,  _______,    _______,  _______,  _______,  _______,  _______,            _______           ),

    [WIN_BASE] = LAYOUT_104_ansi(
        KC_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,     KC_F12,   TD(TDPS), TD(TD16), KC_DEL,   TD(TD17), TD(TD18), TD(TD19), KC_F24,
        KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,    KC_EQL,   KC_BSPC,            KC_PGUP,  TD(TDNUM),KC_PSLS,  KC_PAST,  KC_PMNS,
        KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,    KC_RBRC,  KC_BSLS,            KC_PGDN,  KC_P7,    KC_P8,    KC_P9,    KC_PPLS,
        KC_F13,   KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,              KC_ENT,             KC_HOME,  KC_P4,    KC_P5,    KC_P6,
        KC_LSFT,            KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,              KC_RSFT,  KC_UP,              KC_P1,    KC_P2,    KC_P3,    KC_PENT,
        KC_LCTL,  KC_LWIN,  KC_LALT,                                KC_SPC,                                 KC_RALT,  MO(WIN_FN), KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT,  KC_P0,              KC_PDOT           ),

    [WIN_FN] = LAYOUT_104_ansi(
        _______,  KC_BRID,  KC_BRIU,  KC_TASK,  KC_FILE,  UG_VALD,  UG_VALU,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,    KC_VOLU,  KC_SCRL,  UG_TOGG,  KC_INS,   _______,  _______,  _______,  UG_TOGG,
        _______,  BT_HST1,  BT_HST2,  BT_HST3,  P2P4G,    _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______,  KC_PWR,   _______,  _______,  _______,
        UG_TOGG,  UG_NEXT,  UG_VALU,  UG_HUEU,  UG_SATU,  UG_SPDU,  _______,  _______,  _______,  _______,  _______,  _______,    _______,  _______,            _______,  _______,  _______,  _______,  _______,
        _______,  UG_PREV, UG_VALD,  UG_HUED,  UG_SATD,  UG_SPDD,  _______,  _______,  _______,  _______,  _______,  _______,              _______,            KC_END,   _______,  _______,  _______,
        _______,            _______,  _______,  _______,  _______,  BAT_LVL,  NK_TOGG,  _______,  _______,  _______,  _______,              _______,  KC_PGUP,            _______,  _______,  _______,  _______,
        _______,  _______,  _______,                                _______,                                _______,  _______,    _______,  KC_HOME,  KC_PGDN,  KC_END,   _______,            _______           ),
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [MAC_BASE] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [MAC_FN]   = {ENCODER_CCW_CW(UG_VALD, UG_VALU)},
    [WIN_BASE] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [WIN_FN]   = {ENCODER_CCW_CW(UG_HUED, UG_HUEU)},
};
#endif

// clang-format on

// The default lighting (config.h) applies only when the settings are reset, so the firmware switches
// to it once by itself: the marker in the user EEPROM word says it has been done. A mode chosen later
// stays - the switch does not repeat at every start.
#define JUNGLE_DEFAULT_MARKER 0x4A4E4731 // "JNG1"

void keyboard_post_init_user(void) {
    os_ind_cfg.disable.num_lock = true; // see rgb_matrix_indicators_keychron() below
    if (eeconfig_read_user() != JUNGLE_DEFAULT_MARKER) {
        rgb_matrix_mode(RGB_MATRIX_DEFAULT_MODE);
        rgb_matrix_sethsv(RGB_MATRIX_DEFAULT_HUE, RGB_MATRIX_DEFAULT_SAT, rgb_matrix_get_val());
        rgb_matrix_set_speed(RGB_MATRIX_DEFAULT_SPD);
        eeconfig_update_user(JUNGLE_DEFAULT_MARKER);
    }
}

// Num Lock: red while it is off, nothing while it is on (numbers are what this keyboard types by
// default). Keychron lights its own (white) indicator while it is on from os_state_indicate(), which
// is called from several places - the connection indicators and the backlight-off path among them -
// so its Num Lock part is switched off for good (the flag is only in RAM; the Launcher may set it
// back, which the next frame undoes). The rest of rgb_matrix_indicators_keychron()
// (common/keychron_task.c) is kept as it is.
static void num_lock_indicator(void) {
    if (!host_keyboard_led_state().num_lock) {
        rgb_matrix_set_color(NUM_LOCK_INDEX, 255, 0, 0);
    }
}

// Late LED driver re-init after a transport change. LED_DRIVER_REINIT_ON_TRANSPORT_CHANGE (config.h)
// re-initialises the driver at the change itself, but the power is still settling then: switching from
// 2.4G back to the cable, a solid colour blinked and the keys went dark again. The driver had been reset
// once more, and the firmware did not know: a static effect renders the same frame, nothing is marked
// for rewriting, and the cleared driver stays dark (Jungle changes every frame, so it came back by itself;
// turning the lighting off and on through VIA brought a solid colour back too). Re-init once more when
// the power has settled. rgb_matrix_init() reads the settings back from EEPROM - AutoHotKey saves them.
#define LATE_REINIT_DELAYS_MS {1000, 3000}

void housekeeping_task_user(void) {
    static const uint16_t delays[] = LATE_REINIT_DELAYS_MS;
    static transport_t    last     = TRANSPORT_NONE;
    static uint8_t        pending  = 0;
    static uint32_t       since    = 0;

    transport_t now = get_transport();
    if (now != last) {
        if (last != TRANSPORT_NONE) {
            pending = sizeof(delays) / sizeof(delays[0]);
            since   = timer_read32();
        }
        last = now;
    }
    if (pending == 0) {
        return;
    }
    uint8_t next = sizeof(delays) / sizeof(delays[0]) - pending;
    if (timer_elapsed32(since) >= delays[next]) {
        rgb_matrix_driver_shutdown();
        wait_ms(20);
        rgb_matrix_init();
        pending--;
    }
}

bool rgb_matrix_indicators_keychron(void) {
    os_ind_cfg.disable.num_lock = true;
    os_state_indicate();
    num_lock_indicator();
#if defined(LK_WIRELESS_ENABLE) || defined(KC_BLUETOOTH_ENABLE)
    extern bool rgb_matrix_indicators_bt(void);
    rgb_matrix_indicators_bt();
#endif
    return true;
}

// Backlight off: only the indicators are drawn
void rgb_matrix_none_indicators_user(void) {
    num_lock_indicator();
}
