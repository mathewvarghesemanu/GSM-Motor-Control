int fonafoundpin = 3;


#include "Adafruit_FONA.h"
#define FONA_RX 11
#define FONA_TX 10
#define FONA_RST 4
char replybuffer[255];
#include <SoftwareSerial.h>
SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);
SoftwareSerial *fonaSerial = &fonaSS;
Adafruit_FONA fona = Adafruit_FONA(FONA_RST);

uint8_t readline(char *buff, uint8_t maxbuff, uint16_t timeout) {
  uint16_t buffidx = 0;
  boolean timeoutvalid = true;
  if (timeout == 0) timeoutvalid = false;

  while (true) {
    if (buffidx > maxbuff) {
      //Serial.println(F("SPACE"));
      break;
    }

    while (Serial.available()) {
      char c =  Serial.read();

      //Serial.print(c, HEX); Serial.print("#"); Serial.println(c);

      if (c == '\r') continue;
      if (c == 0xA) {
        if (buffidx == 0)   // the first 0x0A is ignored
          continue;

        timeout = 0;         // the second 0x0A is the end of the line
        timeoutvalid = true;
        break;
      }
      buff[buffidx] = c;
      buffidx++;
    }

    if (timeoutvalid && timeout == 0) {
      //Serial.println(F("TIMEOUT"));
      break;
    }
    delay(1);
  }
  buff[buffidx] = 0;  // null term
  return buffidx;
}

uint8_t type;
void setup()
{
  pinMode(fonafoundpin, OUTPUT);
  digitalWrite(fonafoundpin, LOW);
  while (!Serial);

  Serial.begin(9600);
  fonaSerial->begin(9600);

  if (! fona.begin(*fonaSerial))
  {
    {
      while (1)
      {
        digitalWrite(fonafoundpin, HIGH);
        delay(1000);
        digitalWrite(fonafoundpin, LOW);
        delay(1000);
      }
    }
    type = fona.type();

  }
}
void loop()
{
  while (! Serial.available() )
  {
    if (fona.available())
    {
      Serial.write(fona.read());
    }
  }

}



