/**
 * This program is designed to randomly trigger a series of LED that follow an
 * exponential decay when a button is pressed.
 * Ideally there would be several digital outputs that would allow for a bank
 * of four LEDs to fade with a different decay rate.
 *
 * Objectives
 * 1. Create a program that executes when a button is pressed
 *    which fades an LED in an exponential fashion
 *
 * 1. a - very fast flickr
 *
 *
 * 2. Create a program that randomly flickers all LEDs until 
 *    a second button is pressed
 *
 * 3. (time permitted) - sound sensitive LED from a piezo pictup
 *
 */

// Maximum number of LEDs to support
const int led_count = 6;

// Declare the output pins for the LEDs
int leds[] = {11,10,9,6,5,3};

// Declare inputs for the momentary switches and set default
int button_A = 12;
int button_B = 4;
char last_pressed = 'A';

// Some constants
const float pi = 3.141529;
const int phase_offset = 60;
const int rand_min = 1;
const int rand_max = 100;
// Controls how fast the phased-sine wave cycles. Smaller steps means longer cycle
const float rotate = 2 * pi / 16184;

// Holds the current level for the output
unsigned int led_levels[] = { random(255), random(255), random(255), random(255), random(255), random(255) };

// Holds the random power for the LEDs
int led_decays[] = { random(rand_min, rand_max), random(rand_min, rand_max), random(rand_min, rand_max), random(rand_min, rand_max), random(rand_min, rand_max) };

float radians = 0;
float *radians_ptr = &radians;

void setup() {
  Serial.begin(9600);

  for(int x = 0; x < led_count; x++) {
    pinMode(leds[x], OUTPUT);
  }
  pinMode(button_A, INPUT);
  pinMode(button_B, INPUT);
}

void loop() {
  // Added the negative statement as the hardware controls I was using were non-latching switches that always read high
  int pressed_A = !digitalRead(button_A);
  int pressed_B = !digitalRead(button_B);

  // Run the last pressed button until another change is detected
  if(pressed_A && !pressed_B) {
    last_pressed = 'A';
  }
  if(!pressed_A && pressed_B) {
    last_pressed = 'B';
  }
  
  if( last_pressed == 'B' ) {
    for(int x = 0; x < led_count; x++) {
      led_levels[x] = sine_program(float( phase_offset * x));
      analogWrite(leds[x], led_levels[x]);
    }
  } else {
    for(int x = 0; x < led_count; x++) {
      analogWrite(leds[x], led_levels[x]);
      led_levels[x] = decay(led_levels[x], led_decays[x], x);
    }
    //delay(20);
  }
}

unsigned int decay(int current_level, int decay_factor, int array_index) {
  if(current_level > 0){
    current_level = current_level - (random(7));
    // Correction for "negative" values on unsigned ints. Ends up making the int value something greater than 255
    if( current_level > 255 ) {
      current_level = 0;
    } 
    
     // Randomly retrigger
    if(random(500) == random(100)) {
      current_level = 255;
    }
    delay(3);
    return current_level;
  } else {
    if(random(1000) == random(1000)) {
      return 255;
    } else {
      return 0;
    }
  }
  

}

int sine_program(float offset){  
  if(*radians_ptr >= 2 * pi){
    *radians_ptr = 0;
  } 
  *radians_ptr += float(rotate);
  return (255 * ( ( sin(*radians_ptr + offset) + 1 ) / 2 ));
  //return current_level;
}
