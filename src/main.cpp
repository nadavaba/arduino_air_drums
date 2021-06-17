#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <string.h>
#include <stdint.h>
#include <FastLED.h>

#define PIN 3 //control pin for led ring
#define BRIGHTNESS  64
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB

#define SNARE_COLOR CRGB::Red
#define KICK_COLOR CRGB::Yellow
#define TOM_COLOR CRGB(0xFF,0X80,0X00)
#define FLOOR_COLOR CRGB::DarkGreen
#define HIHAT_COLOR CRGB::Blue
#define RIDE_COLOR CRGB::Magenta

#define LED_COUNT 39*2 //number of leds in a strip
//number of drums is 6
#define NUM_LEDS_IN_DRUM 13

enum drum{SNARE,KICK,TOM,FLOOR,HIHAT,RIDE};
typedef enum drum Drum;


Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, PIN, NEO_GRB + NEO_KHZ800);

CRGB leds[LED_COUNT];

int start_index[RIDE+1] = {SNARE*NUM_LEDS_IN_DRUM,KICK*NUM_LEDS_IN_DRUM,TOM*NUM_LEDS_IN_DRUM,
                           FLOOR*NUM_LEDS_IN_DRUM,HIHAT*NUM_LEDS_IN_DRUM,RIDE*NUM_LEDS_IN_DRUM};
CRGB drum_color[RIDE+1] = {SNARE_COLOR, KICK_COLOR, TOM_COLOR, FLOOR_COLOR, HIHAT_COLOR, RIDE_COLOR};

//void fill(uint32_t current_drum_color, int start, int num_of_leds){
//    for(int i=0;i<num_of_leds ;i++){
//        leds[i+start] = CRGB::Red;
//    }
//}

void showDrum(Drum drum, const struct CRGB& color){
    int current_drum_color = drum_color[drum];
  //  for(int i = start_index[drum];i<=end_index[drum];i++){
  //      strip.setPixelColor(i,current_drum_color);
  //  }
    //fill(current_drum_color, start_index[drum], NUM_LEDS_IN_DRUM);
    fill_solid( leds + start_index[drum], NUM_LEDS_IN_DRUM,
                color);
}

void setup() {
    FastLED.addLeds<LED_TYPE, PIN, COLOR_ORDER>(leds, LED_COUNT).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );

  //  for(int i=0 ;i<6;i++){
  //      start_index[i] = start_index[i] * NUM_LEDS_IN_DRUM;
  //      end_index[i] = end_index[i] * NUM_LEDS_IN_DRUM;
  //  }

    //strip.begin();
    //strip.setBrightness(30); //adjust brightness here
    //strip.show();
    Serial.begin(9600);
}


void displayColorByDrum(){
    if(Serial.available()){
        String drum_name = Serial.readStringUntil('@');
        //uint32_t chosen_color = 0;
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
        //strip.fill(chosen_color);
        FastLED.show() ;
        delay(50);
        Serial.flush();
    }

    else{
        FastLED.clear(1);
        //strip.clear();
        //strip.show();
    }

}

void loop() {
    displayColorByDrum();
    //for(int i=0;i<LED_COUNT;i++){
    //    leds[i] = CRGB::Red;
    //}
    //FastLED.show();
}



