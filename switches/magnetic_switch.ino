const int reedPin = 2; // the digital input pin connected to the reed switch
int reedState = 0;     // variable to store the state of the reed switch

void setup() {
 pinMode(reedPin, INPUT);
 Serial.begin(9600);
}

void loop() {
 reedState = digitalRead(reedPin);
 if (reedState == HIGH) {
   Serial.println("Magnetic field detected!");
 }
 delay(100);
}

