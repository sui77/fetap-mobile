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
