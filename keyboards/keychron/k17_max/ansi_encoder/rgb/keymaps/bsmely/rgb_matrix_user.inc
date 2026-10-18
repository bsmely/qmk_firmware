// Jungle: the keys flicker in dark shades of the current colour, each with its own brightness,
// speed and tint; now and then one flashes like a firefly. A port of the PC animation for the
// mainboard LEDs (AutoHotKey, Main/lib/rgb/jungle-animation.ahk).
//
// Each LED's level moves towards a random target at a random rate; once there, it stays for a random
// number of frames and takes a new target. Targets are near darkness (gaps between the "leaves"),
// mid and bright levels, and rarely a flash. The level is shown with a gamma: LEDs are not linear, and
// without it the dark levels would look lighter and the contrast would be lost.
//
// Tuned for the keyboard rather than copied from the PC: under keycaps at a usual brightness the PC
// numbers left most keys looking off - a third of the targets were near darkness, and with gamma 1.8
// the mid levels came out at a few percent. Here dark targets are rarer and a floor keeps every key
// faintly lit, so the dark spots read as shade, not as dead keys.
//
// Hue and saturation are the matrix settings (green is hue 85), the peak is the brightness setting,
// and speed sets the frame rate: 60 ms at speed 0, 40 ms (as on the PC) at 128, 20 ms at 255.

RGB_MATRIX_EFFECT(JUNGLE)

#ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#    include <math.h>

#    define JUNGLE_GAMMA 1.4f            // higher - darker gaps and more contrast
#    define JUNGLE_FLOOR 0.06f           // share of the peak every key keeps even at level 0
#    define JUNGLE_DARK_CHANCE 0.15f     // shade: 0-8%
#    define JUNGLE_BRIGHT_CHANCE 0.3f    // bright: 65-95%
#    define JUNGLE_FIREFLY_CHANCE 0.01f  // flash: 90-100%, fast
#    define JUNGLE_RATE_MIN 0.04f        // share of the way to the target per frame
#    define JUNGLE_RATE_MAX 0.3f
#    define JUNGLE_HOLD_MAX_FRAMES 30    // frames an LED stays at a reached target (up to)
#    define JUNGLE_HUE_SHIFT_MIN -5      // tint: towards yellow ... towards cyan
#    define JUNGLE_HUE_SHIFT_MAX 14

typedef struct {
    float   level;
    float   target;
    float   rate;
    uint8_t hold;
    int8_t  hue_shift;
} jungle_led_t;

static jungle_led_t jungle_leds[RGB_MATRIX_LED_COUNT];
static uint32_t     jungle_next_step = 0;

static float jungle_random(float min, float max) {
    return min + (max - min) * (float)random16() / 65535.0f;
}

static void jungle_next_target(jungle_led_t *led) {
    float chance = jungle_random(0.0f, 1.0f);
    led->rate    = jungle_random(JUNGLE_RATE_MIN, JUNGLE_RATE_MAX);
    if (chance < JUNGLE_FIREFLY_CHANCE) {
        led->target = jungle_random(0.9f, 1.0f);
        led->rate   = jungle_random(0.35f, 0.6f);
    } else if (chance < JUNGLE_FIREFLY_CHANCE + JUNGLE_DARK_CHANCE) {
        led->target = jungle_random(0.0f, 0.08f);
    } else if (chance < JUNGLE_FIREFLY_CHANCE + JUNGLE_DARK_CHANCE + JUNGLE_BRIGHT_CHANCE) {
        led->target = jungle_random(0.65f, 0.95f);
    } else {
        led->target = jungle_random(0.25f, 0.6f);
    }
}

static void jungle_init(void) {
    random16_set_seed((uint16_t)g_rgb_timer);
    for (uint8_t i = 0; i < RGB_MATRIX_LED_COUNT; i++) {
        jungle_led_t *led = &jungle_leds[i];
        led->level        = jungle_random(0.0f, 0.3f);
        led->hold         = 0;
        led->hue_shift    = (int8_t)(JUNGLE_HUE_SHIFT_MIN + random8_max(JUNGLE_HUE_SHIFT_MAX - JUNGLE_HUE_SHIFT_MIN + 1));
        jungle_next_target(led);
    }
}

static void jungle_step(void) {
    for (uint8_t i = 0; i < RGB_MATRIX_LED_COUNT; i++) {
        jungle_led_t *led = &jungle_leds[i];
        if (led->hold > 0) {
            led->hold--;
            continue;
        }
        led->level += (led->target - led->level) * led->rate;
        if (fabsf(led->target - led->level) < 0.01f) {
            led->hold = random8_max(JUNGLE_HOLD_MAX_FRAMES + 1);
            jungle_next_target(led);
        }
    }
}

static uint16_t jungle_frame_ms(void) {
    return 60 - (uint16_t)rgb_matrix_config.speed * 40 / 255;
}

bool JUNGLE(effect_params_t *params) {
    RGB_MATRIX_USE_LIMITS(led_min, led_max);

    // The effect is drawn in chunks (led_min..led_max); the state moves once per frame, on the first one
    if (led_min == 0) {
        if (params->init) {
            jungle_init();
            jungle_next_step = g_rgb_timer;
        }
        if (timer_expired32(g_rgb_timer, jungle_next_step)) {
            jungle_step();
            jungle_next_step = g_rgb_timer + jungle_frame_ms();
        }
    }

    float peak = rgb_matrix_config.hsv.v;
    for (uint8_t i = led_min; i < led_max; i++) {
        RGB_MATRIX_TEST_LED_FLAGS();
        jungle_led_t *led = &jungle_leds[i];
        HSV           hsv = {
            .h = (uint8_t)(rgb_matrix_config.hsv.h + led->hue_shift),
            .s = rgb_matrix_config.hsv.s,
            .v = (uint8_t)(peak * (JUNGLE_FLOOR + (1.0f - JUNGLE_FLOOR) * powf(led->level, JUNGLE_GAMMA)) + 0.5f),
        };
        RGB rgb = rgb_matrix_hsv_to_rgb(hsv);
        rgb_matrix_region_set_color(params->region, i, rgb.r, rgb.g, rgb.b);
    }
    return rgb_matrix_check_finished_leds(led_max);
}

#endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
