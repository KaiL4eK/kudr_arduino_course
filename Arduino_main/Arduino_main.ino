#include <SoftwareSerial.h>
SoftwareSerial BT(10, 11); // RX, TX

int en   = 9,
    in_p = 7,
    in_n = 8;

void setup()  
{
  Serial.begin(9600);
  BT.begin(9600);

  pinMode(en, OUTPUT);
  pinMode(in_p, OUTPUT);
  pinMode(in_n, OUTPUT);
  
  digitalWrite(en, HIGH);
  
  Serial.println("Ok, start =)");
}

void loop() 
{
  char a; // stores incoming character from other device
  if (BT.available())
  // if text arrived in from BT serial...
  {
    a=(BT.read());
    Serial.print(a);
    if (a=='w')
    {
      digitalWrite(in_p, HIGH);
      digitalWrite(in_n, LOW);
    }
    if (a=='s')
    {
      digitalWrite(in_p, LOW);
      digitalWrite(in_n, HIGH);
    }
    if (a==' ')
    {
      digitalWrite(in_p, LOW);
      digitalWrite(in_n, LOW);
    }
    Serial.println(" <<<");   
    // you can add more "if" statements with other characters to add more commands
  }
}
