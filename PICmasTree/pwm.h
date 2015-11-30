/* 
 * File:   pwm.h
 * Author: Author: mhibbett. mike.hibbett@gmail.com, mjhdesigns.com
 *
 * Created on 07 January 2015, 23:48
 */

#ifndef PWM_H
#define	PWM_H

#ifdef	__cplusplus
extern "C" {
#endif

// These identify the PWM pins used by the PWM system API
#define PWMPIN1 0   // CCP4, PORTB4
#define PWMPIN2 1   // CCP5, PORTB5
#define PWMPIN3 2   // CCP6, PORTB6
#define PWMPIN4 3   // CCP7, PORTB7
#define PWMPIN5 4   // CCP9, PORTC6
#define PWMPIN6 5   // CCP10, PORTC7


void PWMInit(unsigned int pin);
void PWMDuty(unsigned int pin, int duty);
void PWMPeriod(unsigned int hertz);

#ifdef	__cplusplus
}
#endif

#endif	/* PWM_H */

