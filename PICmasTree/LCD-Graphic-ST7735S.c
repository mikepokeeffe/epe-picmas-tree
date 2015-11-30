/*
 * File:    LCD-Graphic-ST7735S.c
 *
 *          This is a driver for the Truley 128 x 160 colour LCD
 *          This is available on Ebay very cheaply; less than £4 inc delivery!
 *
 * Author: mike
 *
 * Created on 12 February 2014, 21:49
 */

#include <pic18f27j13.h>
#include <xc.h>

#include "timers.h"
#include "LCD-Graphic-ST7735S.h"
#include "LCD-Graphic-Font.h"

unsigned short BACK_COLOR, POINT_COLOR, LINE_MASK;

#define LCD_CS      LATAbits.LA7
#define LCD_AO      LATCbits.LC2
#define LCD_SDA     LATCbits.LC5
#define LCD_SCK     LATCbits.LC3
#define LCD_REST    LATAbits.LA6

unsigned long lp;


void LCD_Writ_Bus(unsigned char da)
{
    LCD_SDA=(da & 0x80) ? 1 : 0; LCD_SCK = 0; LCD_SCK = 1; 
    LCD_SDA=(da & 0x40) ? 1 : 0; LCD_SCK = 0; LCD_SCK = 1; 
    LCD_SDA=(da & 0x20) ? 1 : 0; LCD_SCK = 0; LCD_SCK = 1; 
    LCD_SDA=(da & 0x10) ? 1 : 0; LCD_SCK = 0; LCD_SCK = 1; 
    LCD_SDA=(da & 0x08) ? 1 : 0; LCD_SCK = 0; LCD_SCK = 1; 
    LCD_SDA=(da & 0x04) ? 1 : 0; LCD_SCK = 0; LCD_SCK = 1; 
    LCD_SDA=(da & 0x02) ? 1 : 0; LCD_SCK = 0; LCD_SCK = 1;
    LCD_SDA=(da & 0x01) ? 1 : 0; LCD_SCK = 0; LCD_SCK = 1;
}


void LCD_WR_DATA8(unsigned char da)
{
    LCD_AO=1;
    LCD_Writ_Bus(da);
}


 void LCD_WR_DATA(unsigned int da)
{
    LCD_AO=1;
    LCD_Writ_Bus(da>>8);
    LCD_Writ_Bus(da);
}


void LCD_WR_REG(unsigned char da)
{
    LCD_AO=0;
    LCD_Writ_Bus(da);
}


void LCD_WR_REG_DATA(unsigned int reg, unsigned int da)
{
    LCD_WR_REG(reg);
    LCD_WR_DATA(da);
}


void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{
   LCD_WR_REG(0x2a);
   LCD_WR_DATA8(x1>>8);
   LCD_WR_DATA8(x1);
   LCD_WR_DATA8(x2>>8);
   LCD_WR_DATA8(x2);

   LCD_WR_REG(0x2b);
   LCD_WR_DATA8(y1>>8);
   LCD_WR_DATA8(y1);
   LCD_WR_DATA8(y2>>8);
   LCD_WR_DATA8(y2);

   LCD_WR_REG(0x2C);
}


void LCD_Clear(unsigned short Color)
{
    unsigned char VH,VL;
    unsigned short i,j;
    
    VH = Color>>8;
    VL = Color;
    
    Address_set(0,0,LCD_W-1,LCD_H-1);

    for (i=0;i<LCD_W;i++) {
        for (j=0;j<LCD_H;j++) {
            LCD_WR_DATA8(VH);
            LCD_WR_DATA8(VL);
        }
    }
}


void LCD_DrawPoint(unsigned short x,unsigned short y)
{
    Address_set(x,y,x,y);
    LCD_WR_DATA(POINT_COLOR);
}


//  (xend-xsta)*(yend-ysta)
void LCD_Fill(unsigned short xsta,unsigned short ysta,unsigned short xend,unsigned short yend,unsigned short color)
{
    unsigned short i,j;
    Address_set(xsta,ysta,xend,yend);

    for (i=ysta;i<=yend;i++){
        for (j=xsta;j<=xend;j++)
            LCD_WR_DATA(color);
    }
}


void LCD_DrawPoint_big(unsigned short x,unsigned short y)
{
    LCD_Fill(x-1,y-1,x+1,y+1,POINT_COLOR);
}


void LCD_DrawLine(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2)
{
    unsigned short t;
    int xerr=0,yerr=0,delta_x,delta_y,distance;
    int incx,incy,uRow,uCol;
    unsigned char lineMask = 0x80;

    delta_x=x2-x1;
    delta_y=y2-y1;
    uRow=x1;
    uCol=y1;

    if(delta_x>0)
        incx = 1;
    else if (delta_x==0)
        incx=0;
    else {
        incx=-1;
        delta_x=-delta_x;
    }
    
    if (delta_y>0)
        incy=1;
    else if (delta_y==0)
        incy=0;
    else {
        incy=-1;delta_y=-delta_y;
    }
    
    if ( delta_x>delta_y)
        distance = delta_x;
    else
        distance = delta_y;

    for (t=0;t<=distance+1;t++ ) {
        if (LINE_MASK & lineMask)
            LCD_DrawPoint(uRow,uCol);
        else {
            Address_set(uRow,uCol,uRow,uCol);
            LCD_WR_DATA(BACK_COLOR);
        }
        lineMask >>= 1;
        if (!lineMask) lineMask = 0x80;

        xerr+=delta_x ;
        yerr+=delta_y ;

        if (xerr>distance) {
            xerr-=distance;
            uRow+=incx;
	}

        if (yerr>distance) {
            yerr-=distance;
            uCol+=incy;
	}
    }
}


void LCD_DrawRectangle(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2)
{
    LCD_DrawLine(x1,y1,x2,y1);
    LCD_DrawLine(x1,y1,x1,y2);
    LCD_DrawLine(x1,y2,x2,y2);
    LCD_DrawLine(x2,y1,x2,y2);
}


void Draw_Circle(unsigned short x0,unsigned short y0,unsigned char r)
{
    int a,b;
    int di;

    a=0;b=r;
    di= 3-(r<<1);
	
    while (a<=b) {
        LCD_DrawPoint(x0-b,y0-a);             //3
        LCD_DrawPoint(x0+b,y0-a);             //0
        LCD_DrawPoint(x0-a,y0+b);             //1
        LCD_DrawPoint(x0-b,y0-a);             //7
        LCD_DrawPoint(x0-a,y0-b);             //2
        LCD_DrawPoint(x0+b,y0+a);             //4
        LCD_DrawPoint(x0+a,y0-b);             //5
        LCD_DrawPoint(x0+a,y0+b);             //6
        LCD_DrawPoint(x0-b,y0+a);
        a++;

        if (di<0)
            di += 4*a+6;
        else {
            di+=10+4*(a-b);
            b--;
	}

        LCD_DrawPoint(x0+a,y0+b);
    }
}


void LCD_ShowChar(unsigned short x,unsigned short y,unsigned char num,unsigned char mode)
{
    unsigned char temp;
    unsigned char pos,t;
    unsigned short x0=x;
    unsigned short colortemp=POINT_COLOR;

    if (x>LCD_W-8 || y>LCD_H-5)
        return;

    num=num-' ';
    Address_set(x,y,x+8-1,y+5-1);

    if (!mode) {
        for(pos=0;pos<5;pos++) {
            temp=Font5x7[(unsigned short)num*5+pos];

            for (t=0;t<8;t++) {
                if (temp&0x80)
                    POINT_COLOR = colortemp;
		else
                    POINT_COLOR = BACK_COLOR;

		LCD_WR_DATA(POINT_COLOR);
		temp<<=1;
	 	x++;
	    }

            x=x0;
            y++;
        }
    } else {
        for(pos=0;pos<16;pos++) {
            temp = Font5x7[(unsigned short)num*5+pos];

            for (t=0;t<8;t++) {

                if (temp&0x80)
                    LCD_DrawPoint(x+t,y+pos);
                temp<<=1;
            }
        }
    }

    POINT_COLOR=colortemp;
}


void LCD_ShowString(unsigned short x,unsigned short y,const unsigned char *p)
{
    while(*p!='\0')
    {
        if(x > LCD_W){x=0;y=0;}
        if(y>LCD_H-6){y=0;x -= 8;}
        LCD_ShowChar(x,y,*p,0);
        y+=6;
        p++;
    }
}


void Lcd_Init(void)
{

    LCD_CS =1;

    LCD_REST=1;
    delayms(5);
    LCD_REST=0;
    delayms(5);
    LCD_REST=1;
    LCD_CS=1;
    delayms(5);
    LCD_CS =0;

    LCD_WR_REG(0x11); 
    delayms(120); 
    //------------------------------------ST7735S Frame Rate-----------------------------------------//
    LCD_WR_REG(0xB1);
    LCD_WR_DATA8(0x05);
    LCD_WR_DATA8(0x3C);
    LCD_WR_DATA8(0x3C);
    LCD_WR_REG(0xB2);
    LCD_WR_DATA8(0x05);
    LCD_WR_DATA8(0x3C);
    LCD_WR_DATA8(0x3C);
    LCD_WR_REG(0xB3);
    LCD_WR_DATA8(0x05);
    LCD_WR_DATA8(0x3C);
    LCD_WR_DATA8(0x3C);
    LCD_WR_DATA8(0x05);
    LCD_WR_DATA8(0x3C);
    LCD_WR_DATA8(0x3C);
    //------------------------------------End ST7735S Frame Rate-----------------------------------------//
    LCD_WR_REG(0xB4); //Dot inversion
    LCD_WR_DATA8(0x03);
    LCD_WR_REG(0xC0);
    LCD_WR_DATA8(0x28);
    LCD_WR_DATA8(0x08);
    LCD_WR_DATA8(0x04);
    LCD_WR_REG(0xC1);
    LCD_WR_DATA8(0XC0);
    LCD_WR_REG(0xC2);
    LCD_WR_DATA8(0x0D);
    LCD_WR_DATA8(0x00);
    LCD_WR_REG(0xC3);
    LCD_WR_DATA8(0x8D);
    LCD_WR_DATA8(0x2A);
    LCD_WR_REG(0xC4);
    LCD_WR_DATA8(0x8D);
    LCD_WR_DATA8(0xEE);
    //---------------------------------End ST7735S Power Sequence-------------------------------------//
    LCD_WR_REG(0xC5); //VCOM
    LCD_WR_DATA8(0x1A);
    LCD_WR_REG(0x36); //MX, MY, RGB mode
    LCD_WR_DATA8(0xC0);
    //------------------------------------ST7735S Gamma Sequence-----------------------------------------//
    LCD_WR_REG(0xE0);
    LCD_WR_DATA8(0x04);
    LCD_WR_DATA8(0x22);
    LCD_WR_DATA8(0x07);
    LCD_WR_DATA8(0x0A);
    LCD_WR_DATA8(0x2E);
    LCD_WR_DATA8(0x30);
    LCD_WR_DATA8(0x25);
    LCD_WR_DATA8(0x2A);
    LCD_WR_DATA8(0x28);
    LCD_WR_DATA8(0x26);
    LCD_WR_DATA8(0x2E);
    LCD_WR_DATA8(0x3A);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x01);
    LCD_WR_DATA8(0x03);
    LCD_WR_DATA8(0x13);
    LCD_WR_REG(0xE1);
    LCD_WR_DATA8(0x04);
    LCD_WR_DATA8(0x16);
    LCD_WR_DATA8(0x06);
    LCD_WR_DATA8(0x0D);
    LCD_WR_DATA8(0x2D);
    LCD_WR_DATA8(0x26);
    LCD_WR_DATA8(0x23);
    LCD_WR_DATA8(0x27);
    LCD_WR_DATA8(0x27);
    LCD_WR_DATA8(0x25);
    LCD_WR_DATA8(0x2D);
    LCD_WR_DATA8(0x3B);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x01);
    LCD_WR_DATA8(0x04);
    LCD_WR_DATA8(0x13);
    //------------------------------------End ST7735S Gamma Sequence-----------------------------------------//
    LCD_WR_REG(0x3A); //65k mode
    LCD_WR_DATA8(0x05);
    LCD_WR_REG(0x29); //Display on
}


/*
 * Function:    LCDST7735sInit
 *
 *              Initialises the port pins to which the LCD is connected
 *              configures the LCD into a state ready to recieve screen
 *              data.
 */

void LCDST7735sInit( void )
{
    // Set our pins as outputs
    ANCON1 |= 0x08;
    TRISC &= ~0x2C;
    TRISA &= ~0xC0;

    BACK_COLOR=BLACK;
    POINT_COLOR=WHITE;
    LINE_MASK = 0xFF;
    
    Lcd_Init();
}
