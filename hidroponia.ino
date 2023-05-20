/* 
Libraries needed for running this code:
  * U8g2
  * Adafruit Unified Sensor
  * DHT sensor library
  * RTClib
  * OneWire
  * Dallas Temperature
*/

#include <Wire.h>    // I2C interface
#include <RTClib.h>   // RTC library
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

RTC_DS3231 rtc;     // create object RTC_DS3231 type

# define RELE 3     // digital pin 3 for relee

int flag = 0;

bool event_start = true;  // variable de control para inicio de evento con valor true
bool event_end = true; 

void setup () {
 Serial.begin(9600);    // inicializa comunicacion serie a 9600 bps
 pinMode(RELE, OUTPUT);   // pin 3 como salida

if (! rtc.begin()) {       // si falla la inicializacion del modulo
  Serial.println("Modulo RTC no encontrado !");  // muestra mensaje de error
  while (1);         // bucle infinito que detiene ejecucion del programa
}
 //rtc.adjust(DateTime(__DATE__, __TIME__));  //use only for setting hour
dht.begin();
u8g2.begin();
u8g2.enableUTF8Print();
u8g2.setFont(u8g2_font_helvB08_tf); 
u8g2.setColorIndex(1);  
sensorDS18B20.begin(); 
pinMode(2, INPUT_PULLUP);
pinMode(4, INPUT_PULLUP);
u8g2.clearBuffer();
 
}

void loop () {
  u8g2.firstPage();
  do {   
    draw();   
    } while( u8g2.nextPage() );
}

void draw(){
  DateTime date_stamp = rtc.now();
  int c_year = date_stamp.year();
  int c_month = date_stamp.month();  
  int c_day = date_stamp.day();
  int c_hour = date_stamp.hour();
  int c_minute = date_stamp.minute();
  int c_second = date_stamp.second();
  
  readTemperature();
  readHumidity();
  //readWaterTemp();

  if ((digitalRead(2) == HIGH && digitalRead(4) == LOW) || (digitalRead(2) == LOW && digitalRead(4) == HIGH)){ //botones intercalados
    digitalWrite(RELE, LOW); 
    flag=0;
  }
  if ((digitalRead(2) == LOW && digitalRead(4) == LOW)){ //ambos botones arriba
    flag = 1;
    digitalWrite(RELE, HIGH); 
  }
  if ((digitalRead(2) == HIGH && digitalRead(4) == HIGH)){ //ambos botones abajo
    if (((c_hour >= 7 && c_hour <= 10) || (c_hour >= 16 && c_hour <= 18)) && (c_minute == 00) && (c_second == 00)){
      if (event_start == true){     // si evento_inicio es verdadero
        digitalWrite(RELE, HIGH);       // activa modulo de rele con nivel alto
        event_start = false;
        event_end = true;
        flag = 1;
      }       
    }
    if (((c_hour >= 7 && c_hour <= 10) || (c_hour >= 16 && c_hour <= 18)) && (c_minute == 00) && (c_second == 45)){
      if (event_end == true){        // si evento_fin es verdadero
        digitalWrite(RELE, LOW);        // desactiva modulo de rele con nivel bajo
        event_end = false;
        event_start = true;
        flag=0;// carga valor falso en variable de control
      }         
    }
    if ((c_hour >= 11) && (c_hour <= 15) && (c_minute == 00 || c_minute == 30) && (c_second == 00)){
      if (event_start == true){     // si evento_inicio es verdadero
        digitalWrite(RELE, HIGH);       // activa modulo de rele con nivel alto
        event_start = false;
        event_end = true;
        flag = 1;
      }       
    }
    if ((c_hour >= 11) && (c_hour <= 15) && (c_minute == 00 || c_minute == 30) && (c_second == 45)){
      if (event_end == true){        // si evento_fin es verdadero
        digitalWrite(RELE, LOW);        // desactiva modulo de rele con nivel bajo
        event_end = false;
        event_start = true;
        flag=0;// carga valor falso en variable de control
      }        
    }
  }
  //Configuracion para mosntrar informacion en pantalla LCD
  u8g2.setCursor(2,10);
  u8g2.print(c_hour);
  u8g2.drawStr(15,10,":");
  u8g2.setCursor(19,10);
  u8g2.print(c_minute);
  u8g2.drawStr(31,10,":");
  u8g2.setCursor(35,10);
  u8g2.print(c_second);
  
  u8g2.setCursor(55,10);
  u8g2.print(c_day);
  u8g2.drawStr(68,10,"/");
  u8g2.setCursor(72,10);
  u8g2.print(c_month);
  u8g2.drawStr(85,10,"/");
  u8g2.setCursor(89,10);
  u8g2.print(c_year);
  
  u8g2.drawStr(2, 20, "Temp. amb:");   
  u8g2.drawStr(60, 20, temperature);   
  u8g2.drawUTF8(82, 20, DEGREE_SYMBOL);
  u8g2.drawUTF8(88, 20, "C");

  u8g2.drawStr(2, 30, "Humedad:");         
  u8g2.drawStr(52, 30, humidity); 
  u8g2.drawStr(75,30, "%");  
  
  u8g2.drawStr(2,40, "Nivel agua: ");
  
  u8g2.drawStr(2, 50, "Temp. agua:");
  //u8g2.drawStr(65, 50, water_temp);   
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
