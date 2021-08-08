#include <Arduino.h>
#include <string.h>
#include <stdint.h>
#include <FastLED.h>

#define PIN 3 //control pin for led ring
#define BRIGHTNESS  255
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB

#define SNARE_COLOR CHSV(0,0,128) //RED
#define KICK_COLOR CHSV(153,255,128) //YELLOW
#define TOM_COLOR CHSV(30,255,128) //Orange
#define FLOOR_COLOR CHSV(120,255,128) //Green
#define HIHAT_COLOR CHSV(240,255,128) //Blue
#define RIDE_COLOR CHSV(300,255,128) //Magenta

#define LED_COUNT 60*5 //number of leds in a strip
//number of drums is 6
#define NUM_LEDS_IN_DRUM 60
#define NUM_OF_DRUMS 6

//A drum is in the set only if we have a led ring for it

#define IS_SNARE 1
#define IS_KICK 0
#define IS_TOM 1
#define IS_FLOOR 0
#define IS_HIHAT 1
#define IS_RIDE 1

enum drum{RIDE, SNARE, TOM, HIHAT, FLOOR, KICK};
typedef enum drum Drum;
CRGB leds[LED_COUNT];
int start_index[NUM_OF_DRUMS] = {SNARE*NUM_LEDS_IN_DRUM,KICK*NUM_LEDS_IN_DRUM,TOM*NUM_LEDS_IN_DRUM,
                                 FLOOR*NUM_LEDS_IN_DRUM,HIHAT*NUM_LEDS_IN_DRUM,RIDE*NUM_LEDS_IN_DRUM};



void showDrum(Drum drum, const struct CHSV& color){
    //fill_solid( leds + start_index[drum], NUM_LEDS_IN_DRUM,color);
    fill_solid( leds + drum * NUM_LEDS_IN_DRUM, NUM_LEDS_IN_DRUM,color);
    FastLED.show();
}

void fullLedPass(){
    static uint8_t hue = 0;
    for (int i = 0; i < NUM_LEDS_IN_DRUM; ++i) {
        for (int j = 0; j < 5; ++j) {
            leds[j*NUM_LEDS_IN_DRUM + i] = CHSV(hue, 255, 100);
        }
        hue = hue+4;
        FastLED.show();
        delay(10);
    }
    for (int i = 0; i < NUM_LEDS_IN_DRUM; ++i) {
        for (int j = 0; j < 5; ++j) {
            leds[j*NUM_LEDS_IN_DRUM + i] = 0;
        }
        FastLED.show();
        delay(10);
    }
}


void setup() {
    FastLED.addLeds<LED_TYPE, PIN, COLOR_ORDER>(leds, LED_COUNT).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(BRIGHTNESS);
    Serial.begin(9600);
    Serial.setTimeout(200);
    fullLedPass();
}


void displayColorByDrum(){
    //String drum_name = Serial.readStringUntil('@');
    char drum_name = Serial.read();
    if(drum_name == 's'){
        showDrum(SNARE, SNARE_COLOR);
    } else if (IS_KICK && drum_name == 'k'){
        showDrum(KICK, KICK_COLOR);
    } else if (IS_TOM && drum_name == 't'){
        showDrum(TOM, TOM_COLOR);
    } else if (IS_FLOOR && drum_name == 'f'){
        showDrum(FLOOR, FLOOR_COLOR);
    } else if (IS_HIHAT && drum_name == 'h'){
        showDrum(HIHAT, HIHAT_COLOR);
    } else if(IS_RIDE && drum_name == 'r'){
        showDrum(RIDE, RIDE_COLOR);
    }
    else{
        Serial.flush();
    }
}

void loop() {
    fadeToBlackBy(leds, LED_COUNT, 50);
    FastLED.show();
    displayColorByDrum();
}

