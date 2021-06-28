#include <SoftwareSerial.h>
SoftwareSerial s(5,6);
void setup() {

  s.begin(9600);

  Serial.begin(9600);

  pinMode(7, OUTPUT);

}

 

void loop() {

 

  if(s.available()>0) {

    int result = 0;

    result = s.read();

    Serial.println(result);

    if(result == 1) { 

      digitalWrite(7, HIGH);

    } else if(result == 2) { 

      digitalWrite(7, LOW);

    }

  }

  delay(100);

}
