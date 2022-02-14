#include <EEPROM.h>

char number[15];

void setNumber(int n, char number[15]) {
  int addr = n * 15;
  for (int i = 0; i<15; i++) {
    EEPROM.write(addr+i, number[i]);
  }
}

char* getNumber(int n) {
  int addr = n * 15;
  for (int i = 0; i<15; i++) {
    char x = EEPROM.read(addr+i);
    if (x > 47 && x <59) {
       number[i] = x;
    } else {
      number[i] = 0;
    }
  } 
  return number; 
}
