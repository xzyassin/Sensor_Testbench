#include "NewPing.h"
#include "DHT.h"
#include "Adafruit_TCS34725.h"
#include "Wire.h"


#define TRIGGER_PIN   10
#define ECHO_PIN      13
#define MAX_DISTANCE  400
#define DHTPIN 2
#define DHTTYPE DHT11


NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
DHT dht(DHTPIN, DHTTYPE);

/*Constructor, GAIN ist die Signalverstärkung, Integration Time ist ein Delay
  nach dem Einlesen (Durch Timerinterrupt ersetzen/ integration selber machen) */
Adafruit_TCS34725 tcs1 =Adafruit34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_1X);
Adafruit_TCS34725 tcs2 =Adafruit34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_1X);

//mit funktionsübergabewerten arbeiten
float dauer, distanz, hum, temp, SchallGeschwcm;
float R1,G1,B1,R2,G2,B2,B3;
// als Struct speichern, siehe unten.
struct Color = {
        float red,green,blue;
};
float SchallGeschw ;  //cm/ms
int iterations = 5;
const uint16_t MAX_RANGE = 300;      //cm
const uint16_t MIN_RANGE = 2;        //cm

void changeBus(uint8_t bus)
{
    Wire.beginTransmission(0x70); // TCA9548A adress is 0x70
    Wire.write(1 << bus);        //send byte to select bus
    Wire.endTransmission();
}

//Struct color als Eingabevariable (Returnparameter)
void setRGB(Adafruit_TCS34725 *tcs,float R,float G,float B) {

    tcs.setInterrupt(false); //LED an. Wire the LED pin to the INT pin and control with setInterrupt()
    tcs.getRGB(R, G, B);
    tcs.setInterrupt(true); //LED aus.
}

void PrintOnDisplay(struct Color color){

}

void setup() {

    TCA9548A(1);
    {
        if (tcs1.begin()) {
            Serial.print(" Erster Sensor gefunden.");
        } else {
            Serial.print(" TCS34725/1 nicht gefunden..Verbindung überprüfen! ");
            while (1);
        }
    }

    TCA9548A(2);
    {
        if (tcs2.begin()) {
            Serial.print(" Zweiter Sensor gefunden.");
        } else {
            Serial.print(" TCS34725/2 nicht gefunden..Verbindung überprüfen! ");
            while (1);
        }
    }

  dht.begin();
}

void loop() {

    TCA9548A(1);
    setRGB(tcs1,R1,G1,B1);

    TCA9548A(2);
    setRGB(tcs2,R2,G2,B2);

    PrintOnDisplay(R1,G1,B1);
    PrintOnDisplay(R2,G2,B2);


delay(50);

hum = dht.readHumidity();
temp = dht.readTemperature();

//konstanten definieren bzw. eine Funktion erstellen
SchallGeschw= 331.4 + (0.606 * temp) + (0.0124 * hum);
SchallGeschwcm = SchallGeschw /10000;

/*
Serial.print("Humidity: ");
Serial.print(hum);
Serial.print(" %, Temp: ");
Serial.print(temp);
Serial.println(" Celsius");

*/

dauer = sonar.ping_median(iterations);
distanz = (dauer/2)*SchallGeschwcm;

Serial.print("Distanz = ");
if (distanz>=MaxRange || distanz<=MinRange){
  Serial.println("Ausser Reichweite");
} else {
  Serial.print(distanz);
  Serial.println(" cm");
  delay(100);
}

delay(100);

}
