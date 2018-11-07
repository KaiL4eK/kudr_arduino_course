#include "test.h"

void setup() {

    test_lcd();
}

void loop() {
    
    test_joystick();
    delay( 1000 );

    test_motors_encoders();
    delay( 1000 );

    // test_ir_servo();
    // delay(1000);
}

/*****************/
/*** TEST ZONE ***/
/*****************/

/* Just render strings */
void test_lcd ( void )
{
    int LCD_contrast_pin = 8;
    // RS, E, DB4, DB5, DB6, DB7
    LiquidCrystal lcd(22, 23, 24, 25, 26, 27);

    analogWrite(LCD_contrast_pin, 127);
    lcd.begin(16, 2);
    // Digits in the end of string - to fill view canvas completely
    lcd.print("First String 123");
    lcd.setCursor(0, 1);
    lcd.print("Second String 12");
}


/* One minute test with Serial output */
void test_joystick ( void )
{
    int joystick_x_pin = A6;
    int joystick_y_pin = A7;
    int joystick_btn_pin = 38;

    pinMode( joystick_x_pin, INPUT );
    pinMode( joystick_y_pin, INPUT );
    pinMode( joystick_btn_pin, INPUT_PULLUP );
    
    char buffer[64];

    Serial.begin( 9600 );
    Serial.write( "Joystick test start (one minute) / Press <Enter> to quit\n" );

    for ( int i = 0; i < 60; i++ )
    {
        int joy_btn_state = digitalRead( joystick_btn_pin );
        int joy_x_state = analogRead( joystick_x_pin );
        int joy_y_state = analogRead( joystick_y_pin );

        sprintf( buffer, "States: X: %04d / Y: %04d / Btn: %01d\n", joy_x_state, joy_y_state, joy_btn_state );
        Serial.write( buffer );

        delay( 1000 );

        if ( Serial.available() )
        {
            if ( Serial.read() == '\n' )
            {
                Serial.write( "User test interrupt\n" );
                break;
            }
        }
    }

    Serial.write( "Joystick test end\n" );
}

void test_ir_servo ( void )
{
    int IR_servo_pin = 34;

    Servo servo;
    servo.attach( IR_servo_pin );

    char buffer[64];

    Serial.begin( 9600 );
    Serial.write( "IR servo test start (one minute) / Press <Enter> to quit\n" );

    for ( int i = 0; i < 60; i++ )
    {
        // y = 180 - abs( 6*x-180 ) - linear equation of triangle
        int current_angle = 180 - abs(6 * i - 180);

        servo.write( current_angle );

        sprintf( buffer, "State: Angle: %d\n", current_angle );
        Serial.write( buffer );

        delay( 1000 );

        if ( Serial.available() )
        {
            if ( Serial.read() == '\n' )
            {
                Serial.write( "User test interrupt\n" );
                break;
            }
        }      
    }

    Serial.write( "IR servo test end\n" );
}

int motorR_pos = 5;  
int motorR_neg = 6; 
int motorL_pos = 4;  
int motorL_neg = 7;

int encoder_right_pin_A = 2;
int encoder_right_pin_B = 19;
int encoder_left_pin_A  = 3;
int encoder_left_pin_B  = 18;

/* Power = [0; 255] */
static void motor_left_power ( int power )  
{    
    if ( power > 0 ) 
    {
        digitalWrite( motorL_neg, LOW );      
        analogWrite( motorL_pos, power );    
    } 
    else if ( power < 0 ) 
    {      
        digitalWrite( motorL_pos, LOW );      
        analogWrite( motorL_neg, -power ); 
    } 
    else 
    {
        digitalWrite( motorL_pos, LOW );
        digitalWrite( motorL_neg, LOW );
    }
} 

/* Power = [0; 255] */
static void motor_right_power ( int power )  
{    
    if ( power > 0 ) 
    {
        digitalWrite( motorR_neg, LOW );      
        analogWrite( motorR_pos, power );    
    } 
    else if ( power < 0 ) 
    {      
        digitalWrite( motorR_pos, LOW );     
        analogWrite( motorR_neg, -power );
    } 
    else 
    {
        digitalWrite( motorR_pos, LOW );
        digitalWrite( motorR_neg, LOW );
    }
} 

volatile long encoder_turnage_left  = 0;

static void encoder_sensor_left() {
    if ( digitalRead( encoder_left_pin_B ) == HIGH )
        encoder_turnage_left--;
    else
        encoder_turnage_left++;
}

volatile long encoder_turnage_right = 0;

static void encoder_sensor_right() {
    if ( digitalRead( encoder_right_pin_B ) == HIGH )
        encoder_turnage_right--;
    else
        encoder_turnage_right++;
}

void test_motors_encoders ( void )
{
    pinMode( motorR_pos, OUTPUT );
    pinMode( motorR_neg, OUTPUT );
    pinMode( motorL_pos, OUTPUT );
    pinMode( motorL_neg, OUTPUT );

    pinMode( encoder_left_pin_A,  INPUT );
    pinMode( encoder_right_pin_A, INPUT );
    pinMode( encoder_left_pin_B,  INPUT );
    pinMode( encoder_right_pin_B, INPUT );

    attachInterrupt( 1,  encoder_sensor_left, RISING );
    attachInterrupt( 0,  encoder_sensor_right, RISING );

    int r_motor_power = 0;
    int l_motor_power = 0;

    char buffer[64];

    Serial.begin( 9600 );
    Serial.write( "Motors test start (one minute) / Press <Enter> to quit\n" );

    for ( int i = 0; i < 60; i++ )
    {
        if ( i < 10 )
        {
            l_motor_power = 100;
            r_motor_power = -100; 
        }
        else if ( i < 20 )
        {
            l_motor_power = -100;
            r_motor_power = 100; 
        }
        else
        {
            break;
        }

        motor_left_power( l_motor_power );
        motor_right_power( r_motor_power );

        sprintf( buffer, "State: Power L: %d / R: %d | Encoders: L: %d / R: %d\n", 
                                l_motor_power, r_motor_power, 
                                encoder_turnage_left, encoder_turnage_right );
        Serial.write( buffer );

        delay( 1000 );

        if ( Serial.available() )
        {
            if ( Serial.read() == '\n' )
            {
                Serial.write( "User test interrupt\n" );
                break;
            }
        }      
    }

    motor_left_power( 0 );
    motor_right_power( 0 );

    Serial.write( "Motors test end\n" );
}
