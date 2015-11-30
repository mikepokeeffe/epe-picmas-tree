/* 
 * File:   LCD_GRAPHIC_ILI9341_H
 * Author: Mike Hibbett & Gary Bleads
 *
 * Created on 15 February 2014, 09:35
 */

#ifndef LCD_GRAPHIC_ILI9341_H
#define	LCD_GRAPHIC_ILI9341_H

#ifdef	__cplusplus
extern "C" {
#endif

#define LCD_W 240
#define LCD_H 320



#define WHITE       0xFFFF
#define BLACK       0x0000
#define BLUE        0x001F
#define BRED        0XF81F
#define GRED        0XFFE0
#define GBLUE       0X07FF
#define RED         0xF800
#define MAGENTA     0xF81F
#define GREEN       0x07E0
#define CYAN        0x7FFF
#define YELLOW      0xFFE0
#define BROWN       0XBC40
#define BRRED       0XFC07
#define GRAY        0X8430
#define DARKBLUE    0X01CF
#define LIGHTBLUE   0X7D7C
#define GRAYBLUE    0X5458
#define LIGHTGREEN  0X841F
#define LGRAY       0XC618
#define LGRAYBLUE   0XA651
#define LBBLUE      0X2B12


#define ILI9341_TFTWIDTH  240
#define ILI9341_TFTHEIGHT 320


#define ILI9341_NOP     0x00
#define ILI9341_SWRESET 0x01
#define ILI9341_RDDID   0x04
#define ILI9341_RDDST   0x09


#define ILI9341_SLPIN   0x10
#define ILI9341_SLPOUT  0x11
#define ILI9341_PTLON   0x12
#define ILI9341_NORON   0x13


#define ILI9341_RDMODE  0x0A
#define ILI9341_RDMADCTL  0x0B
#define ILI9341_RDPIXFMT  0x0C
#define ILI9341_RDIMGFMT  0x0A
#define ILI9341_RDSELFDIAG  0x0F


#define ILI9341_INVOFF  0x20
#define ILI9341_INVON   0x21
#define ILI9341_GAMMASET 0x26
#define ILI9341_DISPOFF 0x28
#define ILI9341_DISPON  0x29


#define ILI9341_CASET   0x2A
#define ILI9341_PASET   0x2B
#define ILI9341_RAMWR   0x2C
#define ILI9341_RAMRD   0x2E


#define ILI9341_PTLAR   0x30
#define ILI9341_MADCTL  0x36
#define ILI9341_PIXFMT  0x3A


#define ILI9341_FRMCTR1 0xB1
#define ILI9341_FRMCTR2 0xB2
#define ILI9341_FRMCTR3 0xB3
#define ILI9341_INVCTR  0xB4
#define ILI9341_DFUNCTR 0xB6

#define ILI9341_PWCTR1  0xC0
#define ILI9341_PWCTR2  0xC1
#define ILI9341_PWCTR3  0xC2
#define ILI9341_PWCTR4  0xC3
#define ILI9341_PWCTR5  0xC4
#define ILI9341_VMCTR1  0xC5
#define ILI9341_VMCTR2  0xC7

#define ILI9341_RDID1   0xDA
#define ILI9341_RDID2   0xDB
#define ILI9341_RDID3   0xDC
#define ILI9341_RDID4   0xDD

#define ILI9341_GMCTRP1 0xE0
#define ILI9341_GMCTRN1 0xE1

extern  unsigned short BACK_COLOR, POINT_COLOR, LINE_MASK;

// GLOBAL VARIABLES
extern unsigned short INFINITE_PERSISTENCE;     // 1 = Infinite Persistence, 0 = Normal Hold Off
extern unsigned short HOLD_OFF;                 // Hold Off Time
extern unsigned short AUTO_SAMPLE;              // Auto Sample when 0, Normal Sample when 1
extern unsigned short EDGE_SAMPLE;              // 0 = Falling Edge, 1 = Rising Edge
extern unsigned short RISING_EDGE;              // Rising Edge = 1, Rising Edge has been seen
extern unsigned short FALLING_EDGE;             // Falling Edge = 1, Falling Edge has been seen
extern unsigned short TIMEBASE;                 // To be changed to some value
extern unsigned char TRIGGER_LINE;              // This is the Trigger Level Line
extern unsigned char MEASURE_LINE;              // This is the Trigger Level Line
extern unsigned char HOLD_SAMPLING;             // 0 = Keep Capturing, 1 = Stop Capturing
extern unsigned int BUTTON_SELECT;              // Button Select 0 to 3


/*
 * Function:    LCD_Clear
 *
 *              Clears the display to the specified colour
 */
void LCD_Clear(unsigned short Color);

/*
 * Function:    LCDILI9341Init
 *
 *              Initialises the port pins to which the LCD is connected
 *              configures the LCD into a state ready to recieve screen
 *              data.
 */
void LCDILI9341Init( void );
void LCD_DrawPoint(unsigned short x,unsigned short y);
void LCD_DrawLine(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2);
void LCD_DrawRectangle(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2);
void LCD_ShowString(unsigned short x,unsigned short y,const unsigned char *p);
void LCD_ShowChar(unsigned short x,unsigned short y,unsigned char num,unsigned char mode);
void LCD_DrawScreenWords(unsigned int button_sel);

#ifdef	__cplusplus
}
#endif

#endif	/* LCD_GRAPHIC_ILI9341_H */

