#include <RotaryDialer.h>

// Rotary dial wheel
// at least in my FeTAP 611 yellow + brown are GND
#define PIN_ROTARY_READY	5   // white
#define PIN_ROTARY_PULSE	6   // green

// Hook - connect it to GND + this pin
#define PIN_HOOK A4

// H-Bridge input for ringer goes to
#define PIN_RINGER1 A6
#define PIN_RINGER2 A7

// RX + TX @sim800
#define PIN_SIM800_RX 8
#define PIN_SIM800_TX 9


#define STATE_OFF 1
#define STATE_READY
#define STATE_READY_TO_DIAL 2
#define STATE_RINGING 3
#define STATE_CALL 4

String numberMemory[10];

int currentState  = STATE_OFF;
int previousState = STATE_OFF;

char readbuffer[100];
int readbufferP = 0;

char phonenumber[20];
int phonenumberP = 0;
double lastNumberMillis = 0;
bool firstNumberDialed = false;

double lastRingingMillis = 0;

RotaryDialer dialer = RotaryDialer(PIN_ROTARY_READY, PIN_ROTARY_PULSE);

  
void setup() {
  numberMemory[1] = "+49173....";
  numberMemory[2] = "";
  numberMemory[3] = "";
  numberMemory[4] = "";
  numberMemory[5] = "";
  numberMemory[6] = "";
  numberMemory[7] = "";
  numberMemory[8] = "";
  numberMemory[9] = "";
  numberMemory[0] = "";

	pinMode(PIN_HOOK, INPUT_PULLUP);

  pinMode(PIN_RINGER1, OUTPUT);
  digitalWrite(PIN_RINGER1, LOW);
  pinMode(PIN_RINGER2, OUTPUT);
  digitalWrite(PIN_RINGER2, LOW);

  sim800_init();


  Serial.begin(9600);
  dialer.setup();

  Serial.println("Start");
//ring();

}

void loop() {

  if (previousState != currentState) {
    Serial.print("State was changed to ");   
    Serial.println(currentState);
  }
  previousState = currentState;
 
  switch (currentState) {
    
    case STATE_OFF:
      phonenumberP = 0;
      firstNumberDialed = 0;
      if (fetap_offhook()) {
        toneFreizeichen();
        currentState = STATE_READY_TO_DIAL;
      }
    break;

    case STATE_READY_TO_DIAL:

      if (!fetap_offhook()) {
        currentState = STATE_OFF;
        toneOff();
        sim800_hangUp();
        Serial.println("OFF");
      }

      // turn off dial tone if rotary wheel was moved
      if (!firstNumberDialed && digitalRead( PIN_ROTARY_READY ) == 0) {
        firstNumberDialed = true;
        toneOff();
      }
  
      if (phonenumberP > 0 && millis() > lastNumberMillis + 4000) {
        
        Serial.print("Call now: ");
        Serial.println( phonenumber);
        if (phonenumberP == 1) {
          int number = phonenumber[0]-48;
          if (numberMemory[number].equals("")) {
            toneError();
          } else {
            toneInfo(number);
            delay(1000*number+100);
            sim800_call(numberMemory[number]);
          }
        } else {
          sim800_call(phonenumber);
        }
        phonenumberP = 0;
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
      
      if (fetap_offhook()) {
        sim800_answerIncomingCall();
        currentState = STATE_CALL;
      } else if (millis() > lastRingingMillis + 5000) {
        currentState = STATE_OFF;
      }
      
    break;

    case STATE_CALL:
      if (!fetap_offhook()) {
        sim800_hangUp();
        currentState = STATE_OFF;
      }    
    break;
  }
 
  
  if (sim800_available()) { 
    Serial.print("From sim800: ");
    Serial.println(readbuffer);
    if (strcmp(readbuffer, "RING")  == 0 && (currentState == STATE_OFF || currentState == STATE_RINGING)) {
          ring();
          currentState = STATE_RINGING;
          lastRingingMillis = millis();
    } else if (strcmp(readbuffer, "NO DIALTONE") == 0 && currentState != STATE_OFF) {
          toneError();
    }
  }

  if (Serial.available()) {
    char b = Serial.read();
    if (b!=0) {
      sim800_print(b);
    }
  }

  

 
}
