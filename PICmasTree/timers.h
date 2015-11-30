/* 
 * File:   timers.h
 * Author: Author: mhibbett. mike.hibbett@gmail.com, mjhdesigns.com
 *
 * Created on 25 February 2014, 14:01
 */

#ifndef TIMERS_H
#define	TIMERS_H

#ifdef	__cplusplus
extern "C" {
#endif

#define USLEEP_2US      65534
#define USLEEP_100US	65461
#define USLEEP_200US	65386
#define USLEEP_3MS	63286
#define USLEEP_8MS	59536
#define USLEEP_80MS	5536
#define USLEEP_1MS      64786

void timer0Init( void );
void timer1Init( void );
void usleep( unsigned short ticks);
void delayms(unsigned short ms);


#ifdef	__cplusplus
}
#endif

#endif	/* TIMERS_H */

