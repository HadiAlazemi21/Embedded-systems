#include "mbed.h"
#include "arm_book_lib.h"

int main()
{
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

    gasDetector.mode(PullDown);
    overTempDetector.mode(PullDown);
    aButton.mode(PullDown);
    bButton.mode(PullDown);
    cButton.mode(PullDown);
    dButton.mode(PullDown);

    alarmLed = 0;
    incorrectCodeLed = 0;
    systemBlockedLed = 0;

    bool alarmState = 0;
    int numberOfIncorrectCodes = 0;
    bool emergency_state = 0;

    while (true) {

        // Emergency detection
        if (gasDetector || overTempDetector) {
            alarmState = 1;
        }
        if (gasDetector && overTempDetector) {
            thread_sleep_for(1000); // confirm for 1 second to prevent faulse emergency triggers 
            if (gasDetector && overTempDetector) {
                  emergency_state = 1;
            
        }
}

        // Emergency handling
        if (emergency_state) {
            // Blink alarm LED
            alarmLed = 1; //Turning on alarm led (LED1)
            thread_sleep_for(200);
            alarmLed = 0; //Turning OFF alarm led (LED1)
            thread_sleep_for(100);

            // Check security code dynamically
            bool securitycode = aButton && bButton && cButton;

            if (securitycode && numberOfIncorrectCodes < 4) {
                emergency_state = 0;
                alarmState = 0;
                alarmLed = 0;
                incorrectCodeLed = 0;
                numberOfIncorrectCodes=0;

            } else if (!securitycode && (aButton || bButton || cButton)) {
                
                incorrectCodeLed = 1;
                thread_sleep_for(300);//incorrect led delay
                incorrectCodeLed = 0;
                numberOfIncorrectCodes++;
            }
        }

        // System blocked indicator
        if (numberOfIncorrectCodes >= 4) {
            systemBlockedLed = 1;
            thread_sleep_for(20000);//system is blocked for 20s =20000ms;
            systemBlockedLed = 0;
            numberOfIncorrectCodes = 0;
            alarmLed = 1;

        }

        // Show alarm LED if alarmState active and not in blinking emergency mode
        if (!emergency_state) {
            alarmLed = alarmState;
        }
    }
}
