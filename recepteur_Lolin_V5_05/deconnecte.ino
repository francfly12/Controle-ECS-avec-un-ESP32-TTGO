void deconnecte ()
{ 
    if (legio > legiomax) {puissance = 90 ;} ; // pour le cas anti-legionellose
    dimmer.setPower(puissance); // name.setPower(0%-100%) 
    sensors.requestTemperatures(); // Send the command to get temperatures 
    tempC = sensors.getTempCByIndex(0); 
//    Serial.print("tempC = "); Serial.println(tempC); 
    temperature= int(tempC);
//    MySerial.write(temperature); 
//+++++++++++++++++++++++++++++++ u8g2
    u8g2.clearBuffer();
    u8g2.setCursor(28,0);
  u8g2.setFont(u8g2_font_fub35_tf);
  u8g2.print(temperature);
  u8g2.print(char(176));          // degres
  curseur = map (puissance,0,100,0,128) ; // calcul du curseur
//  Serial.print("puissance = "); Serial.println(puissance);
//  Serial.print("curseur = "); Serial.println(curseur);
  u8g2.drawRFrame(0,39,127,8,2);  // boite arrondie vide
  u8g2.drawRBox  (2,41,curseur,4,2); // partie pleine du curseur
  u8g2.setFont( u8g2_font_10x20_tf);
//  u8g2.drawStr(0,24,"0         100"); 
// ligne message
//  case 1:    u8g2.drawStr(0,46,"connecte "); u8g2.setCursor(95,47) ;u8g2.print(inByte);u8g2.print(char(37));  break;
    u8g2.drawStr(0,46,"deconnect "); u8g2.setCursor(94,47) ;u8g2.print(puissance);u8g2.print(char(37));
      if (legio > legiomax) { u8g2.drawStr(0,46,"anti legio.  90%"); };    
if (msg == 3) {  u8g2.drawStr(0,46,"CAPTEUR TEMP INOP"); };
  u8g2.sendBuffer();
//+++++++++++++++++++++++++++++++ test des temperatures
if (temperature < 30) { puissance = 95;} ;
if (temperature > 25) { puissance = 90;} ;
if (temperature > 40) { puissance = 80;} ;
if (temperature > 50) { puissance = 20;} ;
if (temperature > 60) { puissance = 10;} ;
//    Serial.println();
//  temperature=temperature+1;
//  if (temperature==99) {temperature=1;};
//  puissance=puissance+1;
//  if (puissance==101) {puissance=0;};
 ; 
}
