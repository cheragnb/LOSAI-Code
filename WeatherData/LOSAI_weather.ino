#include "ctype.h"

bool weathergood = false;
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

#define A 1
void setup() {
    Serial.begin(9600);
    Particle.subscribe("hook-response/weather_openWeatherMaps", currentWeatherHandler , MY_DEVICES);
    Particle.subscribe("hook-response/epochTime_timeZoneDB", epochTimeHandler, MY_DEVICES);
    Particle.subscribe("hook-response/forecast16_openWeatherMaps", dailyMinMaxHandler, MY_DEVICES);
    Particle.subscribe("hook-response/predict_openWeatherMaps", hourlyHandler, MY_DEVICES);
}

void loop() {
     if (personInView()){
        //TODO add stuff
    }
    if ( millis() > millis_current + check_time ) {
        getData();  //gets data ever specified minute interval
        choseAnimation();

    }
}

void currentWeatherHandler(const char *event, const char *currentData) {
    animation_seq = "";
    temp_seq = "";
    Serial.println("Call made for current weather");
        String str = String(currentData); // example: "~10dn~25.79~"" (icon~current temp)
    Serial.println(str);
    char strBuffer[25]= "";
    str.toCharArray(strBuffer, 25);
    String weather_condition_current = strtok(strBuffer, "~");
    String temp_current = strtok(NULL,"~");
    //Serial.println(weather_condition_current);
    //TODO figure out repeat weather conditiojn
    //  Serial.println(temp_current);
    animation_seq = animation_seq + "~" + weather_condition_current + "~";
    temp_seq = temp_seq + "~" + temp_current + "~";
    //    Serial.println("----------");
}

void epochTimeHandler (const char*event, const char*epochData){
    //current epoch time, we dont need this
    String api_status;
    Serial.println("Call made for time");
        String str = String(epochData);
    char strBuffer[15]= "";
    str.toCharArray(strBuffer, 15); // example: "~OK~12121212121~"
    api_status = strtok(strBuffer, "~");
    epoch_time = atol(strtok(NULL, "~"));
    /*Serial.println(api_status);*/
    //  Serial.println(epoch_time);
}

void dailyMinMaxHandler(const char *event, const char *dailyMinMaxData) {
    //max min temp for 2 days. We probably dont need this.
    Serial.println("Call made for weather forecast");
        String str = String(dailyMinMaxData); // example: "(epoch time day 1 ~ daily high ~ daily low ~ epoch time day 2 ~ daily high ~ daily low)"
    //  Serial.println(str);
    char strBuffer[50]= "";
    str.toCharArray(strBuffer,50);
    long epoch_time_1 = atol(strtok(strBuffer, "~"));
    float temp_max_1 = atof(strtok(NULL, "~"));
    float temp_min_1 = atof(strtok(NULL, "~"));
    long epoch_time_2 = atol(strtok(NULL, "~"));
    float temp_max_2 = atof(strtok(NULL, "~"));
    float temp_min_2 = atof(strtok(NULL, "~"));
    /*Serial.println(epoch_time_1);
    Serial.println(temp_max_1);
    Serial.println(temp_min_1);
    Serial.println(epoch_time_2);
    Serial.println(temp_max_2);
    Serial.println(temp_min_2);
    */
}

int hourlyHandler (const char*event, const char*predict_data){
    String str = String (predict_data);
    Serial.println("Call made for prediction");
    Serial.println(str);
    char strBuffer[150]= "";
    str.toCharArray(strBuffer,150);
    String epoch_time_plus3 = (strtok(strBuffer, "~"));
    String temp_plus3 = (strtok(NULL, "~"));
    String weather_condition_plus3 = strtok(NULL, "~");
    String epoch_time_plus6 = (strtok(NULL, "~"));
    String temp_plus6 = (strtok(NULL, "~"));
    String weather_condition_plus6 = strtok(NULL, "~");
    String epoch_time_plus9 = (strtok(NULL, "~"));
    String temp_plus9 = (strtok(NULL, "~"));
    String weather_condition_plus9 = strtok(NULL, "~");
    String epoch_time_plus12 = (strtok(NULL, "~"));
    String temp_plus12 = (strtok(NULL, "~"));
    String weather_condition_plus12 = strtok(NULL, "~");
    animation_seq =animation_seq + weather_condition_plus3 + "~"+ weather_condition_plus6 + "~" + weather_condition_plus9 + "~" + weather_condition_plus12 + "~" + "EOL" ;
    temp_seq = temp_seq + temp_plus3 + "~" + temp_plus6 + "~"+ temp_plus9 + "~"+ temp_plus12 + "~" + "EOL"; //mark end of line
}

void getData(void){
    long wait = millis();
    Serial.println("Begin Data Collection \n");
    bool condition = false;
    int count = 0;
    while ( condition != true && count < 10) { //try untill we get the right data string
        //publish in this order to maintain string
        Particle.publish("weather_openWeatherMaps", currentData, PRIVATE); // Trigger the weather webhook
        //    Particle.publish("epochTime_timeZoneDB", epochData, PRIVATE); // Trigger the time webhook
        //    Particle.publish("forecast16_openWeatherMaps", dailyMinMaxData, PRIVATE); // Trigger the forecast webhook
        //while((millis() < wait + 1000UL))      {Spark.process();}
        Particle.publish("predict_openWeatherMaps", predict_data, PRIVATE); // Trigger the prediction webhook
        while((millis() < wait + 1000UL))      {Spark.process();}
        if (animation_seq.length() > 19 && temp_seq.length() > 29)
        {
            condition = true;
            break;
        }
        //ensure constant animation codes*/
        count ++;
        if (count > 9) { //give error if error in recieving data
            animation_seq = "~err~EOL";
            temp_seq = "~err~EOL";
            break;
        }
    }
    Serial.println(animation_seq);
    Serial.println(temp_seq);
    Serial.println("End Data Collection \n");
    millis_current = millis();  //reset timerk
}

bool personInView(){
    //TODO setup sensor currently working on trinket
    if (digitalRead(D0)){
        //  Serial.println("Person Detected!");
        return true;
    }
    else return false;
    }

void displayAnimation(String animation_code)
{
    Serial.println(animation_code);
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
