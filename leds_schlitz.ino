#include <FastLED.h>
#include "ColorPalettes.h"
#include "Modes.h"

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

#define SWITCH_PIN 3

boolean switch_state = false;
boolean old_switch_state = false;

CRGB bar_1[BAR_NUM_LEDS];
CRGB bar_2[BAR_NUM_LEDS];
CRGB matrix[MATRIX_NUM_LEDS];
CRGB strip_1[STRIP_NUM_LEDS];
CRGB strip_2[STRIP_NUM_LEDS];

Mode current_mode = noise_red;

CRGBPalette16 currentPalette = redPalette;

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

double mod1_frequency = 0.0005;
double mod2_frequency = 0.0008;

uint8_t decay_rate = 100;

unsigned long now;

bool long_pulse = true;
unsigned long last_pulse = 0;
const unsigned long short_interval = 400;
const unsigned long long_interval = 1900;
const unsigned long pulse_rise = 80;
const unsigned long pulse_fall = 500;
const unsigned long pulse_duration = pulse_rise + pulse_fall;


bool particle_event = false;
unsigned long last_particle_event = 0;
unsigned long event_duration = 0;
unsigned long event_intensity = 0;
unsigned long next_particle_event = 0;


void determine_next_particle_event(long now) {
  next_particle_event = now + random(60, 120) * 1000;
  event_duration = random(5, 20) * 1000;
  event_intensity = random(3, 15);
}

void check_particle_event() {
  now = millis();
  if (!particle_event && now > next_particle_event) {
    particle_event = true;
  } else if (particle_event && now > (next_particle_event + event_duration)) {
    particle_event = false;
    determine_next_particle_event(now);
  }
}

void setup() {
  // dev only sanity pause

  determine_next_particle_event(0);

  // Setup switch
  pinMode(SWITCH_PIN, INPUT);
  switch_state = digitalRead(SWITCH_PIN);
  old_switch_state = switch_state;

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


void add_glitter(uint8_t num_particles) 
{
  for(p = 0; p < num_particles; p++) {
    bar_1[ random16(BAR_NUM_LEDS) ] += CRGB::White;
    bar_2[ random16(BAR_NUM_LEDS) ] += CRGB::White;
    strip_1[ random16(STRIP_NUM_LEDS) ] += CRGB::White;
    strip_2[ random16(STRIP_NUM_LEDS) ] += CRGB::White;
  }
}


void loop() {
  EVERY_N_MILLISECONDS(100) {
    old_switch_state = switch_state;
    switch_state = digitalRead(SWITCH_PIN);
    if (old_switch_state != switch_state) {
      next_mode();
    }

  }


  EVERY_N_MILLISECONDS(1000 * (1 / fps)) {
    t_mod = (millis() / 100) % 65535;
    mod1 = sin(t_mod * mod1_frequency);
    mod2 = sin(t_mod * mod2_frequency);

    fade_all(decay_rate);

    if (current_mode == sines) {
      render_sine();
    } else {
      render_noise();
    }
    //render_noise();
    FastLED.show();
  }

  check_particle_event();
  if (particle_event) {
    add_glitter(event_intensity);
  }
  

  if (current_mode == pulsing) {
    add_pulse();
  }
}

void next_mode() {
  if (current_mode == pulsing) {
    current_mode = noise_red;
  } else {
    current_mode = current_mode + 1;
  }

  switch (current_mode) {
    case noise_orange:    
      decay_rate = 255; 
      currentPalette = oranje;
      break;
    case noise_blue:
      decay_rate = 255;
      currentPalette = bluePalette;
      break;
    case noise_red:
      decay_rate = 150;
      currentPalette = redPalette;
      break;
    case noise_lava:
      decay_rate = 255;
      currentPalette = LavaColors_p;
      break;
    case pulsing:
      decay_rate = 50;
      currentPalette = reddish;
      break;
  }
}


void add_pulse() {
  now = millis();
  if (long_pulse && now > (last_pulse + long_interval)) {
    last_pulse = now;
    long_pulse = !long_pulse;
    return;
  }

  if (!long_pulse && now > (last_pulse + short_interval)) {
    last_pulse = now;
    long_pulse = !long_pulse;
    return;
  }

  if (now < (last_pulse + pulse_duration)) {
    unsigned long t_pulse = now - last_pulse;

    if (t_pulse <= pulse_rise) {
       float amount = t_pulse / (float)pulse_rise;
       add_solid_all(CRGB(amount * 255, amount * 50, amount * 50));
       return;
    }
  }
}

