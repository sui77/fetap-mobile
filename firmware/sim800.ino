#include <SoftwareSerial.h>

SoftwareSerial sim800(PIN_SIM800_TX, PIN_SIM800_RX);

void sim800_init() {
  sim800.begin(9600);
  sim800.println("AT");
  sim800_setRingerLevel(0);
  sim800_setMicrophoneGain(7);
}

/**
 * 0..15 => 0db..22.5db
 */
void sim800_setMicrophoneGain(int level) {
  sim800.print("AT+CMIC=0,");
  sim800.println(level);
}

/**
 * 0..100
 */
void sim800_setRingerLevel(int level) {
  sim800.print("AT+CRSL=");
  sim800.println(level);
}

void sim800_hangUp() {
  sim800.println("ATH");
}

void sim800_answerIncomingCall() {
  sim800.println("ATA");
}

void sim800_call(String s) {
  Serial.print("Calling "); Serial.println(s);
  sim800.print("ATD");
  sim800.print(s);
  sim800.println(";");
}

void queryCallReady() {
  sim800.println("AT+CCALR?");
}

void sim800_print(char b) {
  sim800.print(b);
}

bool sim800_available() {
   if (sim800.available()) { 
      readbuffer[readbufferP] = sim800.read();
      if (readbuffer[readbufferP] == 13 || readbuffer[readbufferP] == 10) {
        readbuffer[readbufferP] = 0;
        if (readbufferP != 0) {
          readbufferP = 0;
        
          return true;
        }
      } else if (readbuffer[readbufferP] != 0) {
        readbufferP++; 
      }
      if (readbufferP > 100) {readbufferP = 0; }
   }
   return false;
}
