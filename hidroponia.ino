
#include <Wire.h>		// incluye libreria para interfaz I2C
#include <RTClib.h>		// incluye libreria para el manejo del modulo RTC
#include <SPI.h>		// incluye libreria interfaz SPI
#include <SD.h>			// incluye libreria para tarjetas SD
#include <DHT.h>		// incluye libreria DHT de Adafruit
#include <DHT_U.h>	

RTC_DS3231 rtc;			// crea objeto del tipo RTC_DS3231
# define RELE 3
# define SENSOR 4		// constante SENSOR en pin digital 4 (senal de DHT11)
int TEMPERATURA;		// variable para almacenar valor de temperatura
int HUMEDAD;	

String ano;
String dia;
String mes;
String horas;
String minutos;
String segundos;
String Date;
String Hour;
String Nombrearchivo;
String Imprimir;

#define SSpin 53 //en arduino uno era 10
DHT dht(SENSOR, DHT11);		


bool evento_inicio = true;	// variable de control para inicio de evento con valor true
bool evento_fin = true;	

void setup () {
 Serial.begin(9600);		// inicializa comunicacion serie a 9600 bps
 dht.begin();			// inicializacion de sensor
 pinMode(RELE, OUTPUT);		// pin 3 como salida

 if (! rtc.begin()) {				// si falla la inicializacion del modulo
 Serial.println("Modulo RTC no encontrado !");	// muestra mensaje de error
 while (1);					// bucle infinito que detiene ejecucion del programa
 }
 Serial.println("Inicializando tarjeta ...");	// texto en ventana de monitor
 if (!SD.begin(SSpin)) {			// inicializacion de tarjeta SD
  Serial.println("fallo en inicializacion !");// si falla se muestra texto correspondiente y
  return;					// se sale del setup() para finalizar el programa
 }
 //rtc.adjust(DateTime(__DATE__, __TIME__));	 
}

void loop () {
 DateTime fecha = rtc.now();
 ano = fecha.year();
 mes = fecha.month();
 dia = fecha.day();
 horas = fecha.hour();
 minutos = fecha.minute();
 segundos = fecha.second();
 Date = ano + "/" + mes + "/" + dia;
 Hour = horas + ":" + minutos + ":" + segundos;
 TEMPERATURA = dht.readTemperature();	// almacena en variable valor leido de temperatura
 HUMEDAD = dht.readHumidity();		// almacena en variable valor leido de humedad
 Nombrearchivo = ano + mes + dia + ".txt";
 Imprimir = Date + " " + Hour + "," + TEMPERATURA + "," + HUMEDAD;
 /*
 if (((fecha.hour() <= 6) || (fecha.hour() >= 19)) && (fecha.minute() == 00)){
  File dataFile = SD.open(Nombrearchivo, FILE_WRITE);
  if (dataFile){
    dataFile.println(Imprimir);
    Serial.println(Imprimir);
    dataFile.close();
    }    
 }
 */
 if (fecha.hour() == 20 && fecha.minute() == 00){
    evento_fin = false;
    evento_inicio = true;	   
 }  
 
 if (((fecha.hour() >= 7) || (fecha.hour() <= 10)) && (fecha.minute() == 00)){
    if ( evento_inicio == true ){			// si evento_inicio es verdadero
      digitalWrite(RELE, HIGH);				// activa modulo de rele con nivel alto
      Serial.println( "Rele encendido" );		// muestra texto en monitor serie
      evento_inicio = false;
      evento_fin = true;	
      File dataFile = SD.open(Nombrearchivo, FILE_WRITE);
      if (dataFile){
        dataFile.println(Imprimir);
        Serial.println(Imprimir);
        dataFile.close(); 
      }
    }       
  }
  
 if (((fecha.hour() >= 7) || (fecha.hour() <= 10)) && (fecha.minute() == 1)){
  if ( evento_fin == true ){				// si evento_fin es verdadero
    digitalWrite(RELE, LOW);				// desactiva modulo de rele con nivel bajo
    Serial.println( "Rele apagado" );			// muestra texto en monitor serie
    evento_fin = false;
    evento_inicio = true;				// carga valor falso en variable de control
    }		     
  }  

 if (((fecha.hour() >= 11) || (fecha.hour() <= 13)) && (fecha.minute() == 00)){
    if ( evento_inicio == true ){			// si evento_inicio es verdadero
      digitalWrite(RELE, HIGH);				// activa modulo de rele con nivel alto
      Serial.println( "Rele encendido" );		// muestra texto en monitor serie
      evento_inicio = false;	
      evento_fin = true;
      File dataFile = SD.open(Nombrearchivo, FILE_WRITE);
      if (dataFile){
        dataFile.println(Imprimir);
        Serial.println(Imprimir);
        dataFile.close();
      }
    } 
  }
 if (((fecha.hour() >= 11) || (fecha.hour() <= 13)) && (fecha.minute() == 1)){
  if ( evento_fin == true ){				// si evento_fin es verdadero
    digitalWrite(RELE, LOW);				// desactiva modulo de rele con nivel bajo
    Serial.println( "Rele apagado" );			// muestra texto en monitor serie
    evento_fin = false;
    evento_inicio = true;				// carga valor falso en variable de control
    }		     
  } 
   
 if (((fecha.hour() >= 14) || (fecha.hour() <= 18)) && (fecha.minute() == 0)){
    if ( evento_inicio == true ){			// si evento_inicio es verdadero
      digitalWrite(RELE, HIGH);				// activa modulo de rele con nivel alto
      Serial.println( "Rele encendido" );		// muestra texto en monitor serie
      evento_inicio = false;	
      evento_fin = true;
      File dataFile = SD.open(Nombrearchivo, FILE_WRITE);
      if (dataFile){
      dataFile.println(Imprimir);
      Serial.println(Imprimir);
      dataFile.close();
      }         
    }
  
  }
 if (((fecha.hour() >= 14) || (fecha.hour() <= 18)) && (fecha.minute() == 1)){
  if ( evento_fin == true ){				// si evento_fin es verdadero
    digitalWrite(RELE, LOW);				// desactiva modulo de rele con nivel bajo
    Serial.println( "Rele apagado" );			// muestra texto en monitor serie
    evento_fin = false;
    evento_inicio = true;				// carga valor falso en variable de control
    }		     
  } 
}