/**
 *
 * HX711 library for Arduino - example file
 * https://github.com/bogde/HX711
 *
 * MIT License
 * (c) 2018 Bogdan Necula
 *
**/
#include "HX711.h"


// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 2;
const int LOADCELL_SCK_PIN =0;
const int tombol = 16;
const int speaker = 5;


HX711 scale;

void setup() {
  Serial.begin(38400);
  Serial.println("HX711 Demo");

  Serial.println("Initializing the scale");
  pinMode(tombol, INPUT);
  pinMode(speaker, OUTPUT);

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  Serial.println("Before setting up the scale:");
  Serial.print("read: \t\t");
  Serial.println(scale.read());			// print a raw reading from the ADC


  Serial.print("get value: \t\t");
  Serial.println(scale.get_value(5));		// print the average of 5 readings from the ADC minus the tare weight (not set yet)

  Serial.print("get units: \t\t");
  Serial.println(scale.get_units(5), 1);	// print the average of 5 readings from the ADC minus tare weight (not set) divided
						// by the SCALE parameter (not set yet)

  scale.set_scale(2124.f);                      // this value is obtained by calibrating the scale with known weights; see the README for details
  scale.tare();				        // reset the scale to 0


}

void loop() {
 int tbl =  digitalRead(tombol);
// int spk = digitalWrite(speaker);

  if(tbl == 1)
  {
    Serial.println("ok");
    digitalWrite(speaker, HIGH);
    delay(500);
    digitalWrite(speaker, LOW);
  }
  else{
    Serial.println("No");
  }
 const int berat =  scale.get_units();
//  Serial.print(scale.set_scale(2000.f));
  Serial.print("ASli:\t");
  Serial.println(berat);
 if(berat < 1000)
  {
    Serial.print("Berat:\t");
    Serial.print(berat);
     Serial.println("Gram");
  }
  else if(berat >= 1000 && berat <= 5000)
  {
    Serial.print("Berat:\t");
   float kg = 1000 ;
    kg = berat/kg;
    Serial.print(kg);
     Serial.println("Kg");
  }
  else
  {
    if(berat >5000)
    {
      Serial.print("Timbangan Full");
    }
  }

//  scale.power_down();			        // put the ADC in sleep mode
  delay(500);
//  scale.power_up();
}
