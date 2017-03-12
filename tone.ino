void toneFreizeichen() {
  sim800.println("AT+SIMTONE=1,425,25500,0,500000");
}

void toneRinging() {
  sim800.println("AT+STTONE=1,8,500000");
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


void toneError() {
    sim800.println("AT+SIMTONE=1,950,25500,0,330");
    delay(330);
    sim800.println("AT+SIMTONE=1,1400,25500,0,330");
    delay(330);
    sim800.println("AT+SIMTONE=1,1800,25500,0,330");
    delay(330);
    toneOff();
}



