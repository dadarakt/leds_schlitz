void fade_all(uint8_t fade_amount) {
  fade_leds(bar_1, BAR_NUM_LEDS, fade_amount);
  fade_leds(bar_2, BAR_NUM_LEDS, fade_amount);
  fade_leds(matrix, MATRIX_NUM_LEDS, fade_amount);
  fade_leds(strip_1, STRIP_NUM_LEDS, fade_amount);
  fade_leds(strip_2, STRIP_NUM_LEDS, fade_amount);
}

void fade_leds(CRGB leds[], uint8_t num_leds, uint8_t fade_amount) {
  for (i = 0; i < num_leds; i++) {
    leds[i].fadeToBlackBy(255);
  }
}

void add_solid(CRGB leds[], uint8_t num_leds, uint8_t amount) {
  for (i = 0; i < num_leds; i++) {
    leds[i] += CRGB(amount, 0, 0);
  }
}