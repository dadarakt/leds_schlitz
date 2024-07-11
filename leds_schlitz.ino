#include <FastLED.h>
#include "ColorPalettes.h"

#define BAR_LED_TYPE WS2812
#define BAR_NUM_LEDS 142
#define BAR1_DATA_PIN 9
#define BAR2_DATA_PIN 10

#define MATRIX_NUM_LEDS 255
#define MATRIX_DATA_PIN 8

#define STRIP_LED_TYPE WS2801
#define STRIP_NUM_LEDS 50

#define STRIP1_CLOCK_PIN 4
#define STRIP1_DATA_PIN 5

#define STRIP2_CLOCK_PIN 6
#define STRIP2_DATA_PIN 7

CRGB bar_1[BAR_NUM_LEDS];
CRGB bar_2[BAR_NUM_LEDS];
CRGB matrix[MATRIX_NUM_LEDS];
CRGB strip_1[STRIP_NUM_LEDS];
CRGB strip_2[STRIP_NUM_LEDS];

CRGBPalette16 currentPalette = oranje;
//static CRGBPalette16 currentPalette = LavaColors_p;
//static CRGBPalette16 currentPalette = bluePalette;

TBlendType currentBlending;

uint16_t i, j, p,t ;
static uint16_t x, y, z;

float fps = 120;

// Values that change over time 
uint16_t t_mod = 0;

double mod1 = 0;
double mod2 = 0;

double mod1_phase = 0;
double mod2_phase = 0;

double mod1_frequency = 0.005;
double mod2_frequency = 0.007;


void setup() {
  // dev only sanity pause
  delay(2000);

  // LED setup
  FastLED.addLeds<WS2812, BAR1_DATA_PIN, GRB>(bar_1, BAR_NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<WS2812, BAR2_DATA_PIN, GRB>(bar_2, BAR_NUM_LEDS).setCorrection(TypicalLEDStrip);

  FastLED.addLeds<WS2812, MATRIX_DATA_PIN, GRB>(matrix, MATRIX_NUM_LEDS).setCorrection(TypicalLEDStrip);

  FastLED.addLeds<WS2801, STRIP1_DATA_PIN, STRIP1_CLOCK_PIN, RGB>(strip_1, STRIP_NUM_LEDS);
  FastLED.addLeds<WS2801, STRIP2_DATA_PIN, STRIP2_CLOCK_PIN, RGB>(strip_2, STRIP_NUM_LEDS);

  FastLED.setBrightness(255);
  //currentPalette = ForestColors_p;
  currentBlending = LINEARBLEND;

  fill_solid(bar_1, BAR_NUM_LEDS, CRGB::Black);
  fill_solid(bar_2, BAR_NUM_LEDS, CRGB::Black);
  fill_solid(strip_1, STRIP_NUM_LEDS, CRGB::Black);
  fill_solid(strip_2, STRIP_NUM_LEDS, CRGB::Black);
  fill_solid(matrix, MATRIX_NUM_LEDS, CRGB::Black);

  FastLED.show();

  // Initialize our noise coordinates to some random values
  x = random16();
  y = random16();
  z = random16();
}

void loop() {
  EVERY_N_MILLISECONDS(1000 * (1 / fps)) {
    t_mod = (millis() / 100) % 65535;
    mod1 = sin(t_mod * mod1_frequency);
    mod2 = sin(t_mod * mod2_frequency);

    fade_all(255);

    //render_sine();
    render_noise();
    FastLED.show();
  }
}

