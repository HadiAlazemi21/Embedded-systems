//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"

//=====[Declaration and initialization of public global objects]===============

UnbufferedSerial uartUsb(USBTX, USBRX, 115200);

AnalogIn potentiometer(A0);
AnalogIn lm35(A1); // 10 mV/\xB0 C
DigitalInOut sirenPin(PE_10);
DigitalIn mq2(PE_12);

//=====[Declaration and initialization of public global variables]=============

bool quit = false;

char receivedChar = '\0';

float lm35Reading = 0.0; // Raw ADC input A0 value
float lm35TempC = 0.0;   // Temperature in Celsius degrees [\xB0 C]

float potentiometerReading = 0.0;   // Raw ADC input A1 value

//=====[Declarations (prototypes) of public functions]=========================

void availableCommands();
void uartTask();
void pcSerialComStringWrite( const char* str );
char pcSerialComCharRead();

float analogReadingScaledWithTheLM35Formula( float analogReading );


//=====[Main function, the program entry point after power on or reset]========

int main()
{
    sirenPin.mode(OpenDrain);
    sirenPin.input();
    availableCommands();
    while( true ) {
        uartTask();
    }
}

//=====[Implementations of public functions]===================================

void availableCommands()
{
    pcSerialComStringWrite( "\r\nPress the following keys to continuously " );
    pcSerialComStringWrite( "print the readings until 'q' is pressed:\r\n" );

    pcSerialComStringWrite(" - 'a' the reading at the analog pin A0 ");
    pcSerialComStringWrite("(connected to the potentiometer)\r\n");

    pcSerialComStringWrite(" - 'b' the reading at the analog pin A1 ");
    pcSerialComStringWrite("(connected to the LM35)\r\n");

    pcSerialComStringWrite(" - 'c' the reading of the temperature measured ");
    pcSerialComStringWrite("by the LM35 expressed in \xB0 C\r\n");
a
    pcSerialComStringWrite(" - 'g' the reading of the DOUT signal of the ");
    pcSerialComStringWrite("MQ-2 gas sensor \r\n");

    pcSerialComStringWrite("\r\nWARNING: The readings are printed continuously ");
    pcSerialComStringWrite("until 'q' or 'Q' are pressed.\r\n\r\n");
}

void uartTask()
{
    char receivedChar = '\0';
    char str[100] = "";
    receivedChar = pcSerialComCharRead();
    if( receivedChar !=  '\0') {
        switch (receivedChar) {

        // Print potentiometer ADC input (A1) raw value until pulse 'q' or 'Q'
        case 'a':
        case 'A':
            while( !(receivedChar == 'q' || receivedChar == 'Q') ) {
                potentiometerReading = potentiometer.read();
                str[0] = '\0';
                sprintf ( str, "Potentiometer reading: %.2f\r\n", potentiometerReading);
                pcSerialComStringWrite( str );
                delay( 200 );
                receivedChar = pcSerialComCharRead();
            }
            break;

        // Continuously print LM35 sensor ADC input (A0) raw value until pulse
        // 'q' or 'Q'
        case 'b':
        case 'B':
            while( !(receivedChar == 'q' || receivedChar == 'Q') ) {
                lm35Reading = lm35.read();
                str[0] = '\0';
                sprintf ( str, "LM35 reading: %.2f\r\n", lm35Reading);
                pcSerialComStringWrite( str );
                delay( 200 );
                receivedChar = pcSerialComCharRead();
            }
            break;

        // Continuously print LM35 sensor ADC input (A0) value scaled to
        // Celsius degrees until pulse 'q' or 'Q'
        case 'c':
        case 'C':
            while( !(receivedChar == 'q' || receivedChar == 'Q') ) {             
                lm35Reading = lm35.read();
                lm35TempC = analogReadingScaledWithTheLM35Formula(lm35Reading);
                str[0] = '\0';
                sprintf ( str, "LM35: %.2f \xB0 C\r\n", lm35TempC);
                pcSerialComStringWrite( str );
                delay( 200 );
                receivedChar = pcSerialComCharRead();
            }
            break;

        case 'g':
        case 'G':
            while( !(receivedChar == 'q' || receivedChar == 'Q') ) {
                if ( !mq2 ) {
                    pcSerialComStringWrite( "Gas is being detected\r\n");
                    sirenPin.output();                                     
                    sirenPin = LOW;                        
                } else {
                    pcSerialComStringWrite( "Gas is not being detected\r\n");
                    sirenPin.input();
                }
                delay( 200 );
                receivedChar = pcSerialComCharRead();
            }
            break;

        default:
            availableCommands();
            break;
        }
    }
}

float analogReadingScaledWithTheLM35Formula( float analogReading )
{
    return analogReading * 330.0;
}

void pcSerialComStringWrite( const char* str )
{
    uartUsb.write( str, strlen(str) );
}

char pcSerialComCharRead()
{
    char receivedChar = '\0';
    if( uartUsb.readable() ) {
        uartUsb.read( &receivedChar, 1 );
    }
    return receivedChar;
}