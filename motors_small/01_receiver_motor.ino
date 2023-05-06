// Run the receiver code before transmitter code since no acknowledgment
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//------------stepper--------------------//
const int dirPin = 2;
const int stepPin = 3;
const int stepsPerRevolution = 200;
const int rev = 15; // revolutions
//------------stepper--------------------//

// Set up the radio
RF24 radio(7, 8); // CE, CSN
const byte addresses[][6] = {"00001", "00002"};

void setup()
{
  // Set up the serial communication
  Serial.begin(9600);

  // Set up the radio communication
  radio.begin();
  radio.openWritingPipe(addresses[0]);    // 00001
  radio.openReadingPipe(1, addresses[1]); // 00002
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
  radio.startListening();

  //------------stepper--------------------//
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  //------------stepper--------------------//
}
void loop()
{
  // Check if there is data available to be read
  if (radio.available())
  {
    // Read the data
    char data[10];
    radio.read(&data, sizeof(data));
    // Print the received data
    Serial.print("Received data: ");
    Serial.print(data);
    Serial.println();

    //------------stepper--------------------//
    digitalWrite(dirPin, HIGH);
    // Spin motor slowly
    for (int x = 0; x < rev * stepsPerRevolution; x++) // revolutions to spin
    {
      digitalWrite(stepPin, HIGH); // Pulse frequency / Step frequency
      delayMicroseconds(500);
      digitalWrite(stepPin, LOW);
      delayMicroseconds(500);
    }
    delay(3000); // Wait 3 seconds
                 //------------stepper--------------------//

    char dataToSend[] = "Motor 2";
    radio.stopListening();
    radio.write(&dataToSend, sizeof(dataToSend));
    radio.startListening();
    delay(5000);
  }
}
