// fonction executée toutes les 1000 ms pour ne pas réduire les temps de cycle et permettre un affichage à 1 Hz
// principalement utilisée pour le calcul des messages
// ***************************************************
void Every1000ms ()
        { 
//          Serial.println("one second");
       if (MySerial.available() > 0) {  msg = 1;} else { msg = 2;} ;  // test si connection 
       compteur ++;
       if (compteur == 3600) {EEPROM.put(mem_legio,legio); ESP.restart(); } ; // redemarrage toutes les heures
       if (temperature < 45) {legio ++;};  
 //      EEPROM.put(mem_legio,legio);  //  sauvegarde legio en EPROM
       if (temperature > 60) {legio = 0;}  // reset legio si arrivé à 60 degres
          ;} 
