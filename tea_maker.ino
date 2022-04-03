
const int PIN_LED = 7;
const int PIN_SERVO = 9;
const int PIN_BUTTON_1 = 10;
const int PIN_BUTTON_2 = 13;
const int PIN_BUTTON_3 = 12;
const int PIN_BUTTON_STOP = 11;

#include <Servo.h>

Servo servo;  // create servo object to control a servo


const int STATE_WAIT = 0;
const int STATE_LOWERING = 1; // to 90
const int STATE_STEEPING = 2; // to 30
const int STATE_STOPPING = 3; // to 12

void setup() {
 //start serial connection

  Serial.begin(9600);

  //configure pin 2 as an input and enable the internal pull-up resistor

  pinMode(PIN_BUTTON_1, INPUT_PULLUP);
  pinMode(PIN_BUTTON_2, INPUT_PULLUP);
  pinMode(PIN_BUTTON_3, INPUT_PULLUP);
  pinMode(PIN_BUTTON_STOP, INPUT_PULLUP);

  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, HIGH);

  servo.attach(PIN_SERVO);  // attaches the servo on pin 9 to the servo object

 }

int state = 0;
float pos = 12;
long steeping_ms = 0;
long time_start_ms = 0;

const float STEP = 2;
const float PERIOD = 5000;

void loop() {

  bool b1 = (digitalRead(PIN_BUTTON_1) == LOW);
  bool b2 = (digitalRead(PIN_BUTTON_2) == LOW);
  bool b3 = (digitalRead(PIN_BUTTON_3) == LOW);
  bool bstop = (digitalRead(PIN_BUTTON_STOP) == LOW);

  Serial.print("b1=");
  Serial.print(b1);
  Serial.print(" b2=");
  Serial.print(b2);
  Serial.print(" b3=");
  Serial.print(b3);
  Serial.print(" bstop=");
  Serial.print(bstop);
  Serial.print(" pos=");
  Serial.print(pos);
  Serial.print(" state=");
  Serial.print(state);
  Serial.print(" time_left_ms=");
  Serial.print(time_start_ms + steeping_ms - millis());
  Serial.println();

  switch(state) {
    case STATE_WAIT: 
      if (b1) {
           steeping_ms = 2*60*1000L;
           state = STATE_LOWERING;
        }
      if (b2) {
           steeping_ms = 3*60*1000L;
           state = STATE_LOWERING;
        }
      if (b3) {
           steeping_ms = 5*60*1000L;
           state = STATE_LOWERING;
        }        
      break;
    case STATE_LOWERING: // to 65 - middle of cycle
      pos += STEP;
      if (pos >= 65) {
        state = STATE_STEEPING;
        time_start_ms = millis();
      }
      break;
    case STATE_STEEPING:// to 40
      pos -= STEP;
      pos = 65 + 25*sin(3.14*2*(millis() - time_start_ms)/PERIOD);

      if (millis() >= time_start_ms + steeping_ms || bstop) state = STATE_STOPPING;

      break;
    case STATE_STOPPING: // to 12
      pos -= STEP;
      if (pos <= 12) state = STATE_WAIT;
      break;
  }
  
  servo.write(pos);
}
