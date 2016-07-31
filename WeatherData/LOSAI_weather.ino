
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
//    choseAnimation();
  }
  }

void currentWeatherHandler(const char *event, const char *currentData) {
    animation_seq = "";
    temp_seq = "";
    Serial.println("Call made for current weather");
    String str = String(currentData); // example: "~10dn~25.79~"" (icon~current temp)
  //  Serial.println(str);
    char strBuffer[15]= "";
    str.toCharArray(strBuffer, 15);
    String weather_condition_current = strtok(strBuffer, "~");
    String temp_current = (strtok(NULL, "~"));
    animation_seq = animation_seq + "~" + weather_condition_current + "~";
    temp_seq = temp_seq + "~" + temp_current + "~";
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
    //Serial.println(str);
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

char choseAnimation (){
    /* char animation
    d = clear day n = clear night c = clouds  r = rain    t = thunder s = snow    m = mist   x = error */
  //char animation = 'x';
    String animation[4];
    char strBuffer[20]= "";
    animation_seq.toCharArray(strBuffer, 20);
    animation[0] = strtok(strBuffer, "~");
    animation[1] = strtok(NULL, "~");
    animation[2] = strtok(NULL, "~");
    animation[3] = strtok(NULL, "~");
    int i = 0;
    String animation_prev = "";
    while (animation[i] != "EOL" ){
    if (animation[i] != animation_prev){
      display_animation(animation[i]);
    }
    animation_prev = animation[i];
    i ++;
  }
/*
    Original code
    //check for clear day or night
    if (weather_condition_current == "01d")    {animation = 'd';}
    else if (weather_condition_current == "01n")   {animation = 'n';}
    int status_trim = weather_condition_current.toInt();

    //check rest of the conditions
    if (status_trim == 2 || status_trim == 3 ||status_trim == 4){    animation = 'c' ;   }
    else if (status_trim == 9 || status_trim == 10){    animation = 'r'; }
    else if (status_trim == 11 ){ animation = 't' ; }
    else if (status_trim == 13 ){animation = 's'; }
    else if (status_trim == 50 ){animation = 'm' ;}
    else {animation = 'x' ;}
    return animation ;
  }*/
}


void getData(void){
  long wait = millis();
  Serial.println("Begin Data Collection \n");
  bool condition = false;
  int count = 0;
  while ( condition != true || count > 10) { //try untill we get the right data string
  //publish in this order to maintain string
  Particle.publish("weather_openWeatherMaps", currentData, PRIVATE); // Trigger the weather webhook
  Particle.publish("epochTime_timeZoneDB", epochData, PRIVATE); // Trigger the time webhook
  Particle.publish("forecast16_openWeatherMaps", dailyMinMaxData, PRIVATE); // Trigger the forecast webhook
  Particle.publish("predict_openWeatherMaps", predict_data, PRIVATE); // Trigger the prediction webhook
  while((millis() < wait + 3000UL))      {Spark.process();}
  Serial.println(animation_seq);
  Serial.println(temp_seq);

  /*Serial.println(animation_seq.length());
  Serial.println(animation_seq.substring(21));
  Serial.println(temp_seq.length());
  Serial.println(temp_seq.substring(31));
*/

  if ((animation_seq.length() == 24 && animation_seq.substring(21) == "EOL")&&
  (temp_seq.length() == 34 && temp_seq.substring(31) == "EOL"))
  {
    condition = true;
    break;
  } //ensure constant animation codes*/
  count ++;
}
 if (count == 10 && condition == false) {
  animation_seq = "~err~EOL";
  temp_seq = "~err~EOL";
}
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

void display_animation(String animation_code)
{
Serial.println(animation_code);
}
