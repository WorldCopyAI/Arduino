const int trigPin = 22;    //Trig 핀 할당
const int echoPin = 24;    //Echo 핀 할당
const int piezoPin = 26;  //Piezo 핀 할당
 
void setup()
{
    Serial.begin(115200);
    Serial.println("초음파센서 거리측정및 경보");
 
    pinMode(trigPin, OUTPUT);    //Trig 핀 output으로 세팅
    pinMode(echoPin, INPUT);     //Echo 핀 input으로 세팅
    pinMode(piezoPin, OUTPUT);   //Piezo 핀 output으로 세팅
}
 
void loop()
{
    long duration, distance;
 
    //Trig 핀으로 10us의 신호 발생
    digitalWrite(trigPin, LOW);       //Trig 핀 Low
    delayMicroseconds(2);             //2us 유지
    digitalWrite(trigPin, HIGH);      //Trig 핀 High
    delayMicroseconds(10);            //10us 유지
    digitalWrite(trigPin, LOW);       //Trig 핀 Low
 
    //Echo 핀으로 들어오는 펄스의 시간 측정
    duration = pulseIn(echoPin, HIGH);   //pulseIn함수가 호출되고 펄스가 입력될 때까지의 시간.
    distance = duration /29 / 2;        //센치미터로 환산

    Serial.print(distance);
    Serial.println();
    delay(100);
    
    if(distance < 60){
      for(int i=0;i<3;i++)
      {
        tone(piezoPin,1024,250);      //1초간 piezo울림(523은 주파수)
        delay(250);                   //0.3초 대기
        noTone(piezoPin);             //piezo출력 끔
        tone(piezoPin,512,250);      //1초간 piezo울림(523은 주파수)
        delay(250);                   //0.3초 대기
        noTone(piezoPin);             //piezo출력 끔
      }
    }
}
