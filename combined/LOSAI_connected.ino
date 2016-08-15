// for LED Screen
#include "Adafruit_GFX/Adafruit_GFX.h"
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


//For Weather Data
#include "ctype.h"
//bool weathergood = false;
float temp_current = -1;
String weather_condition_current= "blank";
long check_time = 0.2*60*1000;
int millis_current = millis();
String currentData = String(10);
String dailyMinMaxData = String(10);
String epochData = String(10);
String predict_data = String(10);
long epoch_time;
String animation_seq;
String temp_seq;
char final_animations[]= {'e','e','e','e','e'};
float final_temps[3]; //= {-400,-400,-400}; //impossible value
bool currentHappened;
bool predictHappened;
int led = 7 ;

 
void setup() {
     Serial.begin(9600);
     pinMode(led,OUTPUT);
    pinMode(0,INPUT);
    matrix.begin();
    matrix.setTextWrap(false);
    matrix.setBrightness(30);
    matrix.setTextColor(matrix.Color(80,255,0));
    matrix.fillScreen(matrix.Color(0,0,0));
    matrix.show();
     
    Particle.subscribe("hook-response/weather_openWeatherMaps", currentWeatherHandler , MY_DEVICES);
    Particle.subscribe("hook-response/predict_openWeatherMaps", hourlyHandler, MY_DEVICES);
}

  void loop() {
  
   Particle.process();
    if (personInView()){
        digitalWrite(led,HIGH);
        displayAnimation();
    }
    //else    digitalWrite(led,LOW);
    Particle.process();
    if ( millis() > millis_current + check_time ) {
        digitalWrite(led,HIGH);
        getData_1();  //gets data ever specified minute interval
        choseAnimation();
        digitalWrite(led,LOW);
    } 
}

///////////////         Weather Collection Functions                ///////////////////////////////////////////////////////////////

void currentWeatherHandler(const char *event, const char *currentData) {
    animation_seq = "";
    temp_seq = "";
    Serial.println("Call made for current weather");
    String str = String(currentData); // example: "~10dn~25.79~"" (icon~current temp)
    //Serial.println(str);
    char strBuffer[25]= "";
    str.toCharArray(strBuffer, 25);
    String weather_condition_current = strtok(strBuffer, "~");
    String temp_current = strtok(NULL,"~");
    //Serial.println(weather_condition_current);
    //TODO figure out repeat weather conditiojn
    //  Serial.println(temp_current);
    animation_seq = animation_seq + "~" + weather_condition_current + "~";
    temp_seq = temp_seq + "~" + temp_current + "~";
    currentHappened = true;
}

int hourlyHandler (const char*event, const char*predict_data){
    String str = String (predict_data);
    Serial.println("Call made for prediction");
   // Serial.println(str);
    char strBuffer[150]= "";
    str.toCharArray(strBuffer,150);
    String temp_plus3 = (strtok(strBuffer, "~"));
    String weather_condition_plus3 = strtok(NULL, "~");
    String temp_plus6 = (strtok(NULL, "~"));
    String weather_condition_plus6 = strtok(NULL, "~");
    String temp_plus9 = (strtok(NULL, "~"));
    String weather_condition_plus9 = strtok(NULL, "~");
    String temp_plus12 = (strtok(NULL, "~"));
    String weather_condition_plus12 = strtok(NULL, "~");
    animation_seq =animation_seq + weather_condition_plus3 + "~"+ weather_condition_plus6 + "~" + weather_condition_plus9 + "~" + weather_condition_plus12 + "~" + "EOL" ;
    temp_seq = temp_seq + temp_plus3 + "~" + temp_plus6 + "~"+ temp_plus9 + "~"+ temp_plus12 + "~" + "EOL"; //mark end of line
    predictHappened = true;
}

bool personInView(){
    //TODO setup sensor currently working on trinket
    if (digitalRead(D0)){
        Serial.println("Person Detected!");
        return true;
    }
    else return false;
    }

void displayAnimation()
{
   CURRENT(final_temps[0]);
   animationPlayer(final_animations[0]);
   animationPlayer(final_animations[1]);
   animationPlayer(final_animations[2]);
   HILO(final_temps[1],final_temps[2]);
}

void choseAnimation(){
    /*find only weather icon changes and consolidate images forecast16_openWeatherMaps
    and convert to usable icons*/
    Serial.println("choose animations");
    char strBuffer[35]= "";

    for (int j = 0 ; j < 5; j ++)   {final_animations[j] = 'e';}
    animation_seq.toCharArray(strBuffer,35);
    int i = 1;
    String value = (strtok(strBuffer, "~"));
    final_animations[0] = converter(value);
    char previous = final_animations[0];
    while (value != "EOL"){
        value = strtok(NULL,"~");
        char temp = converter(value);
        if (temp != previous){
            final_animations[i] = temp;
            previous = temp;
            i ++ ;
        }
    }
    for (i = 0 ; i< 3; i ++){
        Serial.println(final_animations[i]);
    }

    char numBuffer[35]= "";
    
    temp_seq.toCharArray(numBuffer,35);
    final_temps[0] = atof(strtok(numBuffer, "~"));
    float min = 400;
    float max = -400;
    value = "";
    while (true){
        value = (strtok(NULL,"~"));
        if (value == "EOL") break;

        if (atof(value)<= min)  {
            final_temps[1] = atof(value);
            min = final_temps[1];
        }

        if(atof(value)>=max) {
            final_temps[2] = atof(value);
            max = final_temps[2];
        }
    }
    if (final_temps[0] > final_temps[2]) { 
        final_temps[2] = final_temps[0];
        }
    if (final_temps[0] < final_temps[1]) { 
        final_temps[1] = final_temps[0];
        }
    for (i = 0 ; i < 3 ; i ++){
        Serial.println(final_temps[i]);
    }
}

char converter(String raw){
    /*   n = clear night ; c = clear day ;
    l = little cloudy; m = medium clowdy ; v = very cloudy;
    d = drizzle ; r = rain ; t = thunderstorm
    s = snow ; f = foggy/misty;
    x = error ; q = quit ; e = eol*/

    if (raw == "01d") return 'c';
    else if (raw == "01n")  return 'n';
    else if (raw == "02d" || raw == "02n")  return 'l';
    else if (raw == "03d" || raw == "03n")  return 'm';
    else if (raw == "04d" || raw == "04n")  return 'v';
    else if (raw == "09d" || raw == "09n")  return 'd';
    else if (raw == "10d" || raw == "10n")  return 'r';
    else if (raw == "11d" || raw == "11n")  return 't';
    else if (raw == "13d" || raw == "13n")  return 's';
    else if (raw == "50d" || raw == "50n")  return 'f';
    else if (raw == "EOL") return 'e';
    else return 'x';
    }
    
void getData_1(void){
    Serial.println("==================================================================");
    Serial.println("Begin Data Collection");
    currentHappened = false;
    predictHappened = false;
    
        animation_seq = "";
        temp_seq = "";
        long wait = millis();
        while (currentHappened == false){
            Serial.println("CURRENT PUBLISHED");
            Particle.publish("weather_openWeatherMaps", currentData, PRIVATE); // Trigger the weather webhook
            delay(3000);
           }
        while (predictHappened == false){
            Serial.println("PREDICT PUBLISHED");
            Particle.publish("predict_openWeatherMaps", predict_data, PRIVATE); // Trigger the prediction webhook
            delay(3000);
        }
    if ((animation_seq.length() > 25) || (temp_seq.length() > 36)){
        animation_seq = "~err~EOL";
        temp_seq = "~err~EOL";
    }
    Serial.println(animation_seq);
    Serial.println(temp_seq);
    Serial.println("End Data Collection");
    Serial.println("----------------------------------------------------------------------");
    millis_current = millis();
    
}

void animationPlayer(char c){
    switch (c){
    case 'c':
        SUN_MEDIUM();
        break;
    case 'r':
        RAIN();
        break;
    case 's':
        SNOW();
        break;
    case 't':
        THUNDER();
        break;
    default:
        WIND_LOW();
        break;
    }
    
}

////////////////////////////     Animations for the LED screen          /////////////////////////////////////////////////////////////////

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

void HI(){
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
    delay(3000);
    matrix.fillScreen(matrix.Color(0,0,0));
}

void ARROW(int hi, int low){
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
    delay(3000);
    matrix.fillScreen(matrix.Color(0,0,0));
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
    delay(3000);
    matrix.fillScreen(matrix.Color(0,0,0));
}

void HILO(int lo, int hi){
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
    matrix.fillScreen(matrix.Color(0,0,0));
    matrix.show();
}

