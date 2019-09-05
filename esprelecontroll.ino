#include <ESP8266WiFi.h>//teste
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager
#include <FS.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ArduinoJson.h>
#include <EEPROM.h>
//#include <EEPROMAnything.h>
#include <Wire.h>
#include <OneWire.h>
#include <TickerScheduler.h>
#include <DallasTemperature.h>
#include <stdio.h>

// Variaveis globais
 String ssid, password; 
 String ssid2, password2;

#define ONE_WIRE_BUS 0  
#define ATO_SWITCH_PIN 1 //D2
#define TEMP_HEAT_PIN 1  //D5
#define TEMP_CHILL_PIN 1 //D6
#define ATO_PUMP_PIN 1   //D7
#define output4 1        //D8
#define RELE_1 4 //D2
#define RELE_2 14  //D5
#define RELE_3 12 //D6
#define RELE_4 13 //D6


ESP8266WebServer server(80);
ESP8266HTTPUpdateServer httpUpdater;

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
// arrays to hold device address
DeviceAddress insideThermometer;


int wifi_error = 0;


struct ConfigWIFIStruct{
  int config_inicial;
  char ssid_ap[30];
  char pwd_ap[30];
  char ssid_wifi[30];
  char pwd_wifi[30];
}wifiConfig;

int sizeofvar = sizeof(int);



#define WIFI_ERROR 0 
#define WIFI_CONFIG sizeofvar


boolean Rele_1_flag = false;            //1 if cooling on
boolean Rele_2_flag = false;            //1 if cooling on
boolean Rele_3_flag = false;            //1 if cooling on
boolean Rele_4_flag = false;
boolean Rele_5_flag = false;
boolean Rele_6_flag = false;
boolean Rele_7_flag = false;
boolean Rele_8_flag = false;
boolean wifi_connected = false;
char connected_ip[14];

TickerScheduler ts(1); // 3 = quantos schedules serao utilizados

String padleft(byte valor)
{
  if (valor < 10) 
    return "0" + String(valor, DEC);
  else
    return String(valor, DEC);
}
