// Test with 03 receiver/transmitter motor first before running the code with transmission/reception acknowledgement

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//------------stepper--------------------//
// Define pin connections & motor's steps per revolution
const int dirPin = 1;    //(DIR+/YELLOW)
const int stepPin = 9;   //(PUL+/RED)
const int enablePin = 4; //(ENABLE+/GREEN)
const int stepsPerRevolution = 200;
const int Relay1 = 3;  // IN1 //BLUE
const int Relay2 = 2;  // IN2 // WHITE
const int revfast = 3; // revolutions
// const int revslow = 30;
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
    // Declare pins as Outputs
    pinMode(stepPin, OUTPUT);
    pinMode(dirPin, OUTPUT);
    pinMode(enablePin, OUTPUT);
    pinMode(Relay1, OUTPUT);
    pinMode(Relay2, OUTPUT);
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

        unsigned long startTime = millis();
        while (millis() - startTime < 100)
        {
            // Do nothing and wait
        }

        //------------stepper--------------------//

        digitalWrite(enablePin, LOW); // HIGH = MOTOR PHASES DISCONNECTED FROM THE DRIVER ELECTRONICS / LOW = MOTOR PHASES CONNECTED TO DRIVER ELECTRONICS(REQUIRED STATE FOR DRIVING THE MOTOR)
        digitalWrite(Relay1, HIGH);   // LOW= Normally closed ports-> OPEN (LED ON) //HIGH =Normally closed ports->CLOSED (LED OFF)
        digitalWrite(Relay2, HIGH);
        delay(1000);                                           // "ENA must be ahead of DIR by at least 200ms"- from Driver datasheet
        digitalWrite(dirPin, HIGH);                            // LOW = COUNTERCLOCKWISE / HIGH = CLOCKWISE
        delay(1000);                                           //"DIR must be ahead of PUL effective edge by 2us to ensure correct direction" -from Driver datasheet
                                                               //  Spin motor fast
        for (int x = 0; x < revfast * stepsPerRevolution; x++) // revolutions to spin
        {
            digitalWrite(stepPin, HIGH); // Pulse frequency = Step frequency
            delayMicroseconds(2000);
            digitalWrite(stepPin, LOW);
            delayMicroseconds(2000);
        }
        unsigned long startTime = millis();
        while (millis() - startTime < 100)
        {
            // Do nothing and wait
        }
        digitalWrite(enablePin, LOW); // Free motion // HIGH = MOTOR PHASES DISCONNECTED FROM THE DRIVER ELECTRONICS / LOW = MOTOR PHASES CONNECTED TO DRIVER ELECTRONICS(REQUIRED STATE FOR DRIVING THE MOTOR)
        digitalWrite(Relay1, LOW);    // LOW= Normally closed ports-> OPEN (LED ON) //HIGH =Normally closed ports->CLOSED (LED OFF)
        digitalWrite(Relay2, LOW);
        // delay(20000);

        //------------stepper--------------------//

        // delay(5000);

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

            unsigned long startTime = millis();
            while (millis() - startTime < 100)
            {
                // Do nothing and wait
            }
        }
        // ==============write=================== //
    }
}
