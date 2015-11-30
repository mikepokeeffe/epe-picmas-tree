/*
 * File:   hardware.c
 * Author: mhibbett. mike.hibbett@gmail.com, mjhdesigns.com
 *
 * Created on 31 March 2014, 20:53
 *
 * Low level hardware initialisation.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pic18f27j13.h>
#include "hardware.h"
#include "LCD-Graphic-ILI9341.h"
#include "timers.h"

/**************************************************************************
* Function: HardwareInit
*           Low level hardware initialisation.
**************************************************************************
*/
void HardwareInit( void )
{
    // Initially, set all I/O to digital rather than analogue

    ANCON0 = 0xFE;
    ANCON1 = 0x7F;

    // And set the LED pin as an output, LED Off, as it draws 11mA current
    LATBbits.LATB5 = 0;
    TRISBbits.TRISB5 = 0;
    
    //Buttons Setup
    //TRISBbits.TRISB0 = 1;
    //TRISBbits.TRISB1 = 1;
    //TRISCbits.TRISC6 = 1;
    //TRISCbits.TRISC7 = 1;
}

void LEDBLINK (unsigned int port1, unsigned int port2, unsigned int port3, unsigned int port4, unsigned int port5, unsigned int port6, unsigned int port7, unsigned int port8) {
    /*
     *  This function checks if what each value is and sets LED accordingly 
     *  If port is 0, then turn port off
     *  If port is 1, then turn port on
     *  If port is 2, then turn port as an input (High Impedance)
    */
    TRISCbits.TRISC4 = 0;
    TRISCbits.TRISC5 = 0;
    TRISCbits.TRISC6 = 0;
    TRISCbits.TRISC7 = 0;
    
    TRISAbits.TRISA7 = 0;
    TRISAbits.TRISA6 = 0;
    TRISCbits.TRISC2 = 0;
    TRISCbits.TRISC3 = 0;
    
    LATBbits.LATB5 ^= 1;
    
    if(port1 == 0) {
        TRISCbits.TRISC4 = 0;
        LATCbits.LATC4 = 0;
    } else if (port1 == 1) {
        TRISCbits.TRISC4 = 0;
        LATCbits.LATC4 = 1;
    } else if (port1 == 2) {
        TRISCbits.TRISC4 = 1;
    }
    
    if(port2 == 0) {
        TRISCbits.TRISC5 = 0;
        LATCbits.LATC5 = 0;
    } else if (port2 == 1) {
        TRISCbits.TRISC5 = 0;
        LATCbits.LATC5 = 1;
    } else if (port2 == 2) {
        TRISCbits.TRISC5 = 1;
    }
    
    if(port3 == 0) {
        TRISCbits.TRISC6 = 0;
        LATCbits.LATC6 = 0;
    } else if (port3 == 1) {
        TRISCbits.TRISC6 = 0;
        LATCbits.LATC6 = 1;
    } else if (port3 == 2) {
        TRISCbits.TRISC6 = 1;
    }
    
    if(port4 == 0) {
        TRISCbits.TRISC7 = 0;
        LATCbits.LATC7 = 0;
    } else if (port4 == 1) {
        TRISCbits.TRISC7 = 0;
        LATCbits.LATC7 = 1;
    } else if (port4 == 2) {
        TRISCbits.TRISC7 = 1;
    }
    
    if(port5 == 0) {
        TRISAbits.TRISA7 = 0;
        LATAbits.LATA7 = 0;
    } else if (port5 == 1) {
        TRISAbits.TRISA7 = 0;
        LATAbits.LATA7 = 1;
    } else if (port5 == 2) {
        TRISAbits.TRISA7 = 1;
    }
    
    if(port6 == 0) {
        TRISAbits.TRISA6 = 0;
        LATAbits.LATA6 = 0;
    } else if (port6 == 1) {
        TRISAbits.TRISA6 = 0;
        LATAbits.LATA6 = 1;
    } else if (port6 == 2) {
        TRISAbits.TRISA6 = 1;
    }
    
    if(port7 == 0) {
        TRISCbits.TRISC2 = 0;
        LATCbits.LATC2 = 0;
    } else if (port7 == 1) {
        TRISCbits.TRISC2 = 0;
        LATCbits.LATC2 = 1;
    } else if (port7 == 2) {
        TRISCbits.TRISC2 = 1;
    }
    
    if(port8 == 0) {
        TRISCbits.TRISC3 = 0;
        LATCbits.LATC3 = 0;
    } else if (port8 == 1) {
        TRISCbits.TRISC3 = 0;
        LATCbits.LATC3 = 1;
    } else if (port8 == 2) {
        TRISCbits.TRISC3 = 1;
    }
    
}