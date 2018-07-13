#include<SoftwareSerial.h>
char c[30];
int flag = 0, i;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  SoftwareSerial myserial(11, 10);
  myserial.begin(9600);

}

void loop() {
  while (Serial.available())
  { i = 0;
  //  while (c[i] != '\0')
    {
      c[i] = Serial.read();
      i++;
      if (c[i] == '\n')
        {
          c[i] == '\0';
        }
     
      delay(200);
    }
     Serial.print(c);
  }
  if (strcmp(c, "man") == 0)
  {
    Serial.print("u");
  }
}
