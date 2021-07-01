#include <SoftwareSerial.h>

int sensorValue;

SoftwareSerial ESP8266 (rxPin, txPin);

void setup() { Serial.begin(9600);
    ESP8266.begin(9600); delay(2000);
    ESP8266.println("AT+RST\r\n"); //모듈 리셋
    delay(1000);
    ESP8266.println("AT+CWMODE=3\r\n"); //모드 3번 AP +Station Mode, 디바이이스 + AP 기능
    delay(1000);
    ESP8266.println("AT+CWJAP=\"Test\",\"test1234\"\"\r\n"); //공유기 접속
    delay(3000);
}

void printResponse() {
    while (ESP8266.available()) {
        Serial.println(ESP8266.readStringUntil('\n'));
    }
}

void loop() {
    sensorValue = 30;
    Serial.println(sensorValue); //소리 센서의 값을 시리얼 모니터에 출력
    ESP8266.println("AT+CIPMUX=1");
    //다중접속 설정을 1로한다.
    delay(1000);
    printResponse();
    ESP8266.println("AT+CIPSTART=4,\"TCP\",\"211.207.147.143\",80"); //포트연결
    delay(1000);
    printResponse();
    String name = "테스트";
    String email = "test@test.com";
    String cmd = "GET http://211.207.147.143/NFC/DatabaseUpload.php?name=" + name + "&email=" + email + " HTTP/1.0";

    //GET할 주소 형식은 HTTP/1.0
    ESP8266.println("AT+CIPSEND=4," + String(cmd.length() + 4)); //데이터 전송
    delay(1000);

    ESP8266.println(cmd);
    delay(200);
    ESP8266.println();
    delay(200);
    printResponse();
}
