#include <RotaryDialer.h>
#include <SoftwareSerial.h>

#define PIN_READY	8   // weiß  (rot)
#define PIN_PULSE	9   // grün   (schwarz)
// GND gelb und braun

#define PIN_SPEAKER 5 // grün/gelb
#define PIN_HOOK 12

#define PIN_RING1 2
#define PIN_RING2 3

#define STATE_OFF 1
#define STATE_READY
#define STATE_READY_TO_DIAL 2
#define STATE_RINGING 3
#define STATE_CALL 4



int currentState = STATE_OFF;

char readbuffer[100];
int readbufferP = 0;

char phonenumber[20];
int phonenumberP = 0;

double lastNumberMillis = 0;

RotaryDialer dialer = RotaryDialer(PIN_READY, PIN_PULSE);
SoftwareSerial portOne(10, 11);
  
void setup() {
	pinMode(PIN_HOOK, INPUT);
  digitalWrite(PIN_HOOK, HIGH);

  pinMode(PIN_RING1, OUTPUT);
  digitalWrite(PIN_RING1, LOW);
  pinMode(PIN_RING2, OUTPUT);
  digitalWrite(PIN_RING2, LOW);

  portOne.begin(57600);
  portOne.println("AT");
  Serial.begin(9600);
  dialer.setup();

}

void loop() {
  switch (currentState) {
    
    case STATE_OFF:
      phonenumberP = 0;
      if (isHookOn()) {
        toneFreizeichen();
        currentState = STATE_READY_TO_DIAL;
      }
    break;

    case STATE_READY_TO_DIAL:
      // aufgelegt
      if (!isHookOn()) {
        currentState = STATE_OFF;
      }

      // erste nummer bereits gewählt
      if (phonenumberP != 0) {
         toneOff();
      }
  
      if (phonenumberP > 2 && millis() > lastNumberMillis + 5000) {
        phonenumberP = 0;
        Serial.print("Call now: ");
        Serial.println( phonenumber); 
      }
  
      if (dialer.update()) {
        Serial.println(1);
        phonenumber[phonenumberP] = 48 + dialer.getNextNumber();
        phonenumber[phonenumberP];
        phonenumber[phonenumberP+1] = 0;
        lastNumberMillis = millis();
        
        phonenumberP++;
        if (phonenumberP > 18) {
          phonenumberP = 0;
          toneError();
        }
      }
    break;

    case STATE_RINGING:
      if (isHookOn()) {
        Serial.println('answer call');
        currentState = STATE_CALL;
      }
    break;

    case STATE_CALL:
      if (!isHookOn()) {
        Serial.println('end call');
        currentState = STATE_OFF;
      }    
    break;
  }
 
  
  if (getSSerial()) { 
    Serial.print("From sim800: ");
    Serial.println(readbuffer);
    if (strcmp(readbuffer, "RING")  == 0) {
          ring();
    }
  }

  if (Serial.available()) {
    char b = Serial.read();
    if (b!=0) {
      portOne.print(b);
    }
  }

  

 
}
