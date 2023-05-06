
// Define pin connections & motor's steps per revolution
const int dirPin = 1; //(DIR+/YELLOW)
const int stepPin = 9; //(PUL+/RED)
const int enablePin = 4;  //(ENABLE+/GREEN)
const int stepsPerRevolution = 200;
const int Relay1 = 3; //IN1 //BLUE //3
const int Relay2 = 2; // IN2 // WHITE //2
const int revfast = 80; // revolutions
const int revslow = 30;

void setup()
{
  // Declare pins as Outputs
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(enablePin, OUTPUT);
  pinMode(Relay1,OUTPUT);
  pinMode(Relay2,OUTPUT);
   
}
void loop()
{
  digitalWrite(Relay1, LOW); // LOW= CIRCUIT CLOSED // HIGH = CIRCUIT OPEN
  digitalWrite(Relay2, LOW);
  // Set motor direction counterclockwise ( HIGH = CLOCKWISE ) 
  digitalWrite(enablePin, LOW); // HIGH = MOTOR PHASES DISCONNECTED FROM THE DRIVER ELECTRONICS / LOW = MOTOR PHASES CONNECTED TO DRIVER ELECTRONICS(REQUIRED STATE FOR DRIVING THE MOTOR)  
  digitalWrite(dirPin, HIGH);

//  Spin motor fast
  for(int x = 0; x < revfast*stepsPerRevolution; x++) //revolutions to spin 
  {
    digitalWrite(stepPin, HIGH); // Pulse frequency = Step frequency
    delayMicroseconds(2000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(2000);
  }
  delay(1000); // Wait 1 seconds
  digitalWrite(enablePin,HIGH); // Free motion
  digitalWrite(Relay1, HIGH); // LOW= CIRCUIT CLOSED // HIGH = CIRCUIT OPEN
  digitalWrite(Relay2, HIGH);

}
