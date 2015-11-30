/*
 * File:    pwm.c
 *
 *          Provides the PWM interface, for up to 6 pwm pins on the LPLC
 *          We use the TMR2 module to provide the 10bit PWM timer.
 *          We only use standard mode.
 *
 * Author: mike.hibbett@gmail.com, mjhdesigns.com
 *
 * Created on 07 January 2015, 23:48
 */

#include <pic18f27j13.h>
#include "hardware.h"
#include "pwm.h"

/*************************************************************************
 * Function: PWMPeriod
 * Configures the frequency at which all PWM pins will run at.
 * This uses Timer 2.
 *************************************************************************
 */
void PWMPeriod(unsigned int hertz)
{
    // Set pre-scaler to divide by 4
    // And turn on the timer
    T2CON &= 0xFC;
    T2CON |= 0x05;

    // Set PR2 to the desired count.
    PR2 = ((CPUHZ / 16) / hertz);
}


/*************************************************************************
 * Function: PWMDuty
 * Access the PR2 register to determine the current period,
 * then sets the duty timer to the appropriate percentage, 0..100
 * For pin values, see the pwm.h file
 *************************************************************************
 */
void PWMDuty(unsigned int pin, int duty)
{
    unsigned int dutyVal;
    unsigned int dh, dl;

    if (duty > 0) {
        if (duty > 99)
            dutyVal = PR2 * 4;
        else
            dutyVal = (PR2 * 4 * duty) / 100;
    } else
        dutyVal = 0;

    dh = dutyVal >> 2;
    dl = (dutyVal & 0x03) << 4;

    // Now write the duty value
    switch (pin) {
        case PWMPIN1:
            CCPR4L = dh;
            CCP4CON &= 0xCF;
            CCP4CON |= dl;
        break;

        case PWMPIN2:
            CCPR5L = dh;
            CCP5CON &= 0xCF;
            CCP5CON |= dl;
        break;

        case PWMPIN3:
            CCPR6L = dh;
            CCP6CON &= 0xCF;
            CCP6CON |= dl;
        break;

        case PWMPIN4:
            CCPR7L = dh;
            CCP7CON &= 0xCF;
            CCP7CON |= dl;
        break;

        case PWMPIN5:
            CCPR9L = dh;
            CCP9CON &= 0xCF;
            CCP9CON |= dl;
        break;

        case PWMPIN6:
            CCPR10L = dh;
            CCP10CON &= 0xCF;
            CCP10CON |= dl;
        break;
    }
}


/*************************************************************************
 * Function: PWMInit
 * Enables PWM output on the specified pin
 * For pin values, see the pwm.h file
 *************************************************************************
 */
void PWMInit(unsigned int pin)
{
    /*
     The sequence is:
     Make pin an output
     conf CCPx module for PWM operation ( OR in 0x0F )
     CCPTMRS1/2: CCP4-10 TIMER SELECT 1 REGISTER
     */
    switch (pin) {
        case PWMPIN1:
            TRISBbits.TRISB4 = 0;
            CCP4CON |= 0x0F;
            CCPTMRS1 &= 0xFC;
        break;

        case PWMPIN2:
            TRISBbits.TRISB5 = 0;
            CCP5CON |= 0x0F;
            CCPTMRS1 &= 0xFB;
        break;

        case PWMPIN3:
            TRISBbits.TRISB6 = 0;
            CCP6CON |= 0x0F;
            CCPTMRS1 &= 0xEF;
        break;

        case PWMPIN4:
            TRISBbits.TRISB7 = 0;
            CCP7CON |= 0x0F;
            CCPTMRS1 &= 0x3F;
        break;

        case PWMPIN5:
            TRISCbits.TRISC6 = 0;
            CCP9CON |= 0x0F;
            CCPTMRS2 &= 0xFB;
        break;

        case PWMPIN6:
            TRISCbits.TRISC7 = 0;
            CCP10CON |= 0x0F;
            CCPTMRS2 &= 0xEF;
        break;
    }
}
