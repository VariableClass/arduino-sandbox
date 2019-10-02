#include <SR04.h>

#define MIN_DISTANCE 5
#define MAX_DISTANCE 50

#define BUZZER_PIN 10
#define ECHO_PIN 11
#define TRIG_PIN 12

SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);
long distanceInCm;

void setup() {
  
  // Provide an output channel for the serial data
  Serial.begin(9600);

  // Initialise the buzzer pin as an output
  pinMode(BUZZER_PIN,OUTPUT);
  
  // Wait 1 second
  delay(1000);
}

void loop() {
  
  unsigned char i;

  // Record the measured distance
  distanceInCm=sr04.Distance();

  // Output the distance
  Serial.println(distanceInCm);
    
  // If an object is less than a metre and a half away
  if((distanceInCm<=MAX_DISTANCE)&&(distanceInCm>MIN_DISTANCE)){
    
    // Output a frequency
    for(i=0;i<(distanceInCm);i++)
    {
      digitalWrite(BUZZER_PIN,HIGH);
      delay(2);
      digitalWrite(BUZZER_PIN,LOW);
      delay(2);
    }
  } else if (distanceInCm<=MIN_DISTANCE){

    while(sr04.Distance()<=MIN_DISTANCE){
      // Beep solidly
      digitalWrite(BUZZER_PIN,HIGH);
      delay(2);
      digitalWrite(BUZZER_PIN,LOW);
      delay(2);
    }
  }
}
