/* 
 * File:   LCD-Graphic-ST7735S.h
 * Author: mike
 *
 * Created on 15 February 2014, 09:35
 */

#ifndef LCD_GRAPHIC_ST7735S_H
#define	LCD_GRAPHIC_ST7735S_H

#ifdef	__cplusplus
extern "C" {
#endif

#define LCD_W 128
#define LCD_H 160


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


extern  unsigned short BACK_COLOR, POINT_COLOR, LINE_MASK;

/*
 * Function:    LCD_Clear
 *
 *              Clears the display to the specified colour
 */
void LCD_Clear(unsigned short Color);

/*
 * Function:    LCDST7735sInit
 *
 *              Initialises the port pins to which the LCD is connected
 *              configures the LCD into a state ready to recieve screen
 *              data.
 */
void LCDST7735sInit( void );
void LCD_DrawPoint(unsigned short x,unsigned short y);
void LCD_DrawLine(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2);
void LCD_ShowString(unsigned short x,unsigned short y,const unsigned char *p);
void LCD_ShowChar(unsigned short x,unsigned short y,unsigned char num,unsigned char mode);



#ifdef	__cplusplus
}
#endif

#endif	/* LCD_GRAPHIC_ST7735S_H */

