// ********************************
// recepteur V5
// ********************************
// Le but de ce software est de realiser un recepteur de commandes pour piloter un chauffe eau electrique ECS
// pour moduler la puissance consommée 
// cette version fonctionne sur la carte ESP32 LOLIN + OLED 
// La resistance electrique est pilotée par un module à Triac placé près de celle ci
// la temperature de l'eau est mesurée par un ds18b20
// les echanges sont fait par port serie 
// - reception d'un signal de pilotage de 0 à 100 %
// - transmission d'une temperature à l'emetteur sur le bus Myserial
// un afficheur indique la temperature de l'eau et la puissance de chauffe %
// en cas de perte de signal serie de pilotage, le chauffe eau utilise une puissance fonction de la temperature: chauffe rapidement puis se stabilise entre 50 et 60 degres
// l'eau est maintenue à une temperature minimum
// une fonction de securité active la chauffe si une periode trop longue a basse temperature a eu lieu (anti legionellose)
// elle s'active si plus de 3 jours cumulés sont passes a moins de 45 degres et se resette en passant au dessus de 60 degres
// cette valeur de temps est stockée en EEPROM en cas de coupure.
// mettre le reglage mecanique ECS au maxi
// free software by Fsa 
// *********************************
// Documentation
// *************
// Librairies
// *************
#include <RBDdimmer.h>//  pour le dimmer robotdyn
#include <HardwareSerial.h> // car le port hardware serial1 de l'esp32 est crée ainsi
HardwareSerial MySerial(1);  // bus serial1 particulier au ESP32
#include <OneWire.h>            // pour lire la temperature
#include <DallasTemperature.h>  // pour la temperature
#include <EEPROM.h>             // stockage EEPROM
// *********************************

#define outputPin  25 // pour pilotage triac
#define zerocross  26 // for Lolin board
#define RX1 16  // pour Serial1
#define TX1 17  // pour Serial1

dimmerLamp dimmer(outputPin, zerocross); //initialase port for dimmer for ESP8266, ESP32, Arduino due boards

// *********************************
// capteur DS1820 ou DS18b20 connecté sur pin 52. resistance de tirage au +5 V de 2,7 Kohm
// *********************************
// Data wire is plugged into port 52 on the Arduino
#define ONE_WIRE_BUS 21
// Setup a oneWire instance to communicate with any OneWire devices 
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
// *********************************
// OLED display library - version U8G2
// *********************************
// For a connection via I2C using Wire include
#include <U8g2lib.h>
// Initialize the OLED display using Wire library
// SSD1306Wire display(0x3c, 5, 4);   // ADDRESS, SDA, SCL  -
// constructor for Oled display
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 4, /* data=*/ 5, /* reset=*/ U8X8_PIN_NONE);   // ESP32 Thing, pure SW emulated I2C
// R0,R1,R2,R3 : rotations 0,90,180,270
int outVal = 10;  // pour le debut du fonctionnement , à 10% par defaut
// uint8_t inByte = 10 ;
// int inByte = 10 ;

float celsius; //  temperature mesuree en deg Celsius, est un float a l'origine
long tempC=15; // temperature mesurée par le capteur
int temperature = 34 ;
int puissance = 50 ;
int curseur = 50 ; // pour l affichage de la barre de puissance de 0 a 100
int input ; // pour stocker inByte au format int
int msg = 0; // choix du message a afficher: 0 = demarrage, 1 AUTO, 2 deconnecte, 3 sensor fault
int temps1,temps2 ; // pour compter les temps millis
int compteur = 0; // compteur de secondes pour le reset
long legio ; // compteur pour test anti-legionellose, stocké en memoire. compte les secondes en dessous de 45 degres. Max 3 jours = 3*24*60*60 = 259200 secondes
long legiomax = 259200 ;  // 3 jours en dessous de 45 degres, sans repasser au dessus de 60 degres
int mem_legio = 0 ; // place memoire ou est stocke la variable legio

void setup() {
 // Serial.begin(115200);  // console pour tests
//  Serial.println("Demarrage");
    MySerial.begin(9600, SERIAL_8N1, RX1, TX1);  // connection a l'autre carte de pilotage (intermediaire) par bus serie
    // MySerial.begin(9600, SERIAL_8N1, IN,OUT) 
  dimmer.begin(NORMAL_MODE, ON); //dimmer initialisation: name.begin(MODE, STATE)
  dimmer.setPower(puissance); // name.setPower(0%-100%) 
   u8g2.begin();
   u8g2.setFont( u8g2_font_10x20_tf);
   u8g2.setDrawColor(1);
   u8g2.setFontPosTop();
   u8g2.setFontDirection(0);
   u8g2.drawStr(0,46,"++DEMARRAGE++");
   u8g2.sendBuffer();          // transfer internal memory to the display
   EEPROM.get(mem_legio,legio);  //  charge legio depuis l'EPROM
   delay(300);
}

void loop() 
{
   // determination du cylce de 1 seconde 
 temps1 = millis() ; // valeur actuelle du temps 
  if ((temps1 - temps2) > 1000) {temps2 = temps1 ; Every1000ms ();} // effectuer cette action toutes les secondes  
//      Serial.print("MySerial = "); Serial.println(MySerial.available());
 if (msg == 1) connecte ();
 if (msg == 2) deconnecte () ;
 if (msg == 3) deconnecte () ;  // cas temp inop
 if (msg == 4) deconnecte () ;  // cas reset
}
