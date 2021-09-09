#include <Wire.h>                   // I2C 통신에 반드시 필요한 라이브러리
#include <SPI.h>
#include <Adafruit_MLX90614.h>      // 적외선 센서 라이브러리
#include <Adafruit_PN532.h>         // NFC 라이브러리

#define PN532_SCK  (2)
#define PN532_MOSI (3)
#define PN532_SS   (4)
#define PN532_MISO (5)

const int trigPin = 22;    //Trig 핀 할당
const int echoPin = 24;    //Echo 핀 할당
const int piezoPin = 26;  //Piezo 핀 할당

int count = 0;
long duration, distance;

Adafruit_MLX90614 mlx = Adafruit_MLX90614();
Adafruit_PN532 nfc(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS);
 
void setup()
{
    Serial.begin(9600);
    Serial.println("출입시스템");

    nfc.begin();
    
    uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }
  // Got ok data, print it out!
  Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX); 
  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC); 
  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);
  
  // configure board to read RFID tags
  nfc.SAMConfig();
  
  Serial.println("NFC Reader ready.");
  
    mlx.begin();

    Serial.println("TempSensor prepared.");
     
    pinMode(trigPin, OUTPUT);    //Trig 핀 output으로 세팅
    pinMode(echoPin, INPUT);     //Echo 핀 input으로 세팅
    pinMode(piezoPin, OUTPUT);   //Piezo 핀 output으로 세팅

    Serial.println("HyperSensor OK.");  
}
 
void loop()
{
  //HyperSensor();
    
  switch (count) {
  case 0:
    NFCRead();
    delay(2000);    //온도센서 준비시간
    count = 1;
    break;
  case 1:
    TempSeneor();
    count = 2;
    break;
  case 2:
    if(distance < 60){
      delay(5000);
      count = 0;
    }
    break;
  default:
    Serial.println("ERROR");
    break;
  }
}

void NFCRead()
{
  uint8_t success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // 유저ID
  uint8_t uidLength;                        // 4or7바이트 ID확인
  uint8_t currentblock;                     // Counter to keep track of which block we're on
  bool authenticated = false;               // Flag to indicate if the sector is authenticated
  uint8_t data[16];                         // Array to store block data during reads

  
  // Keyb on NDEF and Mifare Classic should be the same
  uint8_t keyuniversal[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

  // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
  // 'uid' will be populated with the UID, and uidLength will indicate
  // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

  if (success) {
    // Display some basic information about the card
    Serial.println("Found an ISO14443A card");
    Serial.print("  UID Length: ");Serial.print(uidLength, DEC);Serial.println(" bytes");
    Serial.print("  UID Value: ");
    nfc.PrintHex(uid, uidLength);
    Serial.println("");

    if (uidLength == 4)
    {
      // We probably have a Mifare Classic card ...
      Serial.println("Seems to be a Mifare Classic card (4 byte UID)");

      // Now we try to go through all 16 sectors (each having 4 blocks)
      // authenticating each sector, and then dumping the blocks
      for (currentblock = 0; currentblock < 64; currentblock++)
      {
        // Check if this is a new block so that we can reauthenticate
        if (nfc.mifareclassic_IsFirstBlock(currentblock)) authenticated = false;

        // If the sector hasn't been authenticated, do so first
        if (!authenticated)
        {
          // Starting of a new sector ... try to to authenticate
          Serial.print("------------------------Sector ");Serial.print(currentblock/4, DEC);Serial.println("-------------------------");
          if (currentblock == 0)
          {
              // This will be 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF for Mifare Classic (non-NDEF!)
              // or 0xA0 0xA1 0xA2 0xA3 0xA4 0xA5 for NDEF formatted cards using key a,
              // but keyb should be the same for both (0xFF 0xFF 0xFF 0xFF 0xFF 0xFF)
              success = nfc.mifareclassic_AuthenticateBlock (uid, uidLength, currentblock, 1, keyuniversal);
          }
          else
          {
              // This will be 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF for Mifare Classic (non-NDEF!)
              // or 0xD3 0xF7 0xD3 0xF7 0xD3 0xF7 for NDEF formatted cards using key a,
              // but keyb should be the same for both (0xFF 0xFF 0xFF 0xFF 0xFF 0xFF)
              success = nfc.mifareclassic_AuthenticateBlock (uid, uidLength, currentblock, 1, keyuniversal);
          }
          if (success)
          {
            authenticated = true;
          }
          else
          {
            Serial.println("Authentication error");
          }
        }
        // If we're still not authenticated just skip the block
        if (!authenticated)
        {
          Serial.print("Block ");Serial.print(currentblock, DEC);Serial.println(" unable to authenticate");
        }
        else
        {
          // Authenticated ... we should be able to read the block now
          // Dump the data into the 'data' array
          success = nfc.mifareclassic_ReadDataBlock(currentblock, data);
          if (success)
          {
            // Read successful
            Serial.print("Block ");Serial.print(currentblock, DEC);
            if (currentblock < 10)
            {
              Serial.print("  ");
            }
            else
            {
              Serial.print(" ");
            }
            // Dump the raw data
            nfc.PrintHexChar(data, 16);
          }
          else
          {
            // Oops ... something happened
            Serial.print("Block ");Serial.print(currentblock, DEC);
            Serial.println(" unable to read this block");
          }
        }
      }
    }
    else
    {
      Serial.println("Ooops ... this doesn't seem to be a Mifare Classic card!");
    }
  }
}

void TempSeneor()
{
  int temp_count = 1;
  float temp[5];
  float sum_temp = 0;
  float avr_temp;

    for(int i = 0; i < 5; i++)           //5회측정
    {
      Serial.print("Ambient = "); 
      Serial.print(mlx.readAmbientTempC()); // 주변온도를 읽습니다.
      Serial.print("*C\tObject = "); 
      temp[i] = mlx.readObjectTempC();
      sum_temp += temp[i];
      Serial.print(temp[i]); Serial.println("*C"); //객체 온도를 읽습니다.
      Serial.println();
      delay(500);
    }
    avr_temp = sum_temp/5;                        //평균값
    Serial.print(avr_temp); Serial.println("*C");
}

void HyperSensor()
{
  while(count < 2)
  {
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
  }
}
