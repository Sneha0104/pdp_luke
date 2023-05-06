// Run the receiver code before transmitter code since no acknowledgment

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//------------stepper--------------------//
// Define pin connections & motor's steps per revolution
const int dirPin = 1;    //(DIR+/YELLOW)
const int stepPin = 9;   //(PUL+/RED)
const int enablePin = 4; //(ENABLE+/GREEN)
const int stepsPerRevolution = 200;
const int Relay1 = 3;   // IN1 //BLUE //3
const int Relay2 = 2;   // IN2 // WHITE //2
const int revfast = 80; // revolutions
const int revslow = 30;
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
    radio.openWritingPipe(addresses[1]);    // 00002
    radio.openReadingPipe(1, addresses[0]); // 00001
    radio.setPALevel(RF24_PA_MIN);
    radio.setDataRate(RF24_250KBPS);
    radio.startListening();

//------------stepper--------------------//
    pinMode(stepPin, OUTPUT);
    pinMode(dirPin, OUTPUT);
    pinMode(enablePin, OUTPUT);
    pinMode(Relay1, OUTPUT);
    pinMode(Relay2, OUTPUT);
//------------stepper--------------------//

    char dataToSend[] = "Start";
    radio.stopListening();
    radio.write(&dataToSend, sizeof(dataToSend));
    radio.startListening();
    delay(5000);
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

        delay(5000);

//------------stepper--------------------//
        digitalWrite(Relay1, LOW); // LOW= CIRCUIT CLOSED // HIGH = CIRCUIT OPEN
        digitalWrite(Relay2, LOW);
        // Set motor direction counterclockwise ( HIGH = CLOCKWISE )
        digitalWrite(enablePin, LOW); // HIGH = MOTOR PHASES DISCONNECTED FROM THE DRIVER ELECTRONICS / LOW = MOTOR PHASES CONNECTED TO DRIVER ELECTRONICS(REQUIRED STATE FOR DRIVING THE MOTOR)
        digitalWrite(dirPin, HIGH);

        //  Spin motor fast
        for (int x = 0; x < revfast * stepsPerRevolution; x++) // revolutions to spin
        {
            digitalWrite(stepPin, HIGH); // Pulse frequency = Step frequency
            delayMicroseconds(2000);
            digitalWrite(stepPin, LOW);
            delayMicroseconds(2000);
        }
        delay(1000);                   // Wait 1 seconds
        digitalWrite(enablePin, HIGH); // Free motion
        digitalWrite(Relay1, HIGH);    // LOW= CIRCUIT CLOSED // HIGH = CIRCUIT OPEN
        digitalWrite(Relay2, HIGH);
//------------stepper--------------------//

        char dataToSend[] = "Motor 1";
        radio.stopListening();
        radio.write(&dataToSend, sizeof(dataToSend));
        radio.startListening();
        delay(5000);
    }
}
