// working of stepper motor with magnetic switch
const int dirPin = 2;  // stepper motor direction pin
const int stepPin = 3;  // stepper motor step pin
const int magnetPin = 4;  // magnetic switch pin
const int stepsPerRevolution = 200;  // number of steps per motor revolution
const int rev = 5;  // number of revolutions to spin

void setup() {
  pinMode(dirPin, OUTPUT);
  pinMode(stepPin, OUTPUT);
  pinMode(magnetPin, INPUT_PULLUP);  // internal pull-up resistor enabled
}

void loop() {
  digitalWrite(dirPin, HIGH);  // set motor direction clockwise
  int steps = 0;  // count the number of steps
  while (steps < rev * stepsPerRevolution) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(500);
    steps++;
    if (digitalRead(magnetPin) == LOW) {  // magnetic switch encountered magnet
      break;
    }
  }
  digitalWrite(stepPin, LOW);  // stop the motor
  digitalWrite(dirPin, LOW);
  delay(1000);  // let the motor come to a stop
}



