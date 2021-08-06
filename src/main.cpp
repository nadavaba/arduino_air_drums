#include <Arduino.h>
#include <string.h>
#include <stdint.h>
#include <FastLED.h>

#define PIN 3 //control pin for led ring
#define BRIGHTNESS  255
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB

#define SNARE_COLOR CRGB::Red
#define KICK_COLOR CRGB::Yellow
#define TOM_COLOR CRGB(0xFF,0X80,0X00)
#define FLOOR_COLOR CRGB::DarkGreen
#define HIHAT_COLOR CRGB::Blue
#define RIDE_COLOR CRGB::Magenta

#define LED_COUNT 60*4 //number of leds in a strip
//number of drums is 6
#define NUM_LEDS_IN_DRUM 60
#define NUM_OF_DRUMS 6
enum drum{RIDE, SNARE, TOM, HIHAT, FLOOR, KICK};
typedef enum drum Drum;


CRGB leds[LED_COUNT];

int start_index[NUM_OF_DRUMS] = {SNARE*NUM_LEDS_IN_DRUM,KICK*NUM_LEDS_IN_DRUM,TOM*NUM_LEDS_IN_DRUM,
                           FLOOR*NUM_LEDS_IN_DRUM,HIHAT*NUM_LEDS_IN_DRUM,RIDE*NUM_LEDS_IN_DRUM};
//CRGB drum_color[NUM_OF_DRUMS] = {TOM_COLOR, HIHAT_COLOR, SNARE_COLOR, RIDE_COLOR, KICK_COLOR, FLOOR_COLOR};



void showDrum(Drum drum, const struct CRGB& color){
    //fill_solid( leds + start_index[drum], NUM_LEDS_IN_DRUM,color);
    fill_solid( leds + drum * NUM_LEDS_IN_DRUM, NUM_LEDS_IN_DRUM,color);
}

void fullLedPass(){
    static uint8_t hue = 0;
    for(int i=0;i<LED_COUNT;i++){
        leds[i] = CHSV(hue++, 255, 255);;
        FastLED.show();
        delay(10);
    }
    for(int i=0;i<LED_COUNT;i++){
        leds[i] = CHSV(0, 0, 0);;
        FastLED.show();
        delay(10);
    }
    for(int i=LED_COUNT;i>=0;i--){
        leds[i] = CHSV(hue++, 255, 255);;
        FastLED.show();
        delay(10);
    }
    for(int i=LED_COUNT;i>=0;i--){
        leds[i] = CHSV(0, 0, 0);
        FastLED.show();
        delay(10);
    }
}


void setup() {
    FastLED.addLeds<LED_TYPE, PIN, COLOR_ORDER>(leds, LED_COUNT).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(BRIGHTNESS);
    Serial.begin(9600);
    Serial.setTimeout(200);
    //fullLedPass();
}


void displayColorByDrum(){
    String drum_name = Serial.readStringUntil('@');
    if(drum_name == "snare"){
        showDrum(SNARE, SNARE_COLOR);
        //chosen_color = SNARE_COLOR;
    } else if (drum_name == "kick"){
        showDrum(KICK, KICK_COLOR);
        //chosen_color = KICK_COLOR;
    } else if (drum_name == "tom"){
        showDrum(TOM, TOM_COLOR);
        //chosen_color = TOM_COLOR;
    } else if (drum_name == "floor"){
        showDrum(FLOOR, FLOOR_COLOR);
        //chosen_color = FLOOR_COLOR;
    } else if (drum_name == "hihat"){
        showDrum(HIHAT, HIHAT_COLOR);
        //chosen_color = HIHAT_COLOR;
    } else if(drum_name == "ride"){
        showDrum(RIDE, RIDE_COLOR);
        //chosen_color = RIDE_COLOR;
    }
    FastLED.show() ;
}






void loop() {
    for(int i=0;i<LED_COUNT;i++){
        leds[i] = CHSV(0,0,0);
    }
    FastLED.show();
    FastLED.show();
    displayColorByDrum();
}

