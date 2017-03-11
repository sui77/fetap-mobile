void toneFreizeichen() {
  tone(PIN_SPEAKER, 425);
}

void toneOff() {
  noTone(PIN_SPEAKER);
}

void toneError() {
  tone(PIN_SPEAKER, 950);
  delay(330);
  tone(PIN_SPEAKER, 1400);
  delay(330);
  tone(PIN_SPEAKER, 1800);
  delay(330);
  toneOff();
  delay(1000);
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

