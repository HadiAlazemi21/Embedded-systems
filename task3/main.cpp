//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"

//=====[Declaration and initialization of public global objects]===============

DigitalIn enterButton(BUTTON1);
DigitalIn gasDetector(D2);
DigitalIn overTempDetector(D3);
DigitalIn aButton(D4);
DigitalIn bButton(D5);
DigitalIn cButton(D6);
DigitalIn dButton(D7);

DigitalOut alarmLed(LED1);
DigitalOut incorrectCodeLed(LED3);
DigitalOut systemBlockedLed(LED2);

UnbufferedSerial uartUsb(USBTX, USBRX, 115200);

//=====[Declaration and initialization of public global variables]=============

bool alarmState = OFF;
int numberOfIncorrectCodes = 0;
bool gas_warning_sent = false;
bool temp_warning_sent = false;

//=====[Declarations (prototypes) of public functions]=========================

void inputsInit();
void outputsInit();
void alarmActivationUpdate();
void alarmDeactivationUpdate();
void uartTask();
void availableCommands();

//=====[Main function, the program entry point after power on or reset]========

int main()
{
    inputsInit();
    outputsInit();
    while (true) {
        alarmActivationUpdate();
        alarmDeactivationUpdate();
        uartTask();
        ThisThread::sleep_for(200ms);
    }
}

//=====[Implementations of public functions]===================================

void inputsInit()
{
    gasDetector.mode(PullDown);
    overTempDetector.mode(PullDown);
    aButton.mode(PullDown);
    bButton.mode(PullDown);
    cButton.mode(PullDown);
    dButton.mode(PullDown);
}

void outputsInit()
{
    alarmLed = OFF;
    incorrectCodeLed = OFF;
    systemBlockedLed = OFF;
}

void alarmActivationUpdate()
{
    if ( gasDetector || overTempDetector ) {
        alarmState = ON;
    }
    alarmLed = alarmState;
}

void alarmDeactivationUpdate()
{
    if ( numberOfIncorrectCodes < 5 ) {
        if ( aButton && bButton && cButton && dButton && !enterButton ) {
            incorrectCodeLed = OFF;
        }
        if ( enterButton && !incorrectCodeLed && alarmState ) {
            if ( aButton && bButton && !cButton && !dButton ) {
                alarmState = OFF;
                numberOfIncorrectCodes = 0;
            } else {
                incorrectCodeLed = ON;
                numberOfIncorrectCodes++;
            }
        }
    } else {
        systemBlockedLed = ON;
    }
}

void uartTask()
{
    char receivedChar = '\0';

    if( uartUsb.readable() ) {
        uartUsb.read( &receivedChar, 1 );

        if ( receivedChar == '1' ) {
            if ( gasDetector.read() ) {
                const char* msg = "Gas detector is ON\r\n";
                uartUsb.write(msg, strlen(msg));
            } else {
                const char* msg = "Gas detector is OFF\r\n";
                uartUsb.write(msg, strlen(msg));
            }
        }
        else if ( receivedChar == '9' ) {
            if ( overTempDetector.read() ) {
                const char* msg = "Over-temp detector is ON\r\n";
                uartUsb.write(msg, strlen(msg));
            } else {
                const char* msg = "Over-temp detector is OFF\r\n";
                uartUsb.write(msg, strlen(msg));
            }
        }
        else {
            availableCommands();
        }
    }

    //=====[Automatic Warning System]=====
    if ( gasDetector.read() ) {
        if (!gas_warning_sent) {
            const char* warn = "WARNING: Unsafe gas levels detected!\r\n";
            uartUsb.write(warn, strlen(warn));
            gas_warning_sent = true;
        }
    } else {
        gas_warning_sent = false;
    }

    if ( overTempDetector.read() ) {
        if (!temp_warning_sent) {
            const char* warn = "WARNING: Unsafe temperature detected!\r\n";
            uartUsb.write(warn, strlen(warn));
            temp_warning_sent = true;
        }
    } else {
        temp_warning_sent = false;
    }
}

void availableCommands()
{
    const char* msg1 = "Available commands:\r\n";
    uartUsb.write(msg1, strlen(msg1));

    const char* msg2 = "Press '1' to get the gas detector state\r\n";
    uartUsb.write(msg2, strlen(msg2));

    const char* msg3 = "Press '9' to get the over-temp detector state\r\n";
    uartUsb.write(msg3, strlen(msg3));
}
