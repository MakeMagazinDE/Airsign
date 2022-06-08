/*********
 * Airsign (c)2022 Dr. Armin Zink
 * based on esp8266_web_server example
 * 
 * by
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
 
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif


#define SSID "MUFFIN"
#define PASSWORD "12345678"
#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels


// Web Server bereitstellen auf Port 80
AsyncWebServer server(80);


// Which pin on the Arduino is connected to the NeoPixels?
#define PIN        D3 

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 5*7 // Popular NeoPixel ring size

// use second Font
bool cyrillic = false;


// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

 
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


t_color led[50];

int speed = 1000;

t_color CWHITE   {255,255,255};
t_color CBLACK   {0,   0,  0  };
t_color CRED     {255,0,0};
t_color CBLUE    {0,  0,255};
t_color CGREEN   {0,255,0};
t_color CCYAN    {0,255,255};
t_color CMAGENTA {255,0,255};
t_color CYELLOW  {255,255,0};



// aktuelle Farbe
t_color col = CWHITE;


// =========================================================================
// Umrechnung von x,y auf die Pixel Schlange. Diese Schlange hat genau 5x7 Pixel,
// im Beispiel Scroll finden Sie eine Umrechnung für Schlangen mit vielen "ungenutzen"
// Pixeln

void ledbuffer_setPixel(uint8_t x,uint8_t y,t_color col){
   // Serial.print("sp");
   // range 0..4,0..6
    
   led[x*7+y] = col;   
}

// =========================================================================
// kopiert die Pixel aus dem Led[] Array in die Adafruit pixels
void ledbuffer_update(bool up) {
  
   pixels.clear();
uint8_t i=0;

 for (uint8_t x=0;x<5;x++)
    for (uint8_t y=0;y<7;y++) { 
      if (x%2==0)
  //grade (0 reihe = start!1!
  i = x*7     +  y;
     else
     i = (x+1)*7 - y-1 ;
     uint8_t k=      x*7     +  y;
    pixels.setPixelColor(i, pixels.Color(led[k].red,led[k].green,led[k].blue));
 }
    
 pixels.show(); // This sends the updated pixel color to the hardware 
 yield();
 
}


 void ledbuffer_printChar(char ch,t_color ccol) {
  //  Serial.print("pc ");
   uint8_t d;
     for (uint8_t x=0;x<5;x++) { 


    // AUSWAHL des Fonst
      if (cyrillic) 
         d = pgm_read_byte(6+(ch-32)*5+x +Cyrill5x7);
      else
  
          d = pgm_read_byte(6+(ch-32)*5+x +System5x7);
    
      if ((d&64 )  == 64) ledbuffer_setPixel (x,0, ccol); else ledbuffer_setPixel (x,0, CBLACK);
      if ((d&32 )  == 32) ledbuffer_setPixel (x,1, ccol); else ledbuffer_setPixel (x,1, CBLACK);
      if ((d&16 )  == 16) ledbuffer_setPixel (x,2, ccol); else ledbuffer_setPixel (x,2, CBLACK);
      if ((d&8 )   ==  8) ledbuffer_setPixel (x,3, ccol); else ledbuffer_setPixel (x,3, CBLACK);
      if ((d&4 )   ==  4) ledbuffer_setPixel (x,4, ccol); else ledbuffer_setPixel (x,4, CBLACK);
      if ((d&2 )   ==  2) ledbuffer_setPixel (x,5, ccol); else ledbuffer_setPixel (x,5, CBLACK);
      if ((d&1)    ==  1) ledbuffer_setPixel (x,6, ccol); else ledbuffer_setPixel (x,6, CBLACK);
    }           
   ledbuffer_update(true);    
   delay(speed);   
}


/*
 * FLAGGE ZEICHNEN
 */
void    ledbuffer_printFlag(uint8_t ch){
  if (ch =='1') {
  for (uint8_t y=0;y<2;y++)   for (uint8_t x=0;x<5;x++) ledbuffer_setPixel (4,y, CBLUE); 
  for (uint8_t y=3;y<5;y++)   for (uint8_t x=0;x<5;x++) ledbuffer_setPixel (4,y, CYELLOW); 
  for (uint8_t y=6;y<7;y++)   for (uint8_t x=0;x<5;x++) ledbuffer_setPixel (4,y, CBLACK); 
      ledbuffer_update(true);delay(speed);  
  }
   if (ch =='2') {
  for (uint8_t y=0;y<3;y++)   for (uint8_t x=0;x<5;x++) ledbuffer_setPixel (4,y, CWHITE); 
  for (uint8_t y=2;y<4;y++)   for (uint8_t x=0;x<5;x++) ledbuffer_setPixel (4,y, CBLUE); 
  for (uint8_t y=5;y<6;y++)   for (uint8_t x=0;x<5;x++) ledbuffer_setPixel (4,y, CRED); 
  for (uint8_t y=6;y<7;y++)   for (uint8_t x=0;x<5;x++) ledbuffer_setPixel (4,y, CBLACK); 
      ledbuffer_update(true);delay(speed);  
  }
}

// =========================================================================
/*
 * ZEICHENKETTE SCHREIBEN,und  STEUERZEICHEN <> ERKENNEN
 */
 extern void   ledbuffer_printString(String st,t_color ccol,bool scroll) {
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
      if (shift==0) {  
        pixels.clear();pixels.show(); delay(5);
        ledbuffer_printChar(st[i],col);
      }
      if (st[i]=='>') shift=0;    
  }
 }



const char* PARAM_INPUT_1 = "input1";
const char* PARAM_INPUT_2 = "input2";
const char* PARAM_INPUT_3 = "input3";

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



/*
 * SETUP
 */
void setup() { 
  

  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
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
 
  pixels.clear();
  pixels.show();
    Serial.print("End setup ");
  // Plain Text Test
  ledbuffer_printString("ABCDEFG",col,true);
  
}
/*
 * MAIN LOOP
 */
void loop() {
  yield();
  ledbuffer_printString(textMessage,col,scrolling);
}
