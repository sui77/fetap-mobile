
 bool getSSerial() {
   if (portOne.available()) { 
      readbuffer[readbufferP] = portOne.read();
  
      if (readbuffer[readbufferP] == 13 || readbuffer[readbufferP] == 10) {
        readbuffer[readbufferP] = 0;
        if (readbufferP != 0) {
          readbufferP = 0;
          return true;
          if (strcmp(readbuffer, "OK")  == 0) {
            Serial.println("Okay");
          }
        }
      } else if (readbuffer[readbufferP] != 0) {
        readbufferP++; 
      }
      if (readbufferP > 100) {readbufferP = 0; }
   }
   return false;
}
