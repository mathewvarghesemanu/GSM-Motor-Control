/*
  The circuit:
  RX is digital pin 10 (connect to TX of GSM Modem)
  TX is digital pin 11 (connect to RX of GSM Modem)
*/
#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX

void setup()
{
  Serial.begin(9600);
  Serial.println("Calling through GSM Modem");
  mySerial.begin(9600);
  delay(2000);
  mySerial.println("ATD81290255XX;"); // ATD81290255XX; -- watch out here for semicolon at the end!!
  Serial.println("Called ATD81290255XX");
}

void loop()
{
  if (mySerial.available())
    Serial.write(mySerial.read());
}
