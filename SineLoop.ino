// moving sine waves
byte intense;
fract8 intensity = 100;
uint8_t speed_1 = 7 ;
uint8_t speed_2 = 13;
uint8_t speed_3 = 20;
uint8_t frequency_1 = 3;
uint8_t frequency_2 = 7;
uint8_t frequency_3 = 4;
fract8 frequency_factor = 1;

void render_sine() {
  sin_loop(bar_1, BAR_NUM_LEDS);
  sin_loop(bar_2, BAR_NUM_LEDS);
  sin_loop(matrix, MATRIX_NUM_LEDS);
  sin_loop(strip_1, STRIP_NUM_LEDS);
  sin_loop(strip_2, STRIP_NUM_LEDS);
}

void sin_loop(CRGB leds[], uint8_t num_leds) {
  long now = millis();

  t = (now / 100) % 65535;
  //  frequency_factor = fract8(1, abs8(sin8(t)) / 50 + 1);

  t = (now / speed_1) % 65535;
  for (i = 0; i < num_leds; i++) {
    intense = map(sin8(i * frequency_1 * frequency_factor + t), 0, 1, 0, 255);
    leds[i] += CHSV(32, 255, lerp8by8(0, intense, intensity));
  }

  t = (now / speed_2) % 65535;
  for (i = 0; i < num_leds; i++) {
    intense = map(sin8(i * frequency_2 + t), 0, 1, 0, 255);
    leds[i] += CHSV(128, 255, lerp8by8(0, intense, intensity));
  }

  // t = (now / speed_3) % 65535;
  // for (i = 0; i < num_leds; i++) {
  //   intense = map(sin8(i * frequency_3 + t), 0, 1, 0, 200);
  //   leds[i] += CHSV(60, 255, lerp8by8(0, intense, intensity));
  // }
}