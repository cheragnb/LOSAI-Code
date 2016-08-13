// This #include statement was automatically added by the Particle IDE.
#include "Adafruit_GFX/Adafruit_GFX.h"

// This #include statement was automatically added by the Particle IDE.
#include "neomatrix/neomatrix.h"

#include "application.h"
#include "neopixel/neopixel.h"
#include "rain.h"
#include "snow.h"
#include "sun_large.h"
#include "sun_medium.h"
#include "sun_low.h"
#include "thunder.h"
#include "wind_large.h"
#include "wind_medium.h"
#include "wind_low.h"
// IMPORTANT: Set pixel PIN and TYPE and 
#define PIXEL_PIN D2
#define PIXEL_TYPE WS2812B


Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(16,16,1,1, PIXEL_PIN,
  NEO_TILE_TOP   + NEO_TILE_LEFT   + NEO_TILE_ROWS   + NEO_TILE_PROGRESSIVE +
  NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_COLUMNS + NEO_MATRIX_ZIGZAG,
  PIXEL_TYPE);
  
 void drawBitmap(int16_t x, int16_t y, uint8_t *bitmap, int16_t w, int16_t h, uint16_t color);
 void fillScreen(uint16_t color);
 int frames = 12;
 uint8_t anim[12][32];  //blank matrix for holding animation;
 
void setup() {
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(30);
  matrix.setTextColor(matrix.Color(80,255,0));
}

  void loop() {
    char anim = 'm';
 Serial.println("LED WORKING !!!!!!!!!");
   // LEDdefiner('s');
    matrix.setCursor(0, 0);
    
  //  CURRENT(125);
//    delay(5000);
 //   CURRENT(23);
  //  delay(5000);
    //hi();
   // RAIN();
//    SNOW();
    //arrow();
    hilo(120,45);
}
  
void RAIN(){
     for (int i = 0; i < frames ; i ++ ){
    matrix.fillScreen(matrix.Color(0,0,0));
    matrix.drawBitmap(0,0, rain[i],16,16,matrix.Color(250,250,250));
    matrix.show();
    delay(120);
}
}

void SNOW(){
     for (int i = 0; i < frames ; i ++ ){
    matrix.fillScreen(matrix.Color(0,0,0));
    matrix.drawBitmap(0,0, snow[i],16,16,matrix.Color(250,250,250));
    matrix.show();
    delay(120);
}
}

void SUN_LARGE(){
     for (int i = 0; i < frames ; i ++ ){
    matrix.fillScreen(matrix.Color(0,0,0));
    matrix.drawBitmap(0,0, sun_large[i],16,16,matrix.Color(250,250,250));
    matrix.show();
    delay(120);
}
}

void SUN_MEDIUM(){
     for (int i = 0; i < frames ; i ++ ){
    matrix.fillScreen(matrix.Color(0,0,0));
    matrix.drawBitmap(0,0, sun_medium[i],16,16,matrix.Color(250,250,250));
    matrix.show();
    delay(120);
}
}

void THUNDER(){
     for (int i = 0; i < frames ; i ++ ){
    matrix.fillScreen(matrix.Color(0,0,0));
    matrix.drawBitmap(0,0, thunder[i],16,16,matrix.Color(250,250,250));
    matrix.show();
    delay(120);
}
}

void WIND_LARGE(){
     for (int i = 0; i < frames ; i ++ ){
    matrix.fillScreen(matrix.Color(0,0,0));
    matrix.drawBitmap(0,0, wind_large[i],16,16,matrix.Color(250,250,250));
    matrix.show();
    delay(120);
}
}

void WIND_LOW(){
     for (int i = 0; i < frames ; i ++ ){
    matrix.fillScreen(matrix.Color(0,0,0));
    matrix.drawBitmap(0,0, wind_low[i],16,16,matrix.Color(250,250,250));
    matrix.show();
    delay(120);
}
}

void WIND_MEDIUM(){
     for (int i = 0; i < frames ; i ++ ){
    matrix.fillScreen(matrix.Color(0,0,0));
    matrix.drawBitmap(0,0, wind_medium[i],16,16,matrix.Color(250,250,250));
    matrix.show();
    delay(120);
}
}



void hi(){
    matrix.fillScreen(matrix.Color(0,0,0));
    matrix.setTextColor(matrix.Color(255,255,255));
    //code for Hi
    //code for H
    matrix.drawLine(0, 0, 0, 6, matrix.Color(250,250,250));
    matrix.drawLine(2, 0, 2, 6, matrix.Color(250,250,250));
    matrix.drawPixel(1,3, matrix.Color(250,250,250));
    //code for i
    matrix.drawLine(4, 3, 4, 6, matrix.Color(250,250,250));
    matrix.drawPixel(4,1, matrix.Color(250,250,250));
    matrix.setCursor(5,0);
    matrix.print (10);
    //code for Lo
    matrix.drawLine(0, 9, 0, 15, matrix.Color(250,250,250));
    matrix.drawLine(1, 15, 2, 15, matrix.Color(250,250,250));
    matrix.drawRect(2, 12, 3, 4, matrix.Color(250,250,250));
    matrix.setCursor(5,9);
    matrix.print(77);
    matrix.show();
}

void arrow(int hi, int low){
    matrix.fillScreen(matrix.Color(0,0,0));
    matrix.setTextColor(matrix.Color(255,255,255));
    //code for up triangle
    matrix.fillTriangle(2, 2, 0, 4, 4,4, matrix.Color(0,255,0));
    matrix.setCursor(5,0);
    matrix.print (String(hi));
    //code for Lo
    matrix.fillTriangle(0,12,4,12,2,14, matrix.Color(255,0,0));
    matrix.setCursor(5,9);
    matrix.print(String(low));
    matrix.show();
}

void CURRENT(float curr){
    matrix.fillScreen(matrix.Color(0,0,0));
    matrix.setTextColor(matrix.Color(255,255,255));
    matrix.setTextSize(1);
    matrix.setTextWrap(false);
    matrix.setCursor(0,0);
    //printing now
    matrix.print ("N");
    matrix.drawRoundRect(6,2,4,5,1,matrix.Color(250,250,250));
    matrix.setCursor(11,0);
     matrix.print ("w");
    
    if (curr<100){
        matrix.setCursor(3,9);
        matrix.print(String(curr));
    }
    else {
        curr -= 100;
        matrix.drawLine(1,9,1,15,matrix.Color(250,250,250));
        matrix.drawLine(0,15,2,15,matrix.Color(250,250,250));
        matrix.drawPixel(0,10,matrix.Color(250,250,250));
        matrix.setCursor(4,9);
        matrix.print(String(curr));
    }
    matrix.show();
}

void hilo(int hi, int lo){
    matrix.fillScreen(matrix.Color(0,0,0));
    matrix.setTextColor(matrix.Color(255,255,255));
    matrix.setTextSize(1);
    matrix.setTextWrap(false);
    matrix.fillTriangle(0,7,7,0,14,7,matrix.Color(0,255,0));
    if (hi<100){
        matrix.setCursor(2,9);
        matrix.print(String(hi));
    }
    else {
        hi -= 100;
        matrix.drawLine(1,9,1,15,matrix.Color(250,250,250));
        matrix.drawLine(0,15,2,15,matrix.Color(250,250,250));
        matrix.drawPixel(0,10,matrix.Color(250,250,250));
        matrix.setCursor(4,9);
        matrix.print(String(hi));
    }
    matrix.show();
    
    delay(3000);
    
    matrix.fillScreen(matrix.Color(0,0,0));
    matrix.setTextColor(matrix.Color(255,255,255));
    matrix.setTextSize(1);
    matrix.setTextWrap(false);
    matrix.fillTriangle(0,0,7,7,14,0,matrix.Color(255,0,0));
    if (lo<100){
        matrix.setCursor(2,9);
        matrix.print(String(lo));
    }
    else {
        hi -= 100;
        matrix.drawLine(1,9,1,15,matrix.Color(250,250,250));
        matrix.drawLine(0,15,2,15,matrix.Color(250,250,250));
        matrix.drawPixel(0,10,matrix.Color(250,250,250));
        matrix.setCursor(4,9);
        matrix.print(String(lo));
    }
    matrix.show();
    delay(3000);
}

