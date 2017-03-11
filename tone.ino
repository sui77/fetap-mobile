void toneFreizeichen() {
  portOne.println("AT+SIMTONE=1,425,25500,0,500000");
}

void toneOff() {
  portOne.println("AT+SIMTONE=0");
  noTone(PIN_SPEAKER);
}

void toneError() {
    portOne.println("AT+SIMTONE=1,950,25500,0,330");
    delay(330);
    portOne.println("AT+SIMTONE=1,1400,25500,0,330");
    delay(330);
    portOne.println("AT+SIMTONE=1,1800,25500,0,330");
    delay(330);
    toneOff();
}

void ring() {
  for (int i=0; i <= 30; i++) {
      digitalWrite(PIN_RING1, HIGH);
      digitalWrite(PIN_RING2, LOW);
      delay(12);
      digitalWrite(PIN_RING1, LOW);
      digitalWrite(PIN_RING2, HIGH);
      delay(12);
  }
}

