/* 
 * File:   lcd.h
 * Author: mhibbett. mike.hibbett@gmail.com, mjhdesigns.com
 *
 * Created on 25 February 2014, 14:20
 */

#ifndef LCD_H
#define	LCD_H

#ifdef	__cplusplus
extern "C" {
#endif

#define LCD_LINE1   0
#define LCD_LINE2   0x40

void LCDInit(void);
void LCDWrite(unsigned char ch);
void setPos(unsigned char LCDLine, unsigned char LCDCol);


#ifdef	__cplusplus
}
#endif

#endif	/* LCD_H */

