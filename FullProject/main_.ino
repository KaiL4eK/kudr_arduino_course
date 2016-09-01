// #define ENABLE_LCD
// #define SERIAL_ECHO
// #define VOLTAGE_CONTROL
#define MOTOR_CONTROL
#define ENABLE_MOTORS
// #define SERVO_TEST
// #define PWM_TEST
// #define ENABLE_ENCODERS
#define IR_RECEIVER
// #define ENABLE_BT_CONTROL
// #define ENABLE_SHARP_IR
// #define ENABLE_POTENTIOMETER
#define ENABLE_BUZZER

#ifdef ENABLE_LCD
#include <LiquidCrystal.h>

int rs_pin = 22;
int e_pin  = 23;
int d4_pin = 24;
int d5_pin = 25;
int d6_pin = 26;
int d7_pin = 27;

LiquidCrystal lcd(rs_pin, e_pin, d4_pin, d5_pin, d6_pin, d7_pin);
#endif

#ifdef MOTOR_CONTROL
int motorR_pos = 5;  
int motorR_neg = 6; 
int motorL_pos = 4;  
int motorL_neg = 7;

#define MOTOR_RIGHT (1 << 0)
#define MOTOR_LEFT  (1 << 1)

void motor_stop ( int side )  
{   
    if ( side & MOTOR_RIGHT ) {
        digitalWrite( motorR_pos, LOW );    
        digitalWrite( motorR_neg, LOW );
    }

    if ( side & MOTOR_LEFT ) {
        digitalWrite( motorL_pos, LOW );    
        digitalWrite( motorL_neg, LOW );
    }
}  

void motor_start ( int dutyCycle, int side )  
{    
    if ( dutyCycle > 0 ) {
        if ( side & MOTOR_RIGHT ) { 
            digitalWrite( motorR_neg, LOW );      
            analogWrite( motorR_pos, dutyCycle );
        }

        if ( side & MOTOR_LEFT ) {     
            digitalWrite( motorL_neg, LOW );      
            analogWrite( motorL_pos, dutyCycle );    
        }
    } else if ( dutyCycle < 0 ) {      
        if ( side & MOTOR_RIGHT ) { 
            digitalWrite( motorR_pos, LOW );     
            analogWrite( motorR_neg, -dutyCycle );
        }

        if ( side & MOTOR_LEFT ) { 
            digitalWrite( motorL_pos, LOW );      
            analogWrite( motorL_neg, -dutyCycle ); 
        }   
    } else {
        motor_stop( side );
    }
} 

typedef enum motor_status_
{
    MOTOR_FORWARD,
    MOTOR_BACKWARD,
    MOTOR_TURN_RIGHT,
    MOTOR_TURN_LEFT,
    MOTOR_STOP
}motor_status_t;

#define MOTOR_POWER_STEP                40
#define MOTOR_POWER_INCREMENT_TIME_MS   100
#define MOTOR_POWER_MAX                 255
#define MOTOR_POWER_MIN                 -255

motor_status_t motor_status = MOTOR_STOP;
int            motor_power  = 0;
#endif

#ifdef SERVO_TEST
#include <Servo.h>

Servo servo;
int servo_pin = 24;
#endif

#ifdef ENABLE_ENCODERS
int encoder_left_pin_A  = 3;
int encoder_right_pin_A = 2;
int encoder_left_pin_B  = 22;
int encoder_right_pin_B = 23;

volatile long encoder_turnage_right = 0;
volatile long encoder_turnage_left  = 0;

void encoder_sensor_left() {
    if ( digitalRead( encoder_left_pin_B ) == HIGH )
        encoder_turnage_left++;
    else
        encoder_turnage_left--;
}

void encoder_sensor_right() {
    if ( digitalRead( encoder_right_pin_B ) == HIGH )
        encoder_turnage_right--;
    else
        encoder_turnage_right++;
}
#endif

#ifdef IR_RECEIVER
#define IR_UP_BUTTON_CODE       0x60
#define IR_DOWN_BUTTON_CODE     0x06
#define IR_RIGHT_BUTTON_CODE    0x18
#define IR_LEFT_BUTTON_CODE     0x1E
#endif

#ifdef ENABLE_BUZZER
#include "notes.h"
int buzzer_pin = 35;

int melody[] = {
  NOTE_E7, NOTE_E7, 0, NOTE_E7,
  0, NOTE_C7, NOTE_E7, 0,
  NOTE_G7, 0, 0,  0,
  NOTE_G6, 0, 0, 0,
 
  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,
 
  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0,
 
  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,
 
  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0
};
//Mario main them tempo
int tempo[] = {
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
 
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
 
  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
 
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
 
  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
};

void buzz ( int targetPin, long frequency, long length ) 
{
    long delayValue = 1000000 / frequency / 2; // calculate the delay value between transitions
    //// 1 second's worth of microseconds, divided by the frequency, then split in half since
    //// there are two phases to each cycle
    long numCycles = frequency * length / 1000; // calculate the number of cycles for proper timing
    //// multiply frequency, which is really cycles per second, by the number of seconds to
    //// get the total number of cycles to produce
    for (long i = 0; i < numCycles; i++) { // for the calculated length of time...
        digitalWrite(targetPin, HIGH); // write the buzzer pin high to push out the diaphram
        delayMicroseconds(delayValue); // wait for the calculated delay value
        digitalWrite(targetPin, LOW); // write the buzzer pin low to pull back the diaphram
        delayMicroseconds(delayValue); // wait again or the calculated delay value
    }
}

#endif
//---------------------------------------------------------

void setup ( void )
{
#ifdef ENABLE_BUZZER
    // pinMode( buzzer_pin, OUTPUT );
#endif

#ifdef IR_RECEIVER
    Serial2.begin( 2400 );
#endif

    Serial.begin( 115200 );
    Serial3.begin( 115200 );

#ifdef SERVO_TEST
    servo.attach( servo_pin );
    servo.write( 90 );
    delay( 10000 );
#endif

#ifdef MOTOR_CONTROL
    pinMode( motorR_pos, OUTPUT );
    pinMode( motorR_neg, OUTPUT );
    pinMode( motorL_pos, OUTPUT );
    pinMode( motorL_neg, OUTPUT );
#endif

#ifdef ENABLE_LCD
    lcd.begin( 16, 2 );
    analogWrite( 8, 100 );
#endif

#ifdef ENABLE_ENCODERS
    pinMode( encoder_left_pin_A,  INPUT_PULLUP );
    pinMode( encoder_right_pin_A, INPUT_PULLUP );
    pinMode( encoder_left_pin_B,  INPUT );
    pinMode( encoder_right_pin_B, INPUT );

    attachInterrupt( 1,  encoder_sensor_left, RISING );
    attachInterrupt( 0,  encoder_sensor_right, RISING );
#endif
// 
    Serial.println( "/---Start---/" );
}
//---------------------------------------------------------
void loop ( void )
{

#if defined(IR_RECEIVER)
#define sign(x) (x > 0 ? 1 : x < 0 ? -1 : 0)

#define IR_TIMEOUT_MS       100
#define MOTOR_CONST_POWER   100
    static int motor_left_desired_power    = 0;
    static int motor_right_desired_power   = 0;

    static int motor_left_power            = 0;
    static int motor_right_power           = 0;

    static int motor_left_inc              = 0;
    static int motor_right_inc             = 0;

    static unsigned long motor_event_time_ms = 0;
    static unsigned long command_time_ms = 0;

    byte input = Serial2.read();
    switch( input ) {
        case IR_UP_BUTTON_CODE:
            if ( motor_status != MOTOR_FORWARD )
                Serial.println( "Forward" );

            motor_status = MOTOR_FORWARD;
            motor_right_desired_power = motor_left_desired_power = MOTOR_CONST_POWER;
            command_time_ms = millis();
            break;      
        case IR_DOWN_BUTTON_CODE:
            if ( motor_status != MOTOR_BACKWARD )
                Serial.println( "Backward" );

            motor_status = MOTOR_BACKWARD;
            motor_right_desired_power = motor_left_desired_power = -MOTOR_CONST_POWER/2;
            command_time_ms = millis();
            break;      
        case IR_LEFT_BUTTON_CODE:
            if ( motor_status != MOTOR_TURN_LEFT )
                Serial.println( "Left" );

            motor_status = MOTOR_TURN_LEFT;
            motor_right_desired_power = MOTOR_CONST_POWER;
            motor_left_desired_power  = -MOTOR_CONST_POWER;
            command_time_ms = millis();
            break;      
        case IR_RIGHT_BUTTON_CODE:
            if ( motor_status != MOTOR_TURN_RIGHT )
                Serial.println( "Right" );
  
            motor_status = MOTOR_TURN_RIGHT;
            motor_right_desired_power = -MOTOR_CONST_POWER;
            motor_left_desired_power  = MOTOR_CONST_POWER;
            command_time_ms = millis();
            break;  
        default:
            if ( millis() - command_time_ms > IR_TIMEOUT_MS ) {
                if ( motor_status != MOTOR_STOP )
                    Serial.println( "Stop" );

                motor_status = MOTOR_STOP;
                motor_right_desired_power = motor_left_desired_power = 0;
            }
    }

    if ( millis() - motor_event_time_ms > MOTOR_POWER_INCREMENT_TIME_MS ) {
        motor_event_time_ms = millis();

        motor_right_inc = sign( motor_right_desired_power - motor_right_power ) * MOTOR_POWER_STEP;
        motor_left_inc = sign( motor_left_desired_power - motor_left_power ) * MOTOR_POWER_STEP;

        if ( motor_right_inc ) {
            motor_right_power += motor_right_inc;
            if ( (motor_right_inc > 0 && motor_right_power > motor_right_desired_power) || 
                 (motor_right_inc < 0 && motor_right_power < motor_right_desired_power) )
                motor_right_power = motor_right_desired_power;
        }

        if ( motor_right_inc ) {
            motor_left_power += motor_left_inc;
            if ( (motor_left_inc > 0 && motor_left_power > motor_left_desired_power) || 
                 (motor_left_inc < 0 && motor_left_power < motor_left_desired_power) )
                motor_left_power = motor_left_desired_power;
        }
    }
#ifdef ENABLE_MOTORS
    motor_start( motor_right_power, MOTOR_RIGHT );
    motor_start( motor_left_power, MOTOR_LEFT );
#endif

#ifdef ENABLE_BUZZER
#define SOUND_BACK_DELAY_MS 600
    static unsigned long buzzer_stop_time_ms = 0;
    static bool back_sound_enable = false;

    static bool mario_soubnd_enable = false;
    static unsigned long buzzer_mario_time_ms = 0;
    static int current_note = 0;
    static unsigned int current_delay = 0;

    if ( motor_status == MOTOR_BACKWARD ) {
        if ( millis() - buzzer_stop_time_ms > SOUND_BACK_DELAY_MS ) {
            buzzer_stop_time_ms = millis();

            if ( back_sound_enable ) {
                noTone(buzzer_pin);
            } else {
                tone(buzzer_pin, 1432); 
            }
            back_sound_enable = !back_sound_enable;
        }
    } else if ( motor_status == MOTOR_FORWARD ) {
        if ( millis() - buzzer_mario_time_ms > current_delay ) {
            buzzer_mario_time_ms = millis();

            if ( mario_soubnd_enable ) {
                noTone(buzzer_pin);

                current_delay = 0;
            } else {
                int noteDuration = 1100 / tempo[current_note];
                current_delay = noteDuration * 1.30;

                tone(buzzer_pin, melody[current_note++], noteDuration);

                int size_melody = sizeof(melody) / sizeof(int);
                if ( current_note == size_melody )
                    current_note = 0;
            }
            mario_soubnd_enable = !mario_soubnd_enable;
        }
    } else {
        // current_note = 0;
        noTone(buzzer_pin);
        mario_soubnd_enable = false;
        back_sound_enable = false;
    }

    // Serial.println(" 'Mario Theme'");
    
    // for (int thisNote = 0; thisNote < size_melody; thisNote++) {
 
    //     // to calculate the note duration, take one second
    //     // divided by the note type.
    //     //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    //     int noteDuration = 1100 / tempo[thisNote];

    //     // tone( buzzer_pin, melody[thisNote], noteDuration );

    //     // to distinguish the notes, set a minimum time between them.
    //     // the note's duration + 30% seems to work well:
    //     int pauseBetweenNotes = noteDuration * 1.30;
    //     delay(pauseBetweenNotes);

    //     // stop the tone playing:
    //     // buzz(buzzer_pin, 0, noteDuration);
    //     noTone( buzzer_pin );
    // }

#endif  

#ifdef ENABLE_POTENTIOMETER
    Serial.println( analogRead( A5 ) );
    delay( 100 );
#endif
#endif

#ifdef ENABLE_SHARP_IR
    delay( 50 );
    int sharp_ir_adc = analogRead( A1 );
    int val = (6762 / (sharp_ir_adc - 9)) - 4;

    Serial.print( "Sharp " );
    Serial.println( val );
#endif
    // Serial.println( "Hello =)" );
#ifdef VOLTAGE_CONTROL
#define SEND_DATA_PERIOD_MS 1000
    static long previous_send_data_voltage_ms = 0;
    int voltage_adc = analogRead( A0 );
    float voltage = voltage_adc * 4.15 /*Bad voltage */ * 2/1023;
    if ( millis() - previous_send_data_voltage_ms > SEND_DATA_PERIOD_MS ) {
        Serial3.println( voltage );
        previous_send_data_voltage_ms = millis();
    }
#endif

#ifdef PWM_TEST
#define CHANGE_BRIGHTNESS_DELAY_MS 30
    int brightness = 0;
    for ( brightness = 0; brightness <= 255; brightness++ ) {
        analogWrite( 13, brightness );
        delay( CHANGE_BRIGHTNESS_DELAY_MS );
    }

    delay( 500 );
    
    for ( brightness = 255; brightness >= 0; brightness-- ) {
        analogWrite( 13, brightness );
        delay( CHANGE_BRIGHTNESS_DELAY_MS );
    }
    
    delay( 500 );
#endif

#ifdef SERVO_TEST
#define SERVO_DELAY 15
#define SERVO_BORDER_OFFSET 60
#define SERVO_MAX_ANGLE     (180 - SERVO_BORDER_OFFSET)
#define SERVO_MIN_ANGLE     (0   + SERVO_BORDER_OFFSET)
    int servo_angle;
    for ( servo_angle = SERVO_MIN_ANGLE; servo_angle <= SERVO_MAX_ANGLE; servo_angle++ ) {
        servo.write( servo_angle );
        delay( SERVO_DELAY );
    }

    delay( 500 );
    
    for ( servo_angle = SERVO_MAX_ANGLE; servo_angle >= SERVO_MIN_ANGLE; servo_angle-- ) {
        servo.write( servo_angle );
        delay( SERVO_DELAY );
    }
    
    delay( 2000 );
#endif

    // int power_dutyCycle = analogRead( A1 );
    // power_dutyCycle = map( power_dutyCycle, 0, 1023, -127, 127 );

// #define DEAD_ZONE 10
    // if ( -DEAD_ZONE < power_dutyCycle && power_dutyCycle < DEAD_ZONE )
        // power_dutyCycle = 0;

    // Serial.print( power_dutyCycle );
    // Serial.print( " " );
#ifdef ENABLE_ENCODERS
#define ENCODERS_PERIOD 100
    static long previous_send_data_encoders_ms = 0;
    if ( millis() - previous_send_data_encoders_ms > ENCODERS_PERIOD ) {
        Serial3.print( encoder_turnage_left );
        Serial3.print( " " );
        Serial3.println( encoder_turnage_right );
        previous_send_data_encoders_ms = millis();
    }
#endif

#if defined(MOTOR_CONTROL) && defined(ENABLE_BT_CONTROL)
    if ( Serial3.available() ) {    
        char input = Serial3.read();

        switch( input ) {
            case 'W':
                if ( motor_status != MOTOR_FORWARD ) {
                    Serial3.println( "Forward" );
                    motor_power = 0;
                    motor_status = MOTOR_FORWARD;
                } 
                motor_power += POWER_STEP;
                break;      
            case 'S':
                if ( motor_status != MOTOR_BACKWARD ) {
                    Serial3.println( "Backward" );
                    motor_power = 0;
                    motor_status = MOTOR_BACKWARD;
                } 
                motor_power += POWER_STEP;        
                break;      
            case 'A':
                if ( motor_status != MOTOR_TURN_LEFT ) {
                    Serial3.println( "Left" );
                    motor_power = 0;
                    motor_status = MOTOR_TURN_LEFT;
                } 
                motor_power += POWER_STEP;     
                break;      
            case 'D':
                if ( motor_status != MOTOR_TURN_RIGHT ) {
                    Serial3.println( "Right" );
                    motor_power = 0;
                    motor_status = MOTOR_TURN_RIGHT;
                } 
                motor_power += POWER_STEP;       
                break;
            case 'E':
                encoder_turnage_right = encoder_turnage_left = 0;      
                break;
            case 'Z':     
            // default:
                Serial3.println( "Stop" );
                motor_power = 0;
                motor_status = MOTOR_STOP;
        } 
    }

    motor_power = motor_power > POWER_MAX ? POWER_MAX :
                  motor_power < POWER_MIN ? POWER_MIN : motor_power;

    switch ( motor_status ) {
        case MOTOR_FORWARD:
            motor_start( motor_power, MOTOR_RIGHT | MOTOR_LEFT );
            break;
        case MOTOR_BACKWARD:
            motor_start( -motor_power, MOTOR_RIGHT | MOTOR_LEFT );
            break;
        case MOTOR_TURN_RIGHT:
            motor_start( motor_power, MOTOR_LEFT );
            motor_start( -motor_power, MOTOR_RIGHT );
            break;
        case MOTOR_TURN_LEFT:
            motor_start( motor_power, MOTOR_RIGHT );
            motor_start( -motor_power, MOTOR_LEFT );
            break;
        case MOTOR_STOP:
            motor_stop( MOTOR_RIGHT | MOTOR_LEFT );
            break;
        default:
            ;
    }
#endif

#ifdef SERIAL_ECHO
    if ( Serial3.available() )
        Serial.write( Serial3.read() );

    if ( Serial.available() )
        Serial3.write( Serial.read() );
#endif
    
#ifdef ENABLE_LCD
    lcd.clear();  
    lcd.home();  
    lcd.print("hello, world!");
    lcd.setCursor(6, 1);
    lcd.print(millis() / 1000); 

    delay( 1000 );
#endif
}
