// Test with 01 receiver/transmitter motor first before running the code with transmission/reception acknowledgement

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//------------stepper--------------------//
// Define pin connections & motor's steps per revolution
const int dirPin = 4;
const int stepPin = 3;
const int stepsPerRevolution = 200;
const int rev = 15; // revolutions
//------------stepper--------------------//

//------------radio------------------//
RF24 radio(7, 8); // CE, CSN
const byte addresses[][6] = {"00001", "00002"};
//------------radio------------------//

void setup()
{
  // Set up the serial communication
  Serial.begin(9600);

  //------------radio------------------//
  radio.begin();
  radio.openWritingPipe(addresses[0]);    // 00001
  radio.openReadingPipe(1, addresses[1]); // 00002
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
  radio.startListening();
  //------------radio------------------//

  //------------stepper--------------------//
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  //------------stepper--------------------//
}

void loop()
{
  // Serial.print("R: ");
  //  Check if there is data available to be read
  if (radio.available())
  {
    // ==============read=================== //

    // Read the data
    char data[10] = "";
    radio.read(&data, sizeof(data));

    // Print the received data
    Serial.print("Received data: ");
    Serial.print(data);
    Serial.println();

    // Send Acknowledgment
    radio.stopListening();
    char sendData[] = "Ack";
    radio.write(&sendData, sizeof(sendData));
    radio.startListening();

    // ==============read=================== //

    // delay(5000);
    unsigned long startTime = millis();
    while (millis() - startTime < 100)
    {
      // Do nothing and wait 
    }

    //------------stepper--------------------//

    // Set motor direction clockwise
    digitalWrite(dirPin, HIGH);
    for (int x = 0; x < rev * stepsPerRevolution; x++) // revolutions to spin
    {
      digitalWrite(stepPin, HIGH); // Pulse frequency / Step frequency
      delayMicroseconds(500);
      digitalWrite(stepPin, LOW);
      delayMicroseconds(500);
    }
    // delay(3000);

    // Stop the motor
    digitalWrite(stepPin, LOW);
    digitalWrite(dirPin, LOW);
    // delay(1000);  // let the motor come to a stop

    //------------stepper--------------------//

    // ==============write=================== //

    char dataToSend[] = "Motor2";
    bool success = false;
    char dataReceived[] = "NoAck";
    while (!success && strcmp(dataReceived, "Ack") != 0)
    {
      Serial.println("Sending message...");
      radio.stopListening();
      success = radio.write(&dataToSend, sizeof(dataToSend));
      radio.startListening(); // delay(2000);
      // Serial.println(success);
      if (success)
      {
        // Read the data
        char data[10] = "";
        radio.read(&data, sizeof(data));
        Serial.print(data);
        Serial.println();
        if (strcmp(data, "Ack") == 0)
        {
          Serial.println("ACK received!");
          strcpy(dataReceived, "Ack");
        }
      }
      else
      {
        Serial.println("Failed to send message");
      }

      // delay(2000);  // wait 2 seconds before sending again
      unsigned long startTime = millis();
      while (millis() - startTime < 100)
      {
        // Do nothing and wait 
      }
    }
    // ==============write=================== //
  }
}
