
#include <ESP8266WiFi.h>
#include "HX711.h"

HX711 scale(2, 0);

float weight;
//float calibration_factor = -101525;
float calibration_factor = 219640;
void setup() {
  Serial.begin(9600);

  scale.set_scale();
  scale.tare(); //Reset the scale to 0
  long zero_factor = scale.read_average(); //Get a baseline reading
 
  delay(3000);
 

}

void loop() {
  // put your main code here, to run repeatedly:
   scale.set_scale(calibration_factor);
    weight = scale.get_units(5); 
    Serial.print("Berat : ");
    Serial.println(weight);
    delay(500);
//     scale.set_scale();
//  scale.tare();

}
