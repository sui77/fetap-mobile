void fetap611_init() {
  pinMode(PIN_HOOK, INPUT_PULLUP);
  pinMode(PIN_RINGER1, OUTPUT);
  digitalWrite(PIN_RINGER1, LOW);
  pinMode(PIN_RINGER2, OUTPUT);
  digitalWrite(PIN_RINGER2, LOW);
}

bool fetap_offhook() {
  return !digitalRead(PIN_HOOK);
}

void ring() {
  Serial.println("RINGRING");
  
  for (int i=0; i <= 30; i++) {
      digitalWrite(PIN_RINGER1, HIGH);
      digitalWrite(PIN_RINGER2, LOW);
      delay(12);
      digitalWrite(PIN_RINGER1, LOW);
      digitalWrite(PIN_RINGER2, HIGH);
      delay(12);
  }
  
  digitalWrite(PIN_RINGER1, LOW);
  digitalWrite(PIN_RINGER2, LOW);
}
