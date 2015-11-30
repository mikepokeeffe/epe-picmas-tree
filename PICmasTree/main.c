/* 
 * File:   main.c
 * Author: mhibbett. mike.hibbett@gmail.com, mjhdesigns.com
 *
 * Created on 25 February 2014, 13:47
 */

#include <stdio.h>
#include <stdlib.h>
#include <pic18.h>
#include "config.h"
//#include "lplc-adc.h"
#include "timers.h"
// Choose the appropriate header file
//#include "LCD-Graphic-ST7735S.h"
//#include "LCD-Graphic-ILI9341.h"
#include "interrupts.h"
#include "hardware.h"
//#include "lcd.h"
//#include "pwm.h"

/*
 * 
 */

int main(int argc, char** argv)
{
    unsigned short lpy;
    unsigned char first_sample = 1;
    unsigned int LED = 0;
    unsigned int a = 0;
    unsigned int b = 1;
    unsigned int c = 2;
    unsigned int d = 0;
    unsigned int led_delay = 250;

    // Initialise any low level hardware features
    //HardwareInit();

    // Initialise the Interrupt sub-system
    //InterruptInit();

    // Initialise the Interrupt sub-system
    timer0Init();
    timer1Init();

        // And set the LED pin as an output, LED Off, as it draws 11mA current
    LATBbits.LATB5 = 0;
    
    LATCbits.LATC4 = 0;
    LATCbits.LATC5 = 0;
    LATCbits.LATC6 = 0;
    LATCbits.LATC7 = 0;
    
    LATAbits.LATA7 = 0;
    LATAbits.LATA6 = 0;
    LATCbits.LATC2 = 0;
    LATCbits.LATC3 = 0;
    
    TRISBbits.TRISB5 = 0;
    
    //Buttons Setup
    TRISBbits.TRISB0 = 0;
    TRISBbits.TRISB1 = 0;
    
    TRISCbits.TRISC4 = 0;
    TRISCbits.TRISC5 = 0;
    TRISCbits.TRISC6 = 0;
    TRISCbits.TRISC7 = 0;
    
    TRISAbits.TRISA7 = 0;
    TRISAbits.TRISA6 = 0;
    TRISCbits.TRISC2 = 0;
    TRISCbits.TRISC3 = 0;
    
    while (1) {
        
        delayms(led_delay);
        LEDBLINK(0,0,0,0,0,0,0,0);
        
        delayms(1);
        LEDBLINK(0,2,2,2,2,2,2,1);
        delayms(led_delay);
        LEDBLINK(0,2,2,2,2,2,1,2);
        delayms(led_delay);
        LEDBLINK(0,2,2,2,2,1,2,2);
        delayms(led_delay);
        LEDBLINK(0,2,2,2,1,2,2,2);
        delayms(led_delay);
        LEDBLINK(0,2,2,1,2,2,2,2);
        delayms(led_delay);
        LEDBLINK(0,2,1,2,2,2,2,2);
        delayms(led_delay);
        LEDBLINK(0,1,2,2,2,2,2,2);
        
        delayms(led_delay);
        LEDBLINK(2,0,2,2,2,2,2,1);
        delayms(led_delay);
        LEDBLINK(2,0,2,2,2,2,1,2);
        delayms(led_delay);
        LEDBLINK(2,0,2,2,2,1,2,2);
        delayms(led_delay);
        LEDBLINK(2,0,2,2,1,2,2,2);
        delayms(led_delay);
        LEDBLINK(2,0,2,1,2,2,2,2);
        delayms(led_delay);
        LEDBLINK(2,0,1,2,2,2,2,2);
        delayms(led_delay);
        LEDBLINK(1,0,2,2,2,2,2,2);
        delayms(led_delay);
        
        LEDBLINK(2,2,0,2,2,2,2,1);
        delayms(led_delay);
        LEDBLINK(2,2,0,2,2,2,1,2);
        delayms(led_delay);
        LEDBLINK(2,2,0,2,2,1,2,2);
        delayms(led_delay);
        LEDBLINK(2,2,0,2,1,2,2,2);
        delayms(led_delay);
        LEDBLINK(2,2,0,1,2,2,2,2);
        delayms(led_delay);
        LEDBLINK(2,1,0,2,2,2,2,2);
        delayms(led_delay);
        LEDBLINK(1,2,0,2,2,2,2,2);
        
        delayms(led_delay);
        LEDBLINK(2,2,2,0,2,2,2,1);
        delayms(led_delay);
        LEDBLINK(2,2,2,0,2,2,1,2);
        delayms(led_delay);
        LEDBLINK(2,2,2,0,2,1,2,2);
        delayms(led_delay);
        LEDBLINK(2,2,2,0,1,2,2,2);
        delayms(led_delay);
        LEDBLINK(2,2,1,0,2,2,2,2);
        delayms(led_delay);
        LEDBLINK(2,1,2,0,2,2,2,2);
        delayms(led_delay);
        LEDBLINK(1,2,2,0,2,2,2,2);
        
        delayms(led_delay);
        LEDBLINK(2,2,2,2,0,2,2,1);
        delayms(led_delay);
        LEDBLINK(2,2,2,2,0,2,1,2);
        delayms(led_delay);
        LEDBLINK(2,2,2,2,0,1,2,2);
        delayms(led_delay);
        LEDBLINK(2,2,2,1,0,2,2,2);
        delayms(led_delay);
        LEDBLINK(2,2,1,2,0,2,2,2);
        delayms(led_delay);
        LEDBLINK(2,1,2,2,0,2,2,2);
        delayms(led_delay);
        LEDBLINK(1,2,2,2,0,2,2,2);
        
        delayms(led_delay);
        LEDBLINK(2,2,2,2,2,0,2,1);
        delayms(led_delay);
        LEDBLINK(2,2,2,2,2,0,1,2);
        delayms(led_delay);
        LEDBLINK(2,2,2,2,1,0,2,2);
        delayms(led_delay);
        LEDBLINK(2,2,2,1,2,0,2,2);
        delayms(led_delay);
        LEDBLINK(2,2,1,2,2,0,2,2);
        delayms(led_delay);
        LEDBLINK(2,1,2,2,2,0,2,2);
        delayms(led_delay);
        LEDBLINK(2,1,2,2,2,0,2,2);
        delayms(led_delay);
        LEDBLINK(1,2,2,2,2,0,2,2);
        
        delayms(led_delay);
        LEDBLINK(2,2,2,2,2,2,0,1);
        delayms(led_delay);
        LEDBLINK(2,2,2,2,2,1,0,2);
        delayms(led_delay);
        LEDBLINK(2,2,2,2,1,2,0,2);
        delayms(led_delay);
        LEDBLINK(2,2,2,1,2,2,0,2);
        delayms(led_delay);
        LEDBLINK(2,2,1,2,2,2,0,2);
        delayms(led_delay);
        LEDBLINK(2,1,2,2,2,2,0,2);
        delayms(led_delay);
        LEDBLINK(1,2,2,2,2,2,0,2);
        delayms(led_delay);
        
        LEDBLINK(2,2,2,2,2,2,1,0);
        delayms(led_delay);
        LEDBLINK(2,2,2,2,2,1,2,0);
        delayms(led_delay);
        LEDBLINK(2,2,2,2,1,2,2,0);
        delayms(led_delay);
        LEDBLINK(2,2,2,1,2,2,2,0);
        delayms(led_delay);
        LEDBLINK(2,2,1,2,2,2,2,0);
        delayms(led_delay);
        LEDBLINK(2,1,2,2,2,2,2,0);
        delayms(led_delay);
        LEDBLINK(1,2,2,2,2,2,2,0);
        delayms(led_delay);
        
        /*
        delayms(led_delay);
        LEDBLINK(0,0,0,0);
        delayms(led_delay);
        LEDBLINK(0,0,1,2);
        delayms(led_delay);
        LEDBLINK(0,1,0,2);
        delayms(led_delay);
        LEDBLINK(0,0,2,1);
        delayms(led_delay);
        LEDBLINK(0,1,2,0);
        delayms(led_delay);
        LEDBLINK(0,2,0,1);
        delayms(led_delay);
        LEDBLINK(0,2,1,0);
        */
    }

    return (EXIT_SUCCESS);
}

