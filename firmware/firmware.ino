#include <RotaryDialer.h>

// Rotary dial wheel
// at least in my FeTAP 611 yellow + brown are GND
#define PIN_ROTARY_READY	2   // white
#define PIN_ROTARY_PULSE	3   // green

// Hook - connect it to GND + this pin
#define PIN_HOOK A1

// H-Bridge input for ringer goes to
#define PIN_RINGER1 A2
#define PIN_RINGER2 A3

// RX + TX @sim800
#define PIN_SIM800_RX 8
#define PIN_SIM800_TX 9


#define STATE_OFF 1
#define STATE_READY_TO_DIAL 2
#define STATE_RINGING 3
#define STATE_CALL 4
#define STATE_INPUT_SPEED_DIAL_SLOT 5
#define STATE_INPUT_SPEED_DIAL_NUMBER 6


int currentState  = STATE_OFF;
int previousState = STATE_OFF;

char readbuffer[100];
int readbufferP = 0;

char phonenumber[20];
int phonenumberP = 0;
double lastNumberMillis = 0;
bool firstNumberDialed = false;
int speedDial = 0;

double lastRingingMillis = 0;

RotaryDialer dialer = RotaryDialer(PIN_ROTARY_READY, PIN_ROTARY_PULSE);

  
void setup() {
  Serial.begin(9600);
  fetap611_init();
  sim800_init();
  dialer.setup();
  Serial.println("Ready");

  for (int i = 1; i<10; i++) {
    Serial.print("Speed Dial #"); Serial.print(i); Serial.print(": "); Serial.println(getNumber(i));
  }

}


bool numberInput(int inputTimeMs) {

    if (!fetap_offhook()) {
        currentState = STATE_OFF;
        toneOff();
        sim800_hangUp();
        Serial.println("OFF");
    }
  
    if (dialer.update()) {
        int dialedNumber = dialer.getNextNumber();
        Serial.println(dialedNumber);
        phonenumber[phonenumberP] = 48 + dialedNumber;
        phonenumber[phonenumberP];
        phonenumber[phonenumberP+1] = 0;
        lastNumberMillis = millis();
        
        phonenumberP++;
        if (phonenumberP > 18) {
          phonenumberP = 0;
          toneError();
        }
    }

    if (phonenumberP > 0 && millis() > lastNumberMillis + inputTimeMs) {
        Serial.print("Input: ");
        Serial.print(currentState);
        Serial.println( phonenumber);
        return true;
    } 
    
    return false;
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

      // turn off dial tone if rotary wheel was moved
      if (!firstNumberDialed && digitalRead( PIN_ROTARY_READY ) == 0) {
        firstNumberDialed = true;
        toneOff();
      }

      if (numberInput(4000)) {
        if (phonenumber[1] == 0) {
          if (phonenumber[0] == '0') {
            phonenumberP = 0;
            firstNumberDialed = 0;
            playSound("ansage1");
            currentState = STATE_INPUT_SPEED_DIAL_SLOT;
          } else {
            // speed dial
            currentState = STATE_CALL;
            String sNumber = getNumber(phonenumber[0]-48);
            Serial.print("SpeedDial: ");
            Serial.println(sNumber);
            sim800_call(sNumber);
          }
        } else {
            currentState = STATE_CALL;
            sim800_call(phonenumber);
        }
      }
      
  
    break;

    case STATE_INPUT_SPEED_DIAL_SLOT:
      
      if (numberInput(0)) {
          speedDial = phonenumber[0]-48;
          phonenumberP = 0;
          firstNumberDialed = 0;
          currentState = STATE_INPUT_SPEED_DIAL_NUMBER;
          playSound("ansage3");  
      }
    break;

    case STATE_INPUT_SPEED_DIAL_NUMBER:
      
      if (numberInput(4000)) {
            setNumber(speedDial, phonenumber);
        
            playSound("ansage7"); delay(2000);
            for (int i = 0; i<phonenumberP; i++) {
              playSound(String(phonenumber[i])); delay(1000);
            }
            playSound("ansage6"); delay(2000);
            playSound(String(speedDial)); delay(1000);         
            playSound("ansage5"); delay(2000);
            
            phonenumberP = 0;
            firstNumberDialed = 0;
            currentState = STATE_OFF;
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
    if (strcmp(readbuffer, "BUSY")  == 0 && currentState == STATE_CALL) {
          noLine();
    } else if (strcmp(readbuffer, "RING")  == 0 && (currentState == STATE_OFF || currentState == STATE_RINGING)) {
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
      Serial.print(b);
    }
  }

 
}
