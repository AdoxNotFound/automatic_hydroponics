/* 
Libraries needed for running this code:
  *Adafruit BusIO
  *Adafruit Unified Sensor
  *DHT sensor library
  *RTClib
  *OneWire
  *Dallas Temperature
*/
#include <Wire.h>    // I2C interface
#include <RTClib.h>   // RTC library
#include <DHT.h>    // DHT Adafruit library
#include <DHT_U.h>   

# include <OneWire.h>   // libraries needed for water temperature
# include <DallasTemperature.h>  // libraries needed for water temperature

RTC_DS3231 rtc;     // create object RTC_DS3231 type

# define RELE 3     // digital pin 3 for relee
# define SENSOR 4   // digital pin 4 for DHT11
# define ButtonPin 5 // digital input pin for button in 5
int ambient_temperature;    // variable for ambient temperature sensor DHT11
int ambient_humidity;       // variable for ambient humidity sensor DHT11
DHT dht(SENSOR, DHT11);    

OneWire ourWire(2); // enable pin 2 como bus onewire
DallasTemperature sensors(&ourWire); //Se declara una variable u objeto para nuestro sensor

String c_year;
String c_day;
String c_month;
String c_hour;
String c_minute;
String c_second;
String Date;
String Hour;
String message;

bool event_start = true;  // variable de control para inicio de evento con valor true
bool event_end = true; 

int buttonState = 0;
int flag = 0;

void setup () {
 Serial.begin(9600);    // inicializa comunicacion serie a 9600 bps
 dht.begin();     // inicializacion de sensor
 pinMode(RELE, OUTPUT);   // pin 3 como salida
 pinMode(ButtonPin, INPUT_PULLUP);
 sensors.begin();

 if (! rtc.begin()) {       // si falla la inicializacion del modulo
 Serial.println("Modulo RTC no encontrado !");  // muestra mensaje de error
 while (1);         // bucle infinito que detiene ejecucion del programa
 }
 //rtc.adjust(DateTime(__DATE__, __TIME__));  //use only for setting hour
}

void loop () {
 DateTime date_stamp = rtc.now();
 c_year = date_stamp.year();
 c_month = date_stamp.month();  
 c_day = date_stamp.day();
 c_hour = date_stamp.hour();
 c_minute = date_stamp.minute();
 c_second = date_stamp.second();
 Date = c_day + "/" + c_month + "/" + c_year;
 Hour = c_hour + ":" + c_minute + ":" + c_second;
 ambient_temperature = dht.readTemperature(); // almacena en variable valor leido de temperatura
 ambient_humidity = dht.readHumidity();   // almacena en variable valor leido de humedad

 sensors.requestTemperatures(); //Se envía el comando para leer la temperatura
 float water_temperature = sensors.getTempCByIndex(0); //Se obtiene la temperatura en ºC
 
 message = Date + " " + Hour + "," + ambient_temperature + " C, " + ambient_humidity + " %, " + water_temperature;
 
 buttonState = digitalRead(ButtonPin);
 if (buttonState == LOW){
  if (flag == 0){
    digitalWrite(RELE, HIGH);
    delay(1000);
    flag=1;
  }
  else if (flag == 1){
    digitalWrite(RELE, LOW);
    delay(1000);
    flag=0;    
  }
 }
 if ((date_stamp.hour() <= 6) && (date_stamp.hour() >= 19) && (date_stamp.minute() == 00)){
    Serial.println(message);
    }    
 
 if ((date_stamp.hour() >= 7) && (date_stamp.hour() <= 18) && (date_stamp.minute() == 00) && (date_stamp.second() == 00)){
   if (event_start == true){     // si evento_inicio es verdadero
     digitalWrite(RELE, HIGH);       // activa modulo de rele con nivel alto
     Serial.println( "Rele encendido" );   // muestra texto en monitor serie
     event_start = false;
     event_end = true;
     Serial.println(message);
    }       
  }
  
 if ((date_stamp.hour() >= 7) && (date_stamp.hour() <= 19) && (date_stamp.minute() == 00) && (date_stamp.second() == 45)){
  if (event_end == true){        // si evento_fin es verdadero
    digitalWrite(RELE, LOW);        // desactiva modulo de rele con nivel bajo
    Serial.println( "Rele apagado" );     // muestra texto en monitor serie
    event_end = false;
    event_start = true; // carga valor falso en variable de control
    Serial.println(message);
    }        
  } 
}
