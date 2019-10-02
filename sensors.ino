#include <dht.h>

#define GREEN_PIN 2
#define BLUE_PIN 3
#define FAN_PIN 4
#define DHT_PIN 5

const int WATER_PIN = 0;
const int PHOTO_PIN = 1;
const byte numChars = 32;
char receivedChars[numChars]; // an array to store the received data
dht DHT;

boolean newData = false;

void setup() {

  // Initialise serial communication
  Serial.begin(9600);

  // Initialise the output pins
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);

  // Initialise the input pin
  pinMode(DHT_PIN, INPUT);
}

void loop() {

  recvWithEndMarker();
  handleDevice();
}

void handleDevice() {

  if (newData == true) {

    int command = receivedChars[0];

    if (command == 104){

      Serial.print("ARDUINO");

    } else if (command == 108 || command == 102){

      String stringDevice = (String) receivedChars[1];
      int device = stringDevice.toInt();

      bool state = false;
      if (receivedChars[2] == '1') {

        state = true;
      }

      switch (device) {

        case 2:
          digitalWrite(2, state);
          break;
        case 3:
          digitalWrite(3, state);
          break;
        case 4:
          digitalWrite(4, state);
          break;
        default:
          digitalWrite(2, state);
          digitalWrite(3, state);
          digitalWrite(4, state);
      }

      Serial.print("OK");

    } else if (command == 116){

      int tries = 5;
      int chk = DHT.read11(DHT_PIN);
      while (chk != 0 && tries > 0){
        DHT.read(DHT_PIN);
        tries--;
      }

      Serial.print((int) DHT.temperature);

    } else if (command == 112){

      int photoLevel = analogRead(PHOTO_PIN);
      Serial.print(photoLevel);

    } else if (command == 119){

      int waterLevel = analogRead(WATER_PIN);
      Serial.print(waterLevel);
    }

    newData = false;
  }
}

void recvWithEndMarker() {

  static byte ndx = 0;
  char endMarker = '\n';
  char rc;

  while (Serial.available() > 0 && newData == false) {

    rc = Serial.read();

    if (rc != endMarker) {

      receivedChars[ndx] = rc;
      ndx++;

      if (ndx >= numChars) {
        ndx = numChars - 1;
      }

    } else {

      receivedChars[ndx] = '\0'; // terminate the string
      ndx = 0;
      newData = true;
    }
  }
}
