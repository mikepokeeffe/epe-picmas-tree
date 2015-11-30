/* 
 * File:   hardware.h
 * Author: Author: mhibbett. mike.hibbett@gmail.com, mjhdesigns.com
 *
 * Created on 25 February 2014, 14:20
 */

#ifndef HARDWARE_H
#define	HARDWARE_H

#ifdef	__cplusplus
extern "C" {
#endif

#define CPUHZ   48000000UL  // Speed at which the CPU is clocked

void HardwareInit( void );
//void CheckforButtonPress(void);
void LEDBLINK (unsigned int port1, unsigned int port2, unsigned int port3, unsigned int port4, unsigned int port5, unsigned int port6, unsigned int port7, unsigned int port8);
  

#ifdef	__cplusplus
}
#endif

#endif	/* HARDWARE_H */

