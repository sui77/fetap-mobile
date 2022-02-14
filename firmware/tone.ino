void toneFreizeichen() {
  sim800.println("AT+SIMTONE=1,425,25500,0,500000");
}

void toneRinging() {
  sim800.println("AT+SIMTONE=1,425,1000,3500,500000");
}

void noLine() {
  sim800.println("AT+SIMTONE=1,425,240,240,500000");
}

void keinAnschluss() {
  toneError();
  delay(1000);
  sim800.println("AT+CREC=4,C:\\keinanschluss.amr,0,1");
}

void inputTone() {
  sim800.println("AT+SIMTONEX=1,50000,425,400,25500,10,0");
}

void toneOff() {
  sim800.println("AT+SIMTONE=0");
  Serial.println("tone off");
}

void toneInfo(int n) {
  sim800.print("AT+SIMTONE=1,200,200,100,");
  sim800.print( n*300 );
  sim800.println();
}

void playSound(String file) {
  Serial.println("playing");
  sim800.print("AT+CREC=4,C:\\User\\");
  sim800.print(file);
  sim800.println(".amr,0,1");
}


void toneError() {
    sim800.println("AT+SIMTONE=1,950,25500,0,330");
    delay(330);
    sim800.println("AT+SIMTONE=1,1400,25500,0,330");
    delay(330);
    sim800.println("AT+SIMTONE=1,1800,25500,0,330");
    delay(330);
    toneOff();
}
