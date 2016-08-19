// #define ENABLE_LCD
// #define SERIAL_ECHO
// #define VOLTAGE_CONTROL
// #define MOTOR_CONTROL
// #define SERVO_TEST
// #define PWM_TEST
// #define ENABLE_ENCODERS
// #define IR_RECEIVER
// #define ENABLE_SHARP_IR

#ifdef ENABLE_LCD
#include <LiquidCrystal.h>

int rs_pin = 45;
int e_pin  = 43;
int d4_pin = 53;
int d5_pin = 51;
int d6_pin = 49;
int d7_pin = 57;

LiquidCrystal lcd(rs_pin, e_pin, d4_pin, d5_pin, d6_pin, d7_pin);
#endif

#ifdef MOTOR_CONTROL
int motorR_pos = 10;  
int motorR_neg = 9; 
int motorL_pos = 11;  
int motorL_neg = 8;

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

#define POWER_STEP 40
#define POWER_MAX  255
#define POWER_MIN  -255

motor_status_t motor_status = MOTOR_STOP;
int            motor_power  = 0;
#endif

#ifdef SERVO_TEST
#include <Servo.h>

Servo servo;
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
//---------------------------------------------------------

void setup ( void )
{
#ifdef PWM_TEST
    pinMode( 13, OUTPUT );
#endif

#ifdef IR_RECEIVER
    Serial1.begin( 2400 );
#endif

    Serial.begin( 115200 );
    Serial3.begin( 115200 );

#ifdef SERVO_TEST
    servo.attach( 24 );
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
    lcd.begin(16, 2);
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
#ifdef IR_RECEIVER
    if ( Serial1.available() ) {
        byte input = Serial1.read();
        if ( input != 0xFE )
            Serial.println( input, HEX );
    }
#endif

#ifdef ENABLE_SHARP_IR
    delay( 50 );
    int sharp_ir_adc = analogRead( A0 );
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

#ifdef MOTOR_CONTROL
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
