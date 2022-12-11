#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "DHT.h"
//-----------------------------------------------------------------
// Config fyrir NRF24 útvarpsendi
static char send_payload[256];

const int min_payload_size = 16;
const int max_payload_size = 32;
const int payload_size_increments_by = 1;
int next_payload_size = min_payload_size;

// CE = pin 22, CSN = pin 21
RF24 radio(22, 21 );

// Addressur ein til að hlusta á og hin til að skrifa á sem v+ixlast síðan á hinni nóðuni
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };
char receive_payload[max_payload_size + 1];
String message = "";
//-----------------------------------------------------------------
// AM2301 config
#define DHTPIN 15    // modify to the pin we connected
#define DHTTYPE DHT21   // AM2301 
DHT dht1(DHTPIN, DHTTYPE);
//-----------------------------------------------------------------
// DHT22
#define DHTPIN 32  // modify to the pin we connected
#define DHTTYPE DHT22   // AM2301 
DHT dht2(DHTPIN, DHTTYPE);
//-----------------------------------------------------------------
// Neðri vatns hæð
#define POWER_PINned  17 // ESP32 pin GIOP17 connected to sensor's VCC pin
#define SIGNAL_PINned 36 // ESP32 pin GIOP36 (ADC0) connected to sensor's signal pin
int vatnsnedri = 0; // variable to store the sensor value
//-----------------------------------------------------------------
// Efri vatns hæð
#define POWER_PINefr  5 // ESP32 pin GIOP17 connected to sensor's VCC pin
#define SIGNAL_PINefr 39 // ESP32 pin GIOP36 (ADC0) connected to sensor's signal pin
int vatnsefr = 0; // variable to store the sensor value

void setup()
{
  Serial.begin(115200);

  // ----- Radio Setup ----------
  radio.begin();
  // enable dynamic payloads
  radio.enableDynamicPayloads();
  radio.setRetries(5, 15);

  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1, pipes[1]);
  radio.startListening();
  // ----- DHT Setup ----------
  dht1.begin(); // AM2301
  dht2.begin(); // DHT22
  // vatns ned
  pinMode(POWER_PINned, OUTPUT);   // configure pin as an OUTPUT
  digitalWrite(POWER_PINned, LOW); // turn the sensor OFF
  // vatns efr 
  pinMode(POWER_PINefr, OUTPUT);   // configure pin as an OUTPUT
  digitalWrite(POWER_PINefr, LOW); // turn the sensor OFF
}

String lesaAM2301()
{
 float h = dht1.readHumidity();
 float t = dht1.readTemperature();
 // check if returns are valid, if they are NaN (not a number) then something went wrong!
 if (isnan(t) || isnan(h)) 
 {
   String response = "Failed to read from DHT";
   return response;
 } 
 else
 {
   String hu = String(h);
   String te = String(t);
   hu.replace(".","");
   te.replace(".","");
   hu = hu.substring(0,3);
   te = te.substring(0,3); 
   String response = hu +  te;
   return response;
 }
}

String lesaDHT22()
{
 float h = dht2.readHumidity();
 float t = dht2.  readTemperature();
 // check if returns are valid, if they are NaN (not a number) then something went wrong!
 if (isnan(t) || isnan(h)) 
 {
   String response = "Failed to read from DHT";
   return response;
 } 
 else
 {
   String hu = String(h);
   String te = String(t);
   hu.replace(".","");
   te.replace(".","");
   hu = hu.substring(0,3);
   te = te.substring(0,3); 
   String response = hu +  te;
   return response;
 }
}

int vatnshned()
{
  digitalWrite(POWER_PINned, HIGH);  // turn the sensor ON
  delay(10);                      // wait 10 milliseconds
  vatnsnedri = analogRead(SIGNAL_PINned); // read the analog value from sensor
  digitalWrite(POWER_PINned, LOW);   // turn the sensor OFF

  int val = map(vatnsnedri, 0, 1300, 0, 45);
  return val;

}

int vatnshefr()
{
  digitalWrite(POWER_PINefr, HIGH);  // turn the sensor ON
  delay(10);                      // wait 10 milliseconds
  vatnsefr = analogRead(SIGNAL_PINefr); // read the analog value from sensor
  digitalWrite(POWER_PINefr, LOW);   // turn the sensor OFF

  int val = map(vatnsefr, 0, 1300, 0, 45);
  return val;

}

void sendstreng(String str) 
{

  static char send_payload[50];
  str.toCharArray(send_payload, 50);
  Serial.println(send_payload);

  // First, stop listening so we ca n talk.
  radio.stopListening();

  // Take the time, and send it.  This will block until complete
  Serial.print(F("Now sending length "));
  Serial.println(next_payload_size);
  radio.write(send_payload, next_payload_size );
}


void loop()
{
  radio.startListening();
  
  if (radio.available())
  {
    char text[16] = {0};
    radio.read(&text, sizeof(text));
    //Serial.println(text);
    String mystr = String(text);
    String message = mystr.substring(0,16);
    Serial.println(message);
    if (message == "senddata00000000") {
    int vatn = vatnshned()+vatnshefr();
    if (vatn > 100){
      vatn=99;
    }
    else if (vatn < 10){
      vatn = 10;
    }
    Serial.println("Data generation");
    String lAM23 = lesaAM2301();
    String lDht22 = lesaDHT22();
    sendstreng(lDht22 + lAM23 + vatn + "11");
    }
    
  }



}