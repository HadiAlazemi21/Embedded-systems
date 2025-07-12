//=====[Libraries]=============================================================

#include "mbed.h"
#include "arm_book_lib.h"

#include "user_interface.h"

#include "code.h"
#include "siren.h"
#include "smart_home_system.h"
#include "fire_alarm.h"
#include "date_and_time.h"
#include "temperature_sensor.h"
#include "gas_sensor.h"
#include "matrix_keypad.h"
#include "display.h"

//=====[Declaration of private defines]========================================

#define DISPLAY_REFRESH_TIME_MS 1000

//=====[Declaration of private data types]=====================================

//=====[Declaration and initialization of public global objects]===============

DigitalOut incorrectCodeLed(LED3);
DigitalOut systemBlockedLed(LED2);

//=====[Declaration of external public global variables]=======================

//=====[Declaration and initialization of public global variables]=============

char codeSequenceFromUserInterface[CODE_NUMBER_OF_KEYS];

//=====[Declaration and initialization of private global variables]============

static bool incorrectCodeState = OFF;
static bool systemBlockedState = OFF;

static bool codeComplete = false;
static int numberOfCodeChars = 0;

//=====[Declarations (prototypes) of private functions]========================

static void userInterfaceMatrixKeypadUpdate();
static void incorrectCodeIndicatorUpdate();
static void systemBlockedIndicatorUpdate();

static void userInterfaceDisplayInit();
static void userInterfaceDisplayUpdate();

//=====[Implementations of public functions]===================================

void userInterfaceInit()
{
    incorrectCodeLed = OFF;
    systemBlockedLed = OFF;
    matrixKeypadInit( SYSTEM_TIME_INCREMENT_MS );
    userInterfaceDisplayInit();
}

void userInterfaceUpdate()
{
    userInterfaceMatrixKeypadUpdate();
    incorrectCodeIndicatorUpdate();
    systemBlockedIndicatorUpdate();
    userInterfaceDisplayUpdate();

    if (userInterfaceCodeCompleteRead()) {
        bool correct = codeMatchFrom(CODE_KEYPAD);
        displayCharPositionWrite(0, 3); // Feedback line

        if (correct) {
            displayStringWrite("Code Accepted    ");
        } else {
            displayStringWrite("Incorrect Code   ");
        }
    }
}

bool incorrectCodeStateRead()
{
    return incorrectCodeState;
}

void incorrectCodeStateWrite( bool state )
{
    incorrectCodeState = state;
}

bool systemBlockedStateRead()
{
    return systemBlockedState;
}

void systemBlockedStateWrite( bool state )
{
    systemBlockedState = state;
}

bool userInterfaceCodeCompleteRead()
{
    return codeComplete;
}

void userInterfaceCodeCompleteWrite( bool state )
{
    codeComplete = state;
}

//=====[Implementations of private functions]==================================

static void userInterfaceMatrixKeypadUpdate()
{
    static int numberOfHashKeyReleased = 0;
    char keyReleased = matrixKeypadUpdate();

    // Show gas status when '1' is pressed
    if (keyReleased == '1') {
        displayCharPositionWrite(0, 3);
        if (gasDetectorStateRead()) {
            displayStringWrite("Gas: Detected    ");
        } else {
            displayStringWrite("Gas: Not Detected");
        }
    }

    // Show temperature when '9' is pressed
    if (keyReleased == '9') {
        displayCharPositionWrite(0, 3);
        char tempStr[17];
        sprintf(tempStr, "Temp: %.0f'C       ", temperatureSensorReadCelsius());
        displayStringWrite(tempStr);
    }
    if(sirenStateRead()){
         // === Code entry via keypad ===
     if (keyReleased != '\0') {
        if (!systemBlockedStateRead()) {
            if (!incorrectCodeStateRead()) {
                codeSequenceFromUserInterface[numberOfCodeChars] = keyReleased;
                numberOfCodeChars++;
                if (numberOfCodeChars >= CODE_NUMBER_OF_KEYS) {
                    codeComplete = true;
                    numberOfCodeChars = 0;
                }
            } else {
                if (keyReleased == '#') {
                    numberOfHashKeyReleased++;
                    if (numberOfHashKeyReleased >= 2) {
                        numberOfHashKeyReleased = 0;
                        numberOfCodeChars = 0;
                        codeComplete = false;
                        incorrectCodeStateWrite(OFF);
                    }
                }
            }
        }
      } 
    }  
}


static void userInterfaceDisplayInit()
{
    displayInit(DISPLAY_CONNECTION_I2C_PCF8574_IO_EXPANDER);

    displayCharPositionWrite(0, 0);
    displayStringWrite("Enter Code to");

    displayCharPositionWrite(0, 1);
    displayStringWrite("Deactivate Alarm");

    displayCharPositionWrite(0, 2);
    displayStringWrite("Alarm: ");
    
    displayCharPositionWrite(0, 3);
    displayStringWrite("Status: ---       ");
}

static void userInterfaceDisplayUpdate()
{
    static int accumulatedDisplayTime = 0;

    if (accumulatedDisplayTime >= DISPLAY_REFRESH_TIME_MS) {
        accumulatedDisplayTime = 0;

        // Update only alarm state on row 2 regularly
        displayCharPositionWrite(7, 2);
        if (sirenStateRead()) {
            displayStringWrite("ON ");
        } else {
            displayStringWrite("OFF");
        }

    } else {
        accumulatedDisplayTime += SYSTEM_TIME_INCREMENT_MS;
    }
}


static void incorrectCodeIndicatorUpdate()
{
    incorrectCodeLed = incorrectCodeStateRead();
}

static void systemBlockedIndicatorUpdate()
{
    systemBlockedLed = systemBlockedState;
}