/* 
 * File:   config.h
 * Author: Author: mhibbett. mike.hibbett@gmail.com, mjhdesigns.com
 *
 * Created on 25 February 2014, 14:20
 */

#ifndef CONFIG_H
#define	CONFIG_H

#ifdef	__cplusplus
extern "C" {
#endif

#pragma config WDTEN = OFF
#pragma config XINST = OFF
#pragma config IESO = OFF
#pragma config FCMEN = OFF
#pragma config DSWDTEN = OFF
#pragma config OSC = INTOSCPLL
#pragma config CFGPLLEN = ON
#pragma config PLLDIV = 2
#pragma config PLLSEL = PLL96
#pragma config CP0 = OFF
#pragma config WDTPS = 1

#ifdef	__cplusplus
}
#endif

#endif	/* CONFIG_H */

