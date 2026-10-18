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
