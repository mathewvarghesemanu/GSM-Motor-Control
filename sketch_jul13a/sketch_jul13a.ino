#include<SoftwareSerial.h>

char phone_no[]="9446050001";
SoftwareSerial myserial(11,10);

void setup()
{
  Serial.begin(9600);
  delay(200);
  myserial.begin(9600);
  delay(200);
  myserial.println("AT+CMGF=1");
  delay(200);
}

String sendsms(String sms)
{
  delay(1000);  
  myserial.print("AT+CMGS=\"");
  myserial.print(phone_no);
  myserial.println("\"");

  delay(1000);

  myserial.print(sms);
  myserial.print(0x1A);
  delay(1000);
}

int main(void)
{
  String a;
  a=sendsms("HI");
  Serial.print(a);
  while(1);
  return(0);
}

