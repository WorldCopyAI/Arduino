#include <Wire.h>                   // I2C 통신에 반드시 필요한 라이브러리
#include <Adafruit_MiniMLX90614.h>  // 적외선 센서 라이브러리

Adafruit_MLX90614 mix = Adafruit_MLX90614();

void setup() {
  Serial.begin(9600);
  Serial.println("온도센서 측정 테스트");
  mix.begin();

}

void loop() {
  Serial.print("대상 온도 = ");
  Serial.println(mix.readObjectTempC());
  delay(2000);
}
