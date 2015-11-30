/*
 * File:   timers.c
 * Author: mhibbett. mike.hibbett@gmail.com, mjhdesigns.com
 *
 * Created on 25 February 2014, 13:47
 *
 * Low level timer configuration and initialisation; delay routines
 * for general use.
 */

#include <pic18.h>
#include "timers.h"

// TMR0 configuration for our usleep delay routine
// The configuration is so simple we do not need an
// initialisation routine
#define TMR0_DISABLE    0x00
#define TMR0_ENABLE     0x83

// timer is 16bit, TMR1 SOC source, prescale by 1 (None)
#define TMR1_CONF       0x8E


/**************************************************************************
* Function: timer0Init
*           Initialises timer0, ensuring it is turned off
**************************************************************************
*/
void timer0Init( void )
{
    T0CON = TMR0_DISABLE;
}


/**************************************************************************
 * Function: timer1Init
 *           Enables Timer1 based on the attached 32KHz oscillator.
 *           This timer setup is special; it is accurate ( being based off
 *           a crystal ) and it is used to periodically wake the processor
 *           from sleep mode ( if the sleep instruction is used )
 **************************************************************************
 */
void timer1Init( void )
{
    PIR1bits.TMR1IF = 0;
    TMR1H = 0;
    TMR1L = 0;

    // Enable the 32KHz oscillator on Timer1
    // for use as an accurate timebase.

    T1CON = TMR1_CONF;
    PIE1bits.TMR1IE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    T1CONbits.TMR1ON = 1;
}



void delayms(unsigned short ms)
{
    while (ms--)
        usleep(USLEEP_1MS);
}


/**************************************************************************
 * Function: usleep
 *           delays for a number of microseconds.
 *          The delay value to use must be pre-computed; see the
 *          header file for examples.
 *          Uses TMR0
 **************************************************************************
*/
void usleep(unsigned short ticks)
{
    TMR0H = ticks >> 8;
    TMR0L = ticks & 0xFF;

    INTCONbits.TMR0IF = 0;
    T0CON = TMR0_ENABLE;

    while ( !INTCONbits.TMR0IF )
        /* do nothing */ ;

    T0CON = TMR0_DISABLE;
}
