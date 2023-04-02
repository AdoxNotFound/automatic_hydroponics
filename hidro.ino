/*
install libraries 
* U8g2
* DHT sensor library
* adafruit unified sensor
* onewire
* Dallastemperature
*/

#include <DHT.h>
#include <U8g2lib.h>
#include <U8x8lib.h>
#include <OneWire.h>
#include <DallasTemperature.h>

char temperature [5];
char humidity [5];
char water_temp [5];
const char DEGREE_SYMBOL[] = { 0xB0, '\0'};

DHT dht(7, DHT11); //define pin 7 as input of dht11 sensor
U8G2_ST7920_128X64_1_HW_SPI u8g2(U8G2_R0, /* CS=*/ 10, /* reset=*/ 8); //params for LCD 128x64 ST7920

OneWire oneWireObjet(6);
DallasTemperature sensorDS18B20(&oneWireObjet);
int flag = 0;
void setup() {
  dht.begin();
  Serial.begin(9600);
  u8g2.begin();
  u8g2.enableUTF8Print();
  u8g2.setFont(u8g2_font_helvB08_tf); 
  u8g2.setColorIndex(1);  
  sensorDS18B20.begin(); 
  sensorDS18B20.begin();     
  pinMode(2, INPUT_PULLUP);
  pinMode(3, OUTPUT);
  u8g2.clearBuffer();
}

void loop() {
  u8g2.firstPage();
  do {   
    draw();   
  } while( u8g2.nextPage() );
  delay(100);
}

void draw(){
  readTemperature();
  readHumidity();
  readWaterTemp();

  if (digitalRead(2) == HIGH){
    digitalWrite(3, LOW);
    flag = 0;
  }
  else{
    digitalWrite(3, HIGH);
    flag = 1;        
  }
        
  u8g2.drawStr(2,10, "17:02:02 30/03/2023");

  u8g2.drawStr(2, 20, "Temp. amb:");   
  u8g2.drawStr(60, 20, temperature);   
  u8g2.drawUTF8(82, 20, DEGREE_SYMBOL);
  u8g2.drawUTF8(88, 20, "C");

  u8g2.drawStr(2, 30, "Humedad:");         
  u8g2.drawStr(52, 30, humidity); 
  u8g2.drawStr(75,30, "%");  
  
  u8g2.drawStr(2,40, "Nivel agua: ");
  
  u8g2.drawStr(2, 50, "Temp. agua:");
  u8g2.drawStr(65, 50, water_temp);   
  u8g2.drawUTF8(90, 50, DEGREE_SYMBOL);
  u8g2.drawUTF8(96, 50, "C");  
  
  u8g2.drawStr(2, 60, "Estado: motor");  
  if (flag == 0){
  u8g2.drawStr(75, 60, "apagado");
  }
  else{
  u8g2.drawStr(75, 60, "encendido");    
  }
}

void readTemperature()
{
  float t = dht.readTemperature();
  dtostrf(t, 3, 1, temperature);
}

void readHumidity()
{
  float h = dht.readHumidity();
  dtostrf(h, 3, 1, humidity);
}

void readWaterTemp()
{
  sensorDS18B20.requestTemperatures();
  float wt = sensorDS18B20.getTempCByIndex(0);
  dtostrf(wt, 3, 1, water_temp);  
}
