#include <Arduino.h>
#include <string.h>
#include <stdint.h>
#include <FastLED.h>

#define PIN 3 //control pin for led ring
#define BRIGHTNESS  255
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
/*
#define SNARE_COLOR CRGB::Red
#define KICK_COLOR CRGB::Yellow
#define TOM_COLOR CRGB(0xFF,0X80,0X00)
#define FLOOR_COLOR CRGB::DarkGreen
#define HIHAT_COLOR CRGB::Blue
#define RIDE_COLOR CRGB::Magenta
*/

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
enum drum{RIDE, SNARE, TOM, HIHAT, FLOOR, KICK};
typedef enum drum Drum;

CRGB leds[LED_COUNT];

int start_index[NUM_OF_DRUMS] = {SNARE*NUM_LEDS_IN_DRUM,KICK*NUM_LEDS_IN_DRUM,TOM*NUM_LEDS_IN_DRUM,
                                 FLOOR*NUM_LEDS_IN_DRUM,HIHAT*NUM_LEDS_IN_DRUM,RIDE*NUM_LEDS_IN_DRUM};



void showDrum(Drum drum, const struct CHSV& color){
    //fill_solid( leds + start_index[drum], NUM_LEDS_IN_DRUM,color);
    fill_solid( leds + drum * NUM_LEDS_IN_DRUM, NUM_LEDS_IN_DRUM,color);
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
        //chosen_color = SNARE_COLOR;
    } else if (drum_name == 'k'){
        showDrum(KICK, KICK_COLOR);
        //chosen_color = KICK_COLOR;
    } else if (drum_name == 't'){
        showDrum(TOM, TOM_COLOR);
        //chosen_color = TOM_COLOR;
    } else if (drum_name == 'f'){
        showDrum(FLOOR, FLOOR_COLOR);
        //chosen_color = FLOOR_COLOR;
    } else if (drum_name == 'h'){
        showDrum(HIHAT, HIHAT_COLOR);
        //chosen_color = HIHAT_COLOR;
    } else if(drum_name == 'r'){
        showDrum(RIDE, RIDE_COLOR);
        //chosen_color = RIDE_COLOR;
    }
    FastLED.show() ;
}

void loop() {
    /*
    for(int i=0;i<LED_COUNT;i++){
        leds[i] = CHSV(0,0,0);
    }
     */
    fadeToBlackBy(leds, LED_COUNT, 100);
    FastLED.show();
    displayColorByDrum();
    FastLED.show();
}

