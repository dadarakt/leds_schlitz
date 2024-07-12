void fade_all(uint8_t fade_amount) {
  fade_leds(bar_1, BAR_NUM_LEDS, fade_amount);
  fade_leds(bar_2, BAR_NUM_LEDS, fade_amount);
  fade_leds(matrix, MATRIX_NUM_LEDS, fade_amount);
  fade_leds(strip_1, STRIP_NUM_LEDS, fade_amount);
  fade_leds(strip_2, STRIP_NUM_LEDS, fade_amount);
}

void fade_leds(CRGB leds[], uint8_t num_leds, uint8_t fade_amount) {
  for (i = 0; i < num_leds; i++) {
    leds[i].fadeToBlackBy(fade_amount);
  }
}

void add_solid_all(CRGB color) {
  add_solid(bar_1, BAR_NUM_LEDS, color);
  add_solid(bar_2, BAR_NUM_LEDS, color);
  add_solid(strip_1, STRIP_NUM_LEDS, color);
  add_solid(strip_2, STRIP_NUM_LEDS, color);
  add_solid(matrix, MATRIX_NUM_LEDS, color);
}

void add_solid(CRGB leds[], uint8_t num_leds, CRGB color) {
  for (i = 0; i < num_leds; i++) {
    leds[i] += color;
  }
}