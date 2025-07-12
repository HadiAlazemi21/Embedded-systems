#include "mbed.h"

// Analog inputs
AnalogIn ldrRaw(A0);      // Raw LDR signal (before RC filter)
AnalogIn ldrFiltered(A3); // Hardware-filtered LDR signal (after RC filter)

// Serial output
BufferedSerial pc(USBTX, USBRX, 9600);

// Software filter variable
float swFiltered = 0.0;

int main() {
    printf("LDR Signal Processing with Hardware and Software Low-Pass Filters\n");

    while (true) {
        // 1. Read raw LDR signal (before hardware filter)
        float rawLdr = ldrRaw.read(); // Value from 0.0 to 1.0

        // 2. Read hardware filtered signal (after RC filter)
        float hwFiltered = ldrFiltered.read();

        // 3 Print all values
        printf("Raw A0: %.3f\tHardware LPF A3: %.3f\n",
               rawLdr, hwFiltered);

        ThisThread::sleep_for(200ms);
    }
}
