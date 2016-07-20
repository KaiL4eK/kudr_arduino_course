#include <SoftwareSerial.h>
#include <LiquidCrystal.h>

const byte  interruptPin = 2,
            readIntPin   = A4,
            en           = 12,
            in_p         = A0,
            in_n         = A1,
            voltage_2    = A7,
            bt_uart_tx   = A2,
            bt_uart_rx   = A3,
            lcd_e        = 8,
            lcd_rs       = 9,
            lcd_d4       = 4,
            lcd_d5       = 5,
            lcd_d6       = 6,
            lcd_d7       = 7;

SoftwareSerial BT(bt_uart_rx, bt_uart_tx);
LiquidCrystal lcd(lcd_rs, lcd_e, lcd_d4, lcd_d5, lcd_d6, lcd_d7);

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
  lcd.begin(16, 2);
  lcd.print("Motor encoder:");

  attachInterrupt(0, hall_sensor, RISING);

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
  if (BT.available()) {
    a=(BT.read());
    if (a=='W') {
      //wturnage = 0;
      digitalWrite(in_p, HIGH);
      digitalWrite(in_n, LOW);
    }
    if (a=='S') {
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
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Motor encoder:");
    lcd.setCursor(0, 1);
    lcd.print(turnage);
    prev_turn = turnage; 
  }
/*  
  Serial.print("Voltage: ");
  //analog*2(Div)*5(V)/1023(ADC)
  float voltage = analogRead(voltage_2)*10.0/1023;
  delay(10);
  voltage = analogRead(voltage_2)*10.0/1023;
  Serial.println(voltage); 
*/
}

