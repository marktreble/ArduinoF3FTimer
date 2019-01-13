/*
 * Arduino F3F Timer
 * 
 * 5/1/2019 by Mark Treble
 * 
 */

#include <SoftwareSerial.h>

int PIN_RX = 0;
int PIN_TX = 1;
int PIN_BASE_A = 2;
int PIN_BASE_B = 3;
int PIN_LAUNCH = 4;
int PIN_BUZZER = 5;

int BAUD_RATE = 2400;

String CMD_START = "S";
String CMD_BASE = "P";
String CMD_LATE = "L";

unsigned long BUZZ_LENGTH = 200; // Length of time for buzzer to sound for

enum ApplicationState: int {
  STATE_IDLE,
  STATE_LAUNCHED,
  STATE_OFF_COURSE,
  STATE_ON_COURSE,
  STATE_BASE_A,
  STATE_BASE_B
};

class Button {
    private:
        bool _state;
        uint8_t _pin;
        // the following variables are unsigned longs because the time, measured in
        // milliseconds, will quickly become a bigger number than can be stored in an int.
        unsigned long _lastDebounceTime = 0;  // the last time the output pin was toggled
        unsigned long _debounceDelay = 300;    // the debounce time; increase if the output flickers

    public:
        Button(uint8_t pin) : _pin(pin) {}

        void begin() {
            pinMode(_pin, INPUT_PULLUP);
            _state = digitalRead(_pin);
            _lastDebounceTime = millis();
        }

        bool isPressed() {
          if ((millis() - _lastDebounceTime) < _debounceDelay) {     
            return false;
          }
          
          bool v = digitalRead(_pin);

          if (v != _state) {
              _state = v;
              if (!_state) {
                _lastDebounceTime = millis();
                return true;
              }
          }
          return false;
        }
};

int state = STATE_IDLE;
int numberOfTurns = 0;
unsigned long startTime = 0;
unsigned long buzzer = 0;
bool lateEntry = false;

Button baseA(PIN_BASE_A);
Button baseB(PIN_BASE_B);
Button launch(PIN_LAUNCH);

SoftwareSerial serial(PIN_RX, PIN_TX);

void setup() {
  // put your setup code here, to run once:
  serial.begin(BAUD_RATE);

  baseA.begin();
  baseB.begin();
  launch.begin();

  pinMode(PIN_BUZZER, OUTPUT);
  digitalWrite(PIN_BUZZER, LOW);

}

void loop() {
  // put your main code here, to run repeatedly:
  checkInputFromAndroid();
  checkInputFromLaunch();
  checkInputFromBases();
  checkForLateEntry();
  checkForBuzzerExpired();
}

void checkInputFromAndroid(){
  while (serial.available() > 0) {
      // read the incoming byte:
      int incomingByte = serial.read();
      
      if (incomingByte == 65) reset(); // ASCII A
      if (incomingByte == 83) start(); // ASCII S
  }  
}

void checkInputFromBases() {
   if (state == STATE_IDLE) return;
   
  if (baseA.isPressed()) {
    if (state == STATE_LAUNCHED) {
      state = STATE_OFF_COURSE;
      sendCommand(CMD_BASE);
      buzz();
      return;
    } 
    if (state == STATE_OFF_COURSE) {
      state = STATE_ON_COURSE;
      sendCommand(CMD_BASE);
      buzz();
      if (lateEntry == false) {
        startTimer();
      } else {
        sendCommand(CMD_LATE);      
      }
      return;
    } 
    if (state == STATE_BASE_A) {
      state = STATE_BASE_B;
      numberOfTurns++;
      sendCommand(CMD_BASE);
      buzz();
      if (numberOfTurns == 10) {
        finish();
      }
      return;
    }
  }

  if (baseB.isPressed()) {
    if (state == STATE_ON_COURSE) {
      state = STATE_BASE_A;
      numberOfTurns++;
      sendCommand(CMD_BASE);
      buzz();
      return;
    } 

    if (state == STATE_BASE_B) {
      state = STATE_BASE_A;
      numberOfTurns++;
      sendCommand(CMD_BASE);
      buzz();
      return;
    }
  }
}

void checkInputFromLaunch() {
  if (state != STATE_IDLE) return;
  
  if (launch.isPressed()) {
     sendCommand(CMD_START);
  }
}

void checkForLateEntry() {
  if (state == STATE_LAUNCHED 
    || state == STATE_OFF_COURSE) {
    float etime = getElapsedSeconds();
    if (etime >= 30) {
      // Late Entry
      startTimer();
      lateEntry = true;
    }
  }
}

void checkForBuzzerExpired() {
  if (buzzer == 0) return;
  if (getTime() - buzzer > BUZZ_LENGTH){
    digitalWrite(PIN_BUZZER, LOW);
    buzzer = 0;
  }
}

void reset(){
  state = STATE_IDLE;
}

void start() {
  state = STATE_LAUNCHED;
  numberOfTurns = 0;
  lateEntry = false;
}

void startTimer() {
  startTime = getTime();
}

void finish() {
  //TimeSpan ts = rtc.now() - started;
  //sendCommand(String(ts.totalseconds()));
  
  float etime = getElapsedSeconds();
  char buffer[9];
  
  dtostrf(etime,9,5,buffer);
  // Fill in leading zeros into buffer
  if (etime<100) buffer[0] = 48;
  if (etime<10) buffer[1] = 48;

  sendCommand("E");
  sendCommand(String(buffer));

  reset();
}

void sendCommand(String command) {
  serial.println(command);
}

void buzz() {
  digitalWrite(PIN_BUZZER, HIGH);
  buzzer = getTime();
}

float getElapsedSeconds() {
  unsigned long tme = getTime() - startTime;
  return (float)tme/1000;
}

unsigned long getTime() {
  // Needs converting to read from RTC
  return millis();
}


