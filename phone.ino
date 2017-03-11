void hangUp() {
  portOne.println("ATH");
}

void answerIncomingCall() {
  portOne.println("ATA");
}

void call(String s) {
  
}

void queryCallReady() {
  portOne.println("AT+CCALR?");
}

