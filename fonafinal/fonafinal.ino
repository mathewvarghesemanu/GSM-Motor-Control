int fonafoundpin = 3;
int motorstatus = 0;
int flag = 0;
const int MotorPin = 5;
char sendto[] = "9446050001";
String sms;


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
  pinMode(MotorPin, OUTPUT);
  digitalWrite(MotorPin, LOW);

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

    deletesms();
}


void flushSerial() {
  while (Serial.available())
    Serial.read();
}


void deletesms()
{
  flushSerial();
  Serial.print(F("Delete #"));
  uint8_t smsn;
  for (smsn = 1; smsn < 20; smsn++)
    if (fona.deleteSMS(smsn)) {
      Serial.println(F("OK! deleted"));
    } else {
      Serial.println(F("Couldn't delete"));
    }
}

void sendsms(char message[])
{
  flushSerial();
  if (!fona.sendSMS(sendto, message))
  {
    Serial.println(F("Failed"));
  } else
  {
    Serial.println(F("Sent sms!"));
    Serial.println(message);
  }
}

void readsms()
{
  flushSerial();
  Serial.print(F("Read #"));
  uint8_t smsn = 1;
  uint16_t smslen;



  if (! fona.readSMS(smsn, replybuffer, 250, &smslen))
  {
    Serial.println("Failed!");
  }
  sms=sms+replybuffer;
  Serial.println(replybuffer);
  //sms.toCharArray(char_array, str_len);
//  sms=replybuffer;
  deletesms();
}


void loop()
{
  Serial.print("started");
  sms = "";
  replybuffer[0]='\0';
  delay(2000);
  readsms();
  delay(2000);
  

  if (sms == "Alert on")
    {
      flag = 1;
      sendsms("Alert on");
    }
    
  else if (sms == "Alert off")
    flag = 0;
  else if (sms == "Motor on")
  {
    digitalWrite(MotorPin, HIGH);
    motorstatus = 1;
    if (flag == 1)
      sendsms("Motor on");
  }
  else if (sms == "Motor off")
  {
    digitalWrite(MotorPin, LOW);
    motorstatus = 0;
    if (flag == 1)
      sendsms("Motor off");
  }
  else if (sms == "is ok")
    sendsms("I'm fine");
  else if (sms == "Motor status")
  {
    Serial.print("worked");
    if (motorstatus == 1)
      sendsms("Motor on");
    else if (motorstatus == 0)
      sendsms("Motor off");
  }
  

}



