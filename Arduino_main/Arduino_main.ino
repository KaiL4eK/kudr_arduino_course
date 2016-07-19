#include <SoftwareSerial.h>
SoftwareSerial BT(10, 11); // RX, TX

const byte  interruptPin = 2,
            readIntPin   = 3,
            en           = 9,
            in_p         = 7,
            in_n         = 8;

volatile int turnage = 0;

void hall_sensor() {
  if (digitalRead(readIntPin) == HIGH) {
    turnage++;
  } else {
    turnage--;
  }
}

void setup()  
{
  Serial.begin(9600);
  BT.begin(9600);

  attachInterrupt(0, 
                  hall_sensor, 
                  RISING);

  pinMode(interruptPin, INPUT_PULLUP);
  pinMode(readIntPin, INPUT);
  
  pinMode(en, OUTPUT);
  pinMode(in_p, OUTPUT);
  pinMode(in_n, OUTPUT);

//  analogWrite(en, 45);
  digitalWrite(en, HIGH);
  
  Serial.println("Ok, start =)");
}

void loop() 
{
  int prev_turn = 0;
  char a; // stores incoming character from other device
  if (Serial.available())
    // if text arrived in from BT serial...
  {
    a=(Serial.read());
    if (a=='w') {
      turnage = 0;
      digitalWrite(in_p, HIGH);
      digitalWrite(in_n, LOW);
    }
    if (a=='s') {
      digitalWrite(in_p, LOW);
      digitalWrite(in_n, HIGH);
    }
    if (a==' ') {
      digitalWrite(in_p, LOW);
      digitalWrite(in_n, LOW);
    }
  }
  // Count encoder on motor: 360 degrees == 75 ticks on rise/fall
  // One rise or fall per rotation
/*  if (turnage >= 75) {
    digitalWrite(in_p, LOW);
    digitalWrite(in_n, LOW);
  }
*/  
  if ( prev_turn != turnage ) {
    Serial.println(turnage);
    prev_turn = turnage; 
  }
}

