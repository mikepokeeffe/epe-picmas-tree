/*
 * File:   interrupts.c
 * Author: Author: mhibbett. mike.hibbett@gmail.com, mjhdesigns.com
 *
 * Created on 31 March 2014, 20:53
 *
 * Interrupt routines.
 */

#include <pic18f27j13.h>
#include "interrupts.h"
#include "lplc-adc.h"

#define TOGGLE_LED_TIME 1   // time between toggling the LED - 10s


unsigned char delay_2s;


/*************************************************************************
 * Function: InterruptInit
 * Initialises the interrupt sub-system.
 *************************************************************************
 */
void InterruptInit( void )
{
    delay_2s = TOGGLE_LED_TIME;
}


/*************************************************************************
 * Function: InterruptHandler
 * Provides a basic interrupt handler function.
 *************************************************************************
 */
void interrupt InterruptHandler( void )
{
    // Was it the timer?
    if (PIR1bits.TMR1IF) {
        PIR1bits.TMR1IF = 0;

        delay_2s--;

        if ( !delay_2s ) {
            delay_2s = TOGGLE_LED_TIME;
        }
    } else if (PIR1bits.ADIF) {
        PIR1bits.ADIF = 0;

        handle_int_adc();
    }
}