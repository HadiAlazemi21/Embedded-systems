/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"


// Blinking rate in milliseconds
#define WAIT_TIME_MS    100
DigitalOut led1(LED1);// Initialise the digital pin LED1 as an output
DigitalOut led2(LED2);// Initialise the digital pin LED2 as an output
DigitalOut led3(LED3);// Initialise the digital pin LED3 as an output

#define BLINKING_RATE     500ms
void make_other_leds_blink(){
        led1 = !led1;
        led2 = !led2;
        led3 = !led3;
        ThisThread::sleep_for(BLINKING_RATE);
        
}
// Function to blink LEDs in sequence
void blink_leds_in_sequence() {
    // LED1
    led1 = 1;  //led1 on
    thread_sleep_for(WAIT_TIME_MS); //500 ms delay
    led1 = 0; //led 1 off
    thread_sleep_for(WAIT_TIME_MS +100);

    // LED2
    led2 = 1;
    thread_sleep_for(WAIT_TIME_MS);
    led2 = 0;
    thread_sleep_for(WAIT_TIME_MS +100);

    // LED3
    led3 = 1;
    thread_sleep_for(WAIT_TIME_MS);
    led3 = 0;
    thread_sleep_for(WAIT_TIME_MS +100);
}

void simultaneous_blinking(){
    int waiton=200;
 led1=1;
 led2=1;
 led3=1;
 thread_sleep_for(waiton);

int waitoff=100;
if(led1 && led2 && led3){
    led1=0;
    led2=0;
    led3=0;
     thread_sleep_for(waitoff);
}
}

void pattern(){
   int waittime_led1=100;

    led1=1;
    thread_sleep_for(waittime_led1);
    led1=0;
    if(led1==0){
       int waittime_led2=150;
        led2=1;
            thread_sleep_for(waittime_led2);
        led2=0;
    }
    if(led2==0 && led1==0){
       int waittime_led3=200;
       led3=1;
       thread_sleep_for(waittime_led3);
       led3=0;
    }
}
int main() {
    while (true) {
      make_other_leds_blink();
    
      
    }
}