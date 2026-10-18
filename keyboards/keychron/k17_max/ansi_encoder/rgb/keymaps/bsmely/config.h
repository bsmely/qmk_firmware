#pragma once

// Hold time for the tap-dance keys (default 200 ms). A tap waits for it too:
// a single tap is decided when it runs out, unless another key interrupts.
#define TAPPING_TERM 400

// Jungle (rgb_matrix_user.inc) in dark green is the default lighting. These apply when the
// settings are reset; keymap.c switches to it once after flashing.
#undef RGB_MATRIX_DEFAULT_MODE
#define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_CUSTOM_JUNGLE
#undef RGB_MATRIX_DEFAULT_HUE
#define RGB_MATRIX_DEFAULT_HUE 85
#undef RGB_MATRIX_DEFAULT_SAT
#define RGB_MATRIX_DEFAULT_SAT 255
#undef RGB_MATRIX_DEFAULT_SPD
#define RGB_MATRIX_DEFAULT_SPD 128

// Finer speed steps (default 16)
#undef RGB_MATRIX_SPD_STEP
#define RGB_MATRIX_SPD_STEP 8

// Raw HID (VIA) over 2.4G/Bluetooth: Keychron's wireless code has it (common/wireless/wireless.c) but
// no keyboard turns it on. AutoHotKey sets the lighting through VIA commands - this lets it do so
// when the keyboard is not on the cable. The name is spelled as in Keychron's code.
#define WILRESS_RAW_ENABLE
// The code behind that flag never compiled: it calls via_raw_hid_receive(), which no longer exists -
// Keychron's VIA entry point is raw_hid_receive(src, data, length) now, same arguments.
#define via_raw_hid_receive raw_hid_receive

// Switching between the cable and 2.4G can brown-out reset the LED driver without resetting the MCU:
// the lighting stays on in the settings but the keys go dark (seen on USB with a solid colour at full
// brightness). Keychron's workaround re-initialises the driver on a transport change. The old
// wireless_playground K17 Max config had it as REINIT_LED_DRIVER; the 2025q3 port lost it, while
// K1/K2/K3/K8/K10 Max and others there define the new name.
#define LED_DRIVER_REINIT_ON_TRANSPORT_CHANGE
