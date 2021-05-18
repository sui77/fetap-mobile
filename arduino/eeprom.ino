#include <EEPROM.h>

char number[15];

void setNumber(int n, char number[15]) {
  Serial.println("yo");
  Serial.println(sizeof(number));
  int addr = n * 15;
  for (int i = 0; i<15; i++) {
    Serial.print(addr+i); Serial.print(" "); Serial.println(number[i]);
    EEPROM.write(addr+i, number[i]);
  }
}

char* getNumber(int n) {
  int addr = n * 15;
  for (int i = 0; i<15; i++) {
    number[i] = EEPROM.read(addr+i);
  } 
  return number; 
}
