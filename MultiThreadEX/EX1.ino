#define LEDon(n) digitalWrite(LED[n], HIGH);
#define LEDoff(n) digitalWrite(LED[n], LOW);

int LED[] = { 7, 6, 5, 4, 3, 2 }; //7-6-5 초록, 4-3-2 빨강
unsigned long past = 0; //이전시간
unsigned long now = 0; //현재시간
int a = 0;
int i = 0;

void setup() {
    for (int i = 0; i < 6; i++ ){
        pinMode(LED[i], OUTPUT);
    }
    Serial.begin(9600);
}

void loop() {
    int jodo1 = analogRead(A0);
    int jodo2 = analogRead(A1);
    now = millis();

    if (((a == 0))) {
        past = millis();
        LEDon(i) LEDon(i+3)
        a++;}
    if ((now-past) > 500){
        LEDoff(i) LEDoff(i+3)
        past = millis();
        a = 0;
        i++;}
    //////////////////////////////////////////////////////// LED

    if (jodo1 < 100) {
        LEDoff(0)LEDoff(1)LEDoff(2)}

    if (jodo2 < 100) {
        LEDoff(3)LEDoff(4)LEDoff(5)}
    //////////////////////////////////////////////////////// 조도센서

    if (i >= 3 ){  // 루프를 위한 i 초기화
        i = 0;}

    Serial.print(jodo1);
    Serial.print(" ");
    Serial.println(jodo2);
}
//[출처] 연구실 생존기 6. 아두이노 멀티쓰레드 입문 (3주차)|작성자 황혼