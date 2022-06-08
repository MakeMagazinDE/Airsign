/*********
 * AIRSIGN SCROLLER
 * (c)2022 Dr. Armin Zink
 * based on:
 * ESP8266 WEB SERVER Example by
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-esp8266-input-data-html-form/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/

#include <Arduino.h>
#ifdef ESP32
  #include <WiFi.h>
  #include <AsyncTCP.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>
#include <Adafruit_NeoPixel.h>
//#include "FastLED.h"
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif


#define SSID "MUFFIN"
#define PASSWORD "12345678"

// START WEB SERVER ON PORT 80
AsyncWebServer server(80);


// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        D3 // On Trinket or Gemma, suggest changing this to 1
#define NUM_LEDS    228


//#define DATA_PIN    3
/*#define LED_TYPE    WS2811
#define COLOR_ORDER GRB

#define BRIGHTNESS  255
*/
bool cyrillic = false;



// How many NeoPixels are attached to the Arduino?
//#define NUMPIXELS 5*7 // Popular NeoPixel ring size

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

static const uint8_t System5x7[] PROGMEM = {
    0x0, 
    0x0, // size of zero indicates fixed width font, actual length is width * height
    0x05, // width
    0x07, // height
    0x20, // first char
    0x60, // char count
    
    // Fixed width; char width table not used !!!!
    
    // font data
  0x00, 0x00, 0x00, 0x00, 0x00,// (space)
  0x00, 0x00, 0x5F, 0x00, 0x00,// !
  0x00, 0x07, 0x00, 0x07, 0x00,// "
  0x14, 0x7F, 0x14, 0x7F, 0x14,// #
  0x24, 0x2A, 0x7F, 0x2A, 0x12,// $
  0x23, 0x13, 0x08, 0x64, 0x62,// %
  0x36, 0x49, 0x55, 0x22, 0x50,// &
  0x00, 0x05, 0x03, 0x00, 0x00,// '
  0x00, 0x1C, 0x22, 0x41, 0x00,// (
  0x00, 0x41, 0x22, 0x1C, 0x00,// )
  0x08, 0x2A, 0x1C, 0x2A, 0x08,// *
  0x08, 0x08, 0x3E, 0x08, 0x08,// +
  0x00, 0x50, 0x30, 0x00, 0x00,// ,
  0x08, 0x08, 0x08, 0x08, 0x08,// -
  0x00, 0x60, 0x60, 0x00, 0x00,// .
  0x20, 0x10, 0x08, 0x04, 0x02,// /
  0x3E, 0x51, 0x49, 0x45, 0x3E,// 0
  0x00, 0x42, 0x7F, 0x40, 0x00,// 1
  0x42, 0x61, 0x51, 0x49, 0x46,// 2
  0x21, 0x41, 0x45, 0x4B, 0x31,// 3
  0x18, 0x14, 0x12, 0x7F, 0x10,// 4
  0x27, 0x45, 0x45, 0x45, 0x39,// 5
  0x3C, 0x4A, 0x49, 0x49, 0x30,// 6
  0x01, 0x71, 0x09, 0x05, 0x03,// 7
  0x36, 0x49, 0x49, 0x49, 0x36,// 8
  0x06, 0x49, 0x49, 0x29, 0x1E,// 9
  0x00, 0x36, 0x36, 0x00, 0x00,// :
  0x00, 0x56, 0x36, 0x00, 0x00,// ;
  0x00, 0x08, 0x14, 0x22, 0x41,// <
  0x14, 0x14, 0x14, 0x14, 0x14,// =
  0x41, 0x22, 0x14, 0x08, 0x00,// >
  0x02, 0x01, 0x51, 0x09, 0x06,// ?
  0x32, 0x49, 0x79, 0x41, 0x3E,// @
  0x7E, 0x11, 0x11, 0x11, 0x7E,// A
  0x7F, 0x49, 0x49, 0x49, 0x36,// B
  0x3E, 0x41, 0x41, 0x41, 0x22,// C
  0x7F, 0x41, 0x41, 0x22, 0x1C,// D
  0x7F, 0x49, 0x49, 0x49, 0x41,// E
  0x7F, 0x09, 0x09, 0x01, 0x01,// F
  0x3E, 0x41, 0x41, 0x51, 0x32,// G
  0x7F, 0x08, 0x08, 0x08, 0x7F,// H
  0x00, 0x41, 0x7F, 0x41, 0x00,// I
  0x20, 0x40, 0x41, 0x3F, 0x01,// J
  0x7F, 0x08, 0x14, 0x22, 0x41,// K
  0x7F, 0x40, 0x40, 0x40, 0x40,// L
  0x7F, 0x02, 0x04, 0x02, 0x7F,// M
  0x7F, 0x04, 0x08, 0x10, 0x7F,// N
  0x3E, 0x41, 0x41, 0x41, 0x3E,// O
  0x7F, 0x09, 0x09, 0x09, 0x06,// P
  0x3E, 0x41, 0x51, 0x21, 0x5E,// Q
  0x7F, 0x09, 0x19, 0x29, 0x46,// R
  0x46, 0x49, 0x49, 0x49, 0x31,// S
  0x01, 0x01, 0x7F, 0x01, 0x01,// T
  0x3F, 0x40, 0x40, 0x40, 0x3F,// U
  0x1F, 0x20, 0x40, 0x20, 0x1F,// V
  0x7F, 0x20, 0x18, 0x20, 0x7F,// W
  0x63, 0x14, 0x08, 0x14, 0x63,// X
  0x03, 0x04, 0x78, 0x04, 0x03,// Y
  0x61, 0x51, 0x49, 0x45, 0x43,// Z
  0x00, 0x00, 0x7F, 0x41, 0x41,// [
  0x02, 0x04, 0x08, 0x10, 0x20,// "\"
  0x41, 0x41, 0x7F, 0x00, 0x00,// ]
  0x04, 0x02, 0x01, 0x02, 0x04,// ^
  0x40, 0x40, 0x40, 0x40, 0x40,// _
  0x00, 0x01, 0x02, 0x04, 0x00,// `
  0x20, 0x54, 0x54, 0x54, 0x78,// a
  0x7F, 0x48, 0x44, 0x44, 0x38,// b
  0x38, 0x44, 0x44, 0x44, 0x20,// c
  0x38, 0x44, 0x44, 0x48, 0x7F,// d
  0x38, 0x54, 0x54, 0x54, 0x18,// e
  0x08, 0x7E, 0x09, 0x01, 0x02,// f
  0x08, 0x14, 0x54, 0x54, 0x3C,// g
  0x7F, 0x08, 0x04, 0x04, 0x78,// h
  0x00, 0x44, 0x7D, 0x40, 0x00,// i
  0x20, 0x40, 0x44, 0x3D, 0x00,// j
  0x00, 0x7F, 0x10, 0x28, 0x44,// k
  0x00, 0x41, 0x7F, 0x40, 0x00,// l
  0x7C, 0x04, 0x18, 0x04, 0x78,// m
  0x7C, 0x08, 0x04, 0x04, 0x78,// n
  0x38, 0x44, 0x44, 0x44, 0x38,// o
  0x7C, 0x14, 0x14, 0x14, 0x08,// p
  0x08, 0x14, 0x14, 0x18, 0x7C,// q
  0x7C, 0x08, 0x04, 0x04, 0x08,// r
  0x48, 0x54, 0x54, 0x54, 0x20,// s
  0x04, 0x3F, 0x44, 0x40, 0x20,// t
  0x3C, 0x40, 0x40, 0x20, 0x7C,// u
  0x1C, 0x20, 0x40, 0x20, 0x1C,// v
  0x3C, 0x40, 0x30, 0x40, 0x3C,// w
  0x44, 0x28, 0x10, 0x28, 0x44,// x
  0x0C, 0x50, 0x50, 0x50, 0x3C,// y
  0x44, 0x64, 0x54, 0x4C, 0x44,// z
  0x00, 0x08, 0x36, 0x41, 0x00,// {
  0x00, 0x00, 0x7F, 0x00, 0x00,// |
  0x00, 0x41, 0x36, 0x08, 0x00,// }
  0x08, 0x08, 0x2A, 0x1C, 0x08,// ->
  0x08, 0x1C, 0x2A, 0x08, 0x08 // <-
    
};

static const uint8_t Cyrill5x7[] PROGMEM = {
0x0, 
    0x0, // size of zero indicates fixed width font, actual length is width * height
    0x05, // width
    0x07, // height
    0x20, // first char
    0x60, // char count
    
    // Fixed width; char width table not used !!!!
    
    // font data
  0x00, 0x00, 0x00, 0x00, 0x00,// (space)
  0x00, 0x00, 0x5F, 0x00, 0x00,// !
  0x00, 0x07, 0x00, 0x07, 0x00,// "
  0x14, 0x7F, 0x14, 0x7F, 0x14,// #
  0x24, 0x2A, 0x7F, 0x2A, 0x12,// $
  0x23, 0x13, 0x08, 0x64, 0x62,// %
  0x36, 0x49, 0x55, 0x22, 0x50,// &
  0x00, 0x05, 0x03, 0x00, 0x00,// '
  0x00, 0x1C, 0x22, 0x41, 0x00,// (
  0x00, 0x41, 0x22, 0x1C, 0x00,// )
  0x08, 0x2A, 0x1C, 0x2A, 0x08,// *
  0x08, 0x08, 0x3E, 0x08, 0x08,// +
  0x00, 0x50, 0x30, 0x00, 0x00,// ,
  0x08, 0x08, 0x08, 0x08, 0x08,// -
  0x00, 0x60, 0x60, 0x00, 0x00,// .
  0x20, 0x10, 0x08, 0x04, 0x02,// /
  0x3E, 0x51, 0x49, 0x45, 0x3E,// 0
  0x00, 0x42, 0x7F, 0x40, 0x00,// 1
  0x42, 0x61, 0x51, 0x49, 0x46,// 2
  0x21, 0x41, 0x45, 0x4B, 0x31,// 3
  0x18, 0x14, 0x12, 0x7F, 0x10,// 4
  0x27, 0x45, 0x45, 0x45, 0x39,// 5
  0x3C, 0x4A, 0x49, 0x49, 0x30,// 6
  0x01, 0x71, 0x09, 0x05, 0x03,// 7
  0x36, 0x49, 0x49, 0x49, 0x36,// 8
  0x06, 0x49, 0x49, 0x29, 0x1E,// 9
  0x00, 0x36, 0x36, 0x00, 0x00,// :
  0x00, 0x56, 0x36, 0x00, 0x00,// ;
  0x00, 0x08, 0x14, 0x22, 0x41,// <
  0x14, 0x14, 0x14, 0x14, 0x14,// =
  0x41, 0x22, 0x14, 0x08, 0x00,// >
  0x02, 0x01, 0x51, 0x09, 0x06,// ?
  0x32, 0x49, 0x79, 0x41, 0x3E,// @
  // MSB bottom, 7 not used.
  //  0b01000000, 0b01100100 , 0b01100100,0b01100100,0b01100100,
  0x7E, 0x11, 0x11, 0x11, 0x7E,// A
  
  0b01111111, // // B 
  0b01001001,
  0b01001001,
  0b01001001,
  0b00110001,
  
  0b01111111, // C 
  0b01000000,
  0b01000000,
  0b01111111,
  0b01000000,
  
  0b01100000, // D 
  0b00111110,
  0b00100001,
  0b00111111,
  0b01000000,


  0b01111111, // E
  0b01001001,
  0b01001001,
  0b01001001,
  0b01000001,

  0b00011100, // F 
  0b00100010,
  0b01111111,
  0b00100010,
  0b00011100,
  
  0b01111111, // G 
  0b00000001,
  0b00000001,
  0b00000001,
  0b00000001, 

  0b01100011, // H 
  0b00010100,
  0b00001000,
  0b00010100,
  0b01100011,

  0b01111111, // I 
  0b00100000,
  0b00011000,
  0b00000100,
  0b01111111, 

   
  0b01000001, // J 
  0b01001001,
  0b01001001,
  0b01001001,
  0b00111110,

  0b01111111, // K 
  0b00001000,
  0b00010100,
  0b00100010,
  0b01000001,

  0b01000000, // L
  0b01111111,
  0b00000001,
  0b00000001,
  0b11111111,


  0b01111111, // M 
  0b00000010,
  0b00001100,
  0b00000010,
  0b11111111,


  0b01111111, // N
  0b00001000,
  0b00001000,
  0b00001000,
  0b11111111,

  0b00111110, // O
  0b01000001,
  0b01000001,
  0b01000001,
  0b00111110,


  0b01111111, // P
  0b00000001,
  0b00000001,
  0b00000001,
  0b11111111,

  0b01111111, // Q
  0b00001000,
  0b00011100,
  0b00100010,
  0b00011100,

  0b01111111, // R
  0b00010001,
  0b00010001,
  0b00010001,
  0b00001110,

  0b00111110, // S
  0b01000001,
  0b01000001,
  0b01000001,
  0b00100010,

  0b00000001, // T
  0b00000001,
  0b01111111,
  0b00000001,
  0b00000001,


  0b00001111, // U
  0b01010000,
  0b01010000,
  0b01010000,
  0b00111111,


  0b01111111, // V
  0b01001001,
  0b01001001,
  0b01001001,
  0b00110110,


  0b01111111, // W
  0b01000000,
  0b01111111,
  0b01000000,
  0b01111111,


  0b00000001, // X
  0b01111111,
  0b01001000,
  0b01001000,
  0b00110000,

  
  0b01111111, // Y
  0b01010000,
  0b00100000,
  0b00000000,
  0b01111111,


  0b01110111, // Z
  0b00001000,
  0b01111111,
  0b00001000,
  0b01110111,




  0x00, 0x00, 0x7F, 0x41, 0x41,// [
  0x02, 0x04, 0x08, 0x10, 0x20,// "\"
  0x41, 0x41, 0x7F, 0x00, 0x00,// ]
  0x04, 0x02, 0x01, 0x02, 0x04,// ^
  0x40, 0x40, 0x40, 0x40, 0x40,// _
  0x00, 0x01, 0x02, 0x04, 0x00,// `
  0x20, 0x54, 0x54, 0x54, 0x78,// a
  0x7F, 0x48, 0x44, 0x44, 0x38,// b
  0x38, 0x44, 0x44, 0x44, 0x20,// c
  0x38, 0x44, 0x44, 0x48, 0x7F,// d
  0x38, 0x54, 0x54, 0x54, 0x18,// e
  0x08, 0x7E, 0x09, 0x01, 0x02,// f
  0x08, 0x14, 0x54, 0x54, 0x3C,// g
  0x7F, 0x08, 0x04, 0x04, 0x78,// h
  0x00, 0x44, 0x7D, 0x40, 0x00,// i
  0x20, 0x40, 0x44, 0x3D, 0x00,// j
  0x00, 0x7F, 0x10, 0x28, 0x44,// k
  0x00, 0x41, 0x7F, 0x40, 0x00,// l
  0x7C, 0x04, 0x18, 0x04, 0x78,// m
  0x7C, 0x08, 0x04, 0x04, 0x78,// n
  0x38, 0x44, 0x44, 0x44, 0x38,// o
  0x7C, 0x14, 0x14, 0x14, 0x08,// p
  0x08, 0x14, 0x14, 0x18, 0x7C,// q
  0x7C, 0x08, 0x04, 0x04, 0x08,// r
  0x48, 0x54, 0x54, 0x54, 0x20,// s
  0x04, 0x3F, 0x44, 0x40, 0x20,// t
  0x3C, 0x40, 0x40, 0x20, 0x7C,// u
  0x1C, 0x20, 0x40, 0x20, 0x1C,// v
  0x3C, 0x40, 0x30, 0x40, 0x3C,// w
  0x44, 0x28, 0x10, 0x28, 0x44,// x
  0x0C, 0x50, 0x50, 0x50, 0x3C,// y
  0x44, 0x64, 0x54, 0x4C, 0x44,// z
  0x00, 0x08, 0x36, 0x41, 0x00,// {
  0x00, 0x00, 0x7F, 0x00, 0x00,// |
  0x00, 0x41, 0x36, 0x08, 0x00,// }
  0x08, 0x08, 0x2A, 0x1C, 0x08,// ->
  0x08, 0x1C, 0x2A, 0x08, 0x08 // <-
    
};

 

const char* PARAM_INPUT_1 = "input1";
const char* PARAM_INPUT_2 = "input2";
const char* PARAM_INPUT_3 = "input3";

 

 String inputMessage;
 String textMessage;
 String speedMessage;
 String scrollMessage;
 bool   scrolling ;


 
typedef struct                                                          // display colors
{
    uint8_t red;                                                        // red, 0...63 (MAX_COLOR_STEPS)
    uint8_t green;                                                      // green, 0...63 (MAX_COLOR_STEPS)
    uint8_t blue;                                                       // blue, 0...63 (MAX_COLOR_STEPS)
                                                       
} t_color;


t_color led[NUM_LEDS];


// Hier werden die genutzen LEDS der LED Schlange definiert.
// Set Pixel(x,y) verwendet dann genau die "richtige" LED.
// Koordinatensystem: unten rechts = 14,0

// Harware LED Nummern in Schlangenlinien, 
// Die Schlange fängt "unten rechts" an, aber das Koordinatensystem ist unten links (0,0)...


// Umrechnungstabelle von Setpixel(x,y) in LEDS[]
// =========================================================================
// led[] ORIENTIERUNG zur einfachen Rechnung  
/*
90 91 92 93 94 95 96 97 98 99 100 101 102 103 104
75 76 77 78 79 80 81 82 83 84  85  86  87  88  89
60 61 62 63 64 65 66 67 68 69  70  71  72  73  74 
45 46 47 48 49 50 51 52 53 54  55  56  57  58  59
30 31 32 33 34 35 36 37 38 39  40  41  42  43  44
15 16 17 18 19 20 21 22 23 24  25  26  27  28  29
 0  1  2  3  4  5  6  7  8  9  10  11  12  13  14 
 */
 




static const uint8_t m[]  = {
     227,  225,  223,  221,  219,  217,  215,  213,  211,  209,  207,  205,  203,  201, 199,
     166,  168,  170,  172,  174,  176,  178,  180,  182,  184,  186,  188,  190,  192, 194,
     161,  159,  157,  155,  153,  151,  149,  147,  145,  143,  141,  139,  137,  135, 133,
     100,  102,  104,  106,  108,  110,  112,  114,  116,  118,  120,  122,  124,  126, 128,
      95,   93,   91,   89,   87,   85,   83,   81,   79,   77,   75,   73,   71,   69,  67,
      34,   36,   38,   40,   42,   44,   46,   48,   50,   52,   54,   56,   58,   60,  62,
      29,   27,   25,   23,   21,   19,   17,   15,   13,   11,    9,    7,    5,    3,   1
};

// Lauflicht delay zwischen zwei Pixel Spalten
int speed = 100;

// Farbdefinitionen für Steuerzeichen <C> = CYAN
t_color CWHITE   {255,255,255};
t_color CBLACK   {0,   0,  0  };
t_color CRED     {255,0,0};
t_color CBLUE    {0,  0,255};
t_color CGREEN   {0,255,0};
t_color CCYAN    {0,255,255};
t_color CMAGENTA {255,0,255};
t_color CYELLOW  {255,255,0};

// aktuell gewählte Farbe
t_color col = CWHITE;


// Umrechnung von x,y in Schlangenposition, (noch ohne die Lücken)
// =========================================================================
void ledbuffer_setPixel(uint8_t x,uint8_t y,t_color col){
   
   // range 0..14,0..6
    
   led[x+(6-y)*15] = col;   
}


// =========================================================================
// Zeichnet den Pixel Buffer in die Leds. Hier erfolgt die Umrechnung der linearen led[] in die
// Lückenhafte Liste m[] ..
void ledbuffer_update() {
  
 // Serial.print("u");
 pixels.clear();

 for (uint8_t i=0;i<15*7-1;i++)
        pixels.setPixelColor(m[i], pixels.Color(led[i].red,led[i].green,led[i].blue));
    
 pixels.show(); // This sends the updated pixel color to the hardware 
 yield();
 
}

// =========================================================================
// Zeichnet einen Buchstaben Spalte für Spalte und Scrollt ihn dabei nach links
 void ledbuffer_printChar(char ch,t_color ccol) {
  //  Serial.print("pc ");
   uint8_t d;
     // Scroll left
     for (uint8_t y=0;y<7;y++) for (uint8_t x=1;x<15;x++) led[y*15+x-1] = led[y*15+x]; 
    // Eine leere Spalte nach jedem Buchstaben   
     for (uint8_t y=0;y<7;y++) ledbuffer_setPixel (14,y, CBLACK);
     ledbuffer_update( );    
     delay(speed); 


      // Scroll left
     for (uint8_t y=0;y<7;y++) for (uint8_t x=1;x<15;x++) led[y*15+x-1] = led[y*15+x]; 
    // Eine leere Spalte nach jedem Buchstaben   
     for (uint8_t y=0;y<7;y++) ledbuffer_setPixel (14,y, CBLACK);
     ledbuffer_update( );    
     delay(speed); 

     for (uint8_t x=0;x<5;x++) { 

      // Scroll left
      for (uint8_t y=0;y<7;y++) for (uint8_t x=1;x<15;x++) led[y*15+x-1] = led[y*15+x];


      // Hier wird mit pgm_read_byte auf den statischen Programmspeicher zugegriffen um an die zuvor definierten Pixelspalten der
      // Zeichensätzes zu kommen
      
      if (cyrillic) 
         d = pgm_read_byte(6+(ch-32)*5+x +Cyrill5x7);
      else
         d = pgm_read_byte(6+(ch-32)*5+x +System5x7);
    
      if ((d&64 )  == 64) ledbuffer_setPixel (14,0, ccol); else ledbuffer_setPixel (14,0, CBLACK);
      if ((d&32 )  == 32) ledbuffer_setPixel (14,1, ccol); else ledbuffer_setPixel (14,1, CBLACK);
      if ((d&16 )  == 16) ledbuffer_setPixel (14,2, ccol); else ledbuffer_setPixel (14,2, CBLACK);
      if ((d&8 )   ==  8) ledbuffer_setPixel (14,3, ccol); else ledbuffer_setPixel (14,3, CBLACK);
      if ((d&4 )   ==  4) ledbuffer_setPixel (14,4, ccol); else ledbuffer_setPixel (14,4, CBLACK);
      if ((d&2 )   ==  2) ledbuffer_setPixel (14,5, ccol); else ledbuffer_setPixel (14,5, CBLACK);
      if ((d&1)    ==  1) ledbuffer_setPixel (14,6, ccol); else ledbuffer_setPixel (14,6, CBLACK);
      ledbuffer_update( );    
      delay(speed); 

    
    }        
  
}

/*
// =========================================================================
void ledbuffer_scroll() {

     for (uint8_t y=0;y<7;y++) 
      for (uint8_t x=1;x<15;x++)
        led[y*15+x-1] = led[y*15+x]; 
}

*/

// =========================================================================
void    ledbuffer_printFlag(uint8_t ch){
  if (ch =='1') {
  for (uint8_t y=0;y<2;y++)   for (uint8_t x=0;x<5;x++) ledbuffer_setPixel (4,y, CBLUE); 
  for (uint8_t y=3;y<5;y++)   for (uint8_t x=0;x<5;x++) ledbuffer_setPixel (4,y, CYELLOW); 
  for (uint8_t y=6;y<7;y++)   for (uint8_t x=0;x<5;x++) ledbuffer_setPixel (4,y, CBLACK); 
      ledbuffer_update( );delay(speed);  
  }
   if (ch =='2') {
  for (uint8_t y=0;y<3;y++)   for (uint8_t x=0;x<5;x++) ledbuffer_setPixel (4,y, CWHITE); 
  for (uint8_t y=2;y<4;y++)   for (uint8_t x=0;x<5;x++) ledbuffer_setPixel (4,y, CBLUE); 
  for (uint8_t y=5;y<6;y++)   for (uint8_t x=0;x<5;x++) ledbuffer_setPixel (4,y, CRED); 
  for (uint8_t y=6;y<7;y++)   for (uint8_t x=0;x<5;x++) ledbuffer_setPixel (4,y, CBLACK); 
      ledbuffer_update( );delay(speed);  
  }
}

// =========================================================================
 extern void   ledbuffer_printString(String st,t_color ccol ) {
  //  Serial.print("pstrg ");
  bool shift=0;
  col = CWHITE;
  for (uint8_t i=0; i<st.length();i++) {
     
     if (shift==1) {
         if (st[i]=='R') { col = CRED;  shift=2; }
         if (st[i]=='G') { col = CGREEN;shift=2; }
         if (st[i]=='B') { col = CBLUE; shift=2; }
 
         if (st[i]=='W') { col = CWHITE;   shift=2; }
         if (st[i]=='C') { col = CCYAN;    shift=2; }
         if (st[i]=='M') { col = CMAGENTA; shift=2; }
         if (st[i]=='Y') { col = CYELLOW;  shift=2; }     
         if (st[i]=='1') {  shift=2; }   
         if (st[i]=='2') {  shift=2; }     
      }

     if (shift==2) {
       ledbuffer_printFlag(st[i]);
     }
      
      if (st[i]=='<') shift=1;
      if (shift==0)  ledbuffer_printChar(st[i],col);
      if (st[i]=='>') shift=0;    
  }
 }


// =========================================================================
// HTML web page to  3 input fields (input1, input2, input3)
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>ESP Input Form</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  </head><body>
  <form action="/get">
    Text: <input type="text" name="input1">
    <input type="submit" value="Submit">
  </form><br>
  <form action="/get">
    Delay: <input type="text" name="input2">
    <input type="submit" value="Submit">
  </form><br>
  <form action="/get">
    Option: <input type="text" name="input3">
    <input type="submit" value="Submit">
  </form>
</body></html>)rawliteral";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}



// =========================================================================
// =========================================================================
void setup() { 
  
 
pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  // set master brightness control
 // FastLED.setBrightness(BRIGHTNESS);
  
  Serial.begin(115200);

  // create access point
  while (!WiFi.softAP(SSID, PASSWORD, 6, false, 15)) {
    delay(500);
  }
  
  // Send web page with input fields to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
   
    String inputParam;
    // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      textMessage  = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;
    cyrillic = false;
    }
    // GET input2 value on <ESP_IP>/get?input2=<inputMessage>
    else if (request->hasParam(PARAM_INPUT_2)) {
    //     inputMessage = " ";
      speedMessage = request->getParam(PARAM_INPUT_2)->value();
      speed = speedMessage.toInt();
      inputParam = PARAM_INPUT_2;
    }
    // GET input3 value on <ESP_IP>/get?input3=<inputMessage>
    else if (request->hasParam(PARAM_INPUT_3)) {
    //  scrollMessage = request->getParam(PARAM_INPUT_3)->value();
        textMessage  = request->getParam(PARAM_INPUT_3)->value();
      inputParam = PARAM_INPUT_3;
     cyrillic = true;
    }
    else {
      inputMessage = "No message sent";
      inputParam = "none";
    }
    Serial.println(inputMessage);
    request->send(200, "text/html", "HTTP GET request sent to your ESP on input field (" 
                                     + inputParam + ") with value: " + inputMessage +
                                     "<br><a href=\"/\">Return to Home Page</a>");
  });
  server.onNotFound(notFound);
  server.begin();

 
  // Do plain pixel enumeration tests
    test_pixel1();
    test_pixel2();

 // Generate some test output
     ledbuffer_printString("1234567890      ", CWHITE);
     ledbuffer_printString("ABCDEFGHIJKLMNOPQRSTUVWXYZ       ", CWHITE);
}

// IDENTIFY RIGHT COLUMN
// Verwende diesen Test um zu prüfen ob die im Array M eingetragenen Pixel korrekt sind
// Es ist immer das "rechte" Pixel der Zeile aktiv. Von dor aus kann man dann einfach
// die anderen Pixel bestimmen.

void test_pixel0() {
   pixels.clear();
   pixels.show();
   Serial.print(" setup 1 ");


    pixels.clear();
    pixels.setPixelColor(m[14], 0,100,0);
    pixels.setPixelColor(m[29], 0,100,0);
    pixels.setPixelColor(m[44], 0,100,0);
    pixels.setPixelColor(m[59], 0,100,0);
    pixels.setPixelColor(m[74], 0,100,0);
    pixels.setPixelColor(m[89], 0,100,0);
    pixels.setPixelColor(m[104], 0,100,0);
    
    pixels.show(); // This sends the updated pixel color to the hardware 
    yield();delay(1000);

    Serial.print(" setup 2 ");

    pixels.clear();
    
    pixels.setPixelColor(m[90], 0,100,0);
    pixels.setPixelColor(m[75], 0,100,0);
    pixels.setPixelColor(m[60], 0,100,0); 
    pixels.setPixelColor(m[45], 0,100,0);
    pixels.setPixelColor(m[30], 0,100,0);
    pixels.setPixelColor(m[15], 0,100,0);
    pixels.setPixelColor(m[0], 0,100,0);

    pixels.show(); // This sends the updated pixel color to the hardware 
    
    Serial.print(" setup 3 ");
    yield();delay(1000);}



// =========================================================================
/*
 * prüfen des M Arrays
 * Zeichnet alle Pixel von links nach rechts Zeilenweise
 */
 void test_pixel1() {
  
  for (uint8_t i=0; i<15*7;i++) {
    pixels.clear();
    Serial.print (i);
    Serial.println (" ");
    pixels.setPixelColor(m[i], 0,100,0);
    pixels.show(); // This sends the updated pixel color to the hardware 
    yield();
    delay(20);
    
 }
}

void test_pixel2() {
 /*  Prüfen der x,y Funktion
  *   Zeichnet alle Pixel von links nach rechts oben nach unten  
  */
  for (uint8_t x=0; x<15;x++)
    for (uint8_t y=0; y<7;y++) {
      
      ledbuffer_setPixel (x,y,CWHITE);
      ledbuffer_update();
      delay(20);
       ledbuffer_setPixel (x,y,CBLACK);
      }
}
 
// =========================================================================
// =========================================================================
void loop() {
  yield();
  ledbuffer_printString(textMessage,col); 
}
