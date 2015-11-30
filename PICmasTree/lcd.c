/*
 * File:   lcd.c
 * Author: Author: mhibbett. mike.hibbett@gmail.com, mjhdesigns.com
 *
 * Created on 31 March 2014, 20:53
 *
 * Contains the code to handle an attached LCD module based on the
 * 44780 driver, used in 4-bit mode.
 */

#include <pic18.h>
#include "timers.h"

#define RS_LAT_BIT      LATBbits.LB2
#define RW_LAT_BIT      LATBbits.LB1
#define E_LAT_BIT       LATBbits.LB0
#define LCD_DATA_LAT    LATC


/*************************************************************************
 * Function: writeNibble.
 *           Writes a nibble to the LCD ( we use the LCD in 4-bit mode )
 *           We make use of the fact that the LATC upper four bits are
 *           always 0 on entry.
 *************************************************************************
 */
void writeNibble(unsigned char nibbleData)
{
    unsigned char wnTmp;    // Temporary variable used by writeNibble
    
    RW_LAT_BIT = (nibbleData & 0x10) ? 1 : 0; // bit 4 is the state of the RW line
    RS_LAT_BIT = (nibbleData & 0x20) ? 1 : 0; // bit 5 is the state of the RS line

    wnTmp = nibbleData << 4; // Move our bit to the upper nibble

    // now copy the data over to the upper four bits of PORTC, preserving the
    // lower parts of PORTB
    LCD_DATA_LAT = LCD_DATA_LAT | wnTmp;

    usleep(USLEEP_2US);
    E_LAT_BIT = 1;
    usleep(USLEEP_2US);
    E_LAT_BIT = 0;

    // Drop all the LCD lines to the low level
    RW_LAT_BIT = 0;
    RS_LAT_BIT = 0;
    LCD_DATA_LAT &= 0x0F;
}


/*************************************************************************
 * Function: setPos
 *           Sets the position on the display.
 *           Expects the variables LCDLine and LCDCol to have been set.
 *           These take the values LCD_LINE1,LCD_LINE2 and 0..15 respectively
 *************************************************************************
 */
void setPos(unsigned char LCDLine, unsigned char LCDCol)
{
    unsigned char index;

    index = LCDLine + LCDCol;

    writeNibble((index >> 4) | 0x08);
    writeNibble(index & 0x0F);
    usleep(USLEEP_100US);
}


/*************************************************************************
 * Function: LCDWrite
 *           Writes a character in W to the LCD at the current position
 *           Current position then updated.
 *************************************************************************
 */
void LCDWrite(unsigned char ch)
{
    writeNibble((ch >> 4) | 0x20);
    writeNibble((ch & 0x0F) | 0x20);
    usleep(USLEEP_100US);
}


/*************************************************************************
 * Function: LCDInit
 *           Setup the LCD to its default state following power up
 *************************************************************************
 */
void LCDInit(void)
{
    TRISC = 0x0F;   // Set upper 4 bits of PORTC as an output
    TRISB = 0xD8;   // Set lower 3 bits of PORTB as an outputTRIS

    // Set our LCD drive pins to digital I/O pins
    ANCON0 = 0x07;
    ANCON1 = 0x17;

    // Set all LCD signals low, and turn LED off
    LATB = 0;
    LATC = 0;

    // allow LCD time to start up
    usleep(USLEEP_80MS);
    writeNibble(0x03);
    usleep(USLEEP_8MS);
    writeNibble(0x03);
    usleep(USLEEP_200US);
    writeNibble(0x03);
    usleep(USLEEP_200US);
    writeNibble(0x02);
    usleep(USLEEP_200US);
    writeNibble(0x02);
    writeNibble(0x0c);
    usleep(USLEEP_3MS);
    writeNibble(0x00);
    writeNibble(0x0c);
    usleep(USLEEP_3MS);
    writeNibble(0x00);
    writeNibble(0x01);
    usleep(USLEEP_3MS);
    writeNibble(0x00);
    writeNibble(0x06);
    usleep(USLEEP_3MS);
}



