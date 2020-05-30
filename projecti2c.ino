//This will include the library wire so that we can do an I2C connection
#include <Wire.h>
// defining the address of the Argon device on the bus so that it is detectable and we can send information
#define address 0x10

//define our pins connections you can modify this if yuo want
int Red = A2;
int Green  = A5;
int PirPin = D6;
int buzzer = D3;
//we define our variables here for the sensors and conditions
int val = 0;
int bad = 0;
int good = 0;

//This will setup the pins and serial connection to the RPI
void setup() 
{   
    //this just sets up the connections
    //selecting output will tell the device we need to put voltage out
    pinMode(Red, OUTPUT);
    pinMode(Green, OUTPUT);
    pinMode(PirPin, INPUT);
    pinMode(buzzer, OUTPUT);
    // begin the serial with 9600 baud to write data
    Serial.begin(9600);
    //begins connection to the raspberry pi and vice versa
    Wire.begin(address);
    //when a signal is sent we trigger this to enact a command
    Wire.onReceive(reader);
    
    //when the RPi requests information we send these to transmit data
    Wire.onRequest(sendData);
    Wire.onRequest(sendgood);
    Wire.onRequest(sendbad);
}
//the loop which is running 
void loop() 
{   
  delay(100);
}
//function to send data using write
void sendData()
{   
    Wire.write(val);
}
void sendbad()
{
    Wire.write(bad);
}
void sendgood()
{
    Wire.write(good);
}
//buzzer function uses the tone procedure
void buzz()
{   
    //this will use the buzzer with the variables in respective order being 
    //the frequency of the sound and the duration
    tone(buzzer, 3000, 1000);
}
//detects person and changes LED
void person_detected()
{
    digitalWrite(Green, HIGH);
    digitalWrite(Red, LOW);
}
//detects person when they are unkown and activates LEDS and buzzer
void unknown_detected()
{
    digitalWrite(Red, HIGH);
    buzz();
    digitalWrite(Green, LOW);
}
//Main function when rpi sends a signal this will trigger
void reader(int byteCount) 
{   
    //while wire available creates a continuous communication
    Serial.print("Initialisation received");
    while (Wire.available())
    {   
        //PIR sensor sends 0 if false
        int num = Wire.read();
        val = digitalRead(PirPin);
        if (val == 0) 
        {   
            digitalWrite(Red, HIGH);
            digitalWrite(Green, LOW);
            Wire.write(val);
            Wire.onRequest(sendData);
            
        }
        //PIR sensor sends 1 if true
        if (val == 1)
        {
            digitalWrite(Green, HIGH);
            digitalWrite(Red, LOW);
            Wire.write(val);
            Wire.onRequest(sendData);
        }
        //reads signal from pi then sends acknowledgement signal if the person detected 
        if (num == 2)
        {   
            person_detected();
            good = 2;
            sendgood();
            Wire.onRequest(sendgood);
        }
        //reads signal from pi then sends acknowledgement signal if the person detected 
        if(num == 3)
        {   
            unknown_detected();
            bad = 3;
            Wire.write(bad);
            sendbad();
            Wire.onRequest(sendbad);
        }
    }
}

    


