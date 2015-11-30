/*
 * File:    LCD-Graphic-ILI9341.c
 *
 *          This is a driver for the ILI9341 240x320 colour LCD
 *          This is available on Ebay very cheaply; less than £4 inc delivery!
 *
 * Author:  Mike Hibbett & Gary Bleads
 *
 * Created on 12 February 2014, 21:49
 */

#include <stdio.h>
#include <stdlib.h>
#include <pic18f27j13.h>
#include <xc.h>

#include "timers.h"
#include "LCD-Graphic-ILI9341.h"
#include "LCD-Graphic-Font.h"

unsigned short BACK_COLOR, POINT_COLOR, LINE_MASK;

unsigned int trigger_dir = 0;

unsigned short INFINITE_PERSISTENCE = 0;        // 1 = Infinite Persistence, 0 = Normal Hold Off
unsigned short HOLD_OFF = 0;                    // Hold Off Time
unsigned short AUTO_SAMPLE = 0;                 // Auto Sample when 0, Normal Sample when 1 (Capture on Trigger)
unsigned short EDGE_SAMPLE = 0;                 // 0 = Falling Edge, 1 = Rising Edge
unsigned short RISING_EDGE = 0;                 // Rising Edge = 1, Rising Edge has been seen
unsigned short FALLING_EDGE = 0;                // Falling Edge = 1, Falling Edge has been seen
unsigned short TIMEBASE = 0;                    // To be changed to some value
unsigned char TRIGGER_LINE = 100;               // This is the Trigger Level Line
unsigned char MEASURE_LINE = 64;                // This is the Trigger Level Line
unsigned char HOLD_SAMPLING = 0;                // 0 = Keep Capturing, 1 = Stop Capturing
unsigned int BUTTON_SELECT = 0;                 // Button Select, 0 = Auto/Norm
                                                // 1 = Trigger Edge
                                                // 2 = Red Trigger Level
                                                // 3 = Blue Trigger Level

#define LCD_CS      LATAbits.LA7
#define LCD_AO      LATCbits.LC2
#define LCD_SDA     LATCbits.LC5
#define LCD_SCK     LATCbits.LC3
#define LCD_REST    LATAbits.LA6

void LCD_Writ_Bus(unsigned char da)
{
    SSP1BUF;                    // Clear the BF flag
    SSP1BUF = da;               // write the data
    while (!SSP1STATbits.BF);   // Wait for the tx to complete
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

    // Configure the SPI peripheral.
    // 12MHz SPI clock
    SSP1STAT = 0x40; 
    SSP1CON1 = 0x20;

    LCD_CS =1;

    LCD_REST=1;
    delayms(5);
    LCD_REST=0;
    delayms(5);
    LCD_REST=1;
    LCD_CS=1;
    delayms(5);
    LCD_CS =0;

    LCD_WR_REG(0xE8); //
    LCD_WR_DATA8(0x85); // 85
    LCD_WR_DATA8(0x01); // 10
    LCD_WR_DATA8(0x79); //7a

    LCD_WR_REG(0xCB); // all default values anyway
    LCD_WR_DATA8(0x39);
    LCD_WR_DATA8(0x2C);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x34);
    LCD_WR_DATA8(0x02);

    LCD_WR_REG(0xF7); // maybe not required
    LCD_WR_DATA8(0x20);

    LCD_WR_REG(0x3A);
    LCD_WR_DATA8(0x55);

    LCD_WR_REG(0xEA);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x00);

    LCD_WR_REG(0xC0); // maybe not required
    LCD_WR_DATA8(0x1d); //  21

    LCD_WR_REG(0xC1);
    LCD_WR_DATA8(0x11);

    LCD_WR_REG(0xC5);
    LCD_WR_DATA8(0x33); //3f
    LCD_WR_DATA8(0x34); //3c

    LCD_WR_REG(0xC7);
    LCD_WR_DATA8(0xbe); //c6

    LCD_WR_REG(0x36);
    LCD_WR_DATA8(0x08);

    LCD_WR_REG(0xB1); // all default values anyway
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x1B);

    LCD_WR_REG(0xB6); // could change to normal black as a test
    LCD_WR_DATA8(0x0A);
    LCD_WR_DATA8(0xA2); //a2 white, 22, black

    LCD_WR_REG(0xF2);
    LCD_WR_DATA8(0x00);

    LCD_WR_REG(0x26);
    LCD_WR_DATA8(0x01);

    LCD_WR_REG(0xE0);
    LCD_WR_DATA8(0x0F);
    LCD_WR_DATA8(0x23);
    LCD_WR_DATA8(0x1F);
    LCD_WR_DATA8(0x09);
    LCD_WR_DATA8(0x0F);
    LCD_WR_DATA8(0x08);
    LCD_WR_DATA8(0x4B);
    LCD_WR_DATA8(0xF2);
    LCD_WR_DATA8(0x38);
    LCD_WR_DATA8(0x09);
    LCD_WR_DATA8(0x13);
    LCD_WR_DATA8(0x03);
    LCD_WR_DATA8(0x12);
    LCD_WR_DATA8(0x07);
    LCD_WR_DATA8(0x04);

    LCD_WR_REG(0xE1);
    LCD_WR_DATA8(0x00);
    LCD_WR_DATA8(0x1D);
    LCD_WR_DATA8(0x20);
    LCD_WR_DATA8(0x02);
    LCD_WR_DATA8(0x11);
    LCD_WR_DATA8(0x07);
    LCD_WR_DATA8(0x34);
    LCD_WR_DATA8(0x81);
    LCD_WR_DATA8(0x46);
    LCD_WR_DATA8(0x06);
    LCD_WR_DATA8(0x0E);
    LCD_WR_DATA8(0x0C);
    LCD_WR_DATA8(0x32);
    LCD_WR_DATA8(0x38);
    LCD_WR_DATA8(0x0F);

    LCD_WR_REG(0x11); //try putting at start?

    //
    // Delay for 120ms.
    //
    delayms(120);

    LCD_WR_REG(0x29);

    delayms(100);
}


/*
 * Function:    LCDILI9341Init
 *
 *              Initialises the port pins to which the LCD is connected
 *              configures the LCD into a state ready to receive screen
 *              data.
 */
void LCDILI9341Init( void )
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

void LCD_DrawVNums(void) {
    POINT_COLOR = GRAY;
    LCD_ShowString(6,0,"0");
    LCD_ShowString(64,0,"1");
    LCD_ShowString(122,0,"2");
    LCD_ShowString(180,0,"3");
    LCD_ShowString(230,0,"4");
}

void LCD_DrawTriggerLine(void) {
    unsigned char trig_char[16];
    unsigned long int trig_temp = TRIGGER_LINE-11;
    trig_temp = trig_temp * 190;
    trig_temp = trig_temp / 10;
    sprintf(trig_char, "%d mV", trig_temp);

    POINT_COLOR = BLACK;
    LCD_ShowString(TRIGGER_LINE-1,5,">");
    LCD_DrawLine((TRIGGER_LINE+3),11,(TRIGGER_LINE+3),318);
    LCD_ShowString(TRIGGER_LINE+1,5,">");
    LCD_DrawLine((TRIGGER_LINE+5),11,(TRIGGER_LINE+5),318);
    
    if (BUTTON_SELECT == 2) {
        POINT_COLOR = YELLOW;
    } else {
        POINT_COLOR = RED;
    }
    LCD_ShowString(TRIGGER_LINE,5,">");
    LCD_DrawLine((TRIGGER_LINE+4),11,(TRIGGER_LINE+4),318);
    LCD_ShowString(12,257,trig_char);
}

void LCD_DrawMeasLine(void) {
    unsigned char meas_char[16];
    unsigned long int meas_temp = MEASURE_LINE-11;
    meas_temp = meas_temp * 190;
    meas_temp = meas_temp / 10;
    sprintf(meas_char, "%d mV", meas_temp);

    POINT_COLOR = BLACK;
    LCD_ShowString(MEASURE_LINE-1,5,">");
    LCD_DrawLine((MEASURE_LINE+3),11,(MEASURE_LINE+3),318);
    LCD_ShowString(MEASURE_LINE+1,5,">");
    LCD_DrawLine((MEASURE_LINE+5),11,(MEASURE_LINE+5),318);
    
    if (BUTTON_SELECT == 3) {
        POINT_COLOR = YELLOW;
    } else {
        POINT_COLOR = BLUE;
    }
    LCD_ShowString(MEASURE_LINE,5,">");
    LCD_DrawLine((MEASURE_LINE+4),11,(MEASURE_LINE+4),318);
    LCD_ShowString(0,257,meas_char);
}  

void LCD_DrawScreenWords(unsigned int button_sel) {
    // Draw all words on screen
    
    POINT_COLOR = GRAY;
    LCD_ShowString(0,10," 1V/div ");
    
    if(AUTO_SAMPLE) {
        LCD_ShowString(0,67," NORM ");
    } else {
        LCD_ShowString(0,67," AUTO ");
    }
    LCD_ShowString(0,124," 700us/div ");
    if(EDGE_SAMPLE) {
        LCD_ShowString(0,201," Rising ");
    } else {
        LCD_ShowString(0,201," Falling ");
    }
    
    //LCD_DrawTriggerLine();
    //LCD_DrawMeasLine();
    
    POINT_COLOR = YELLOW;
    if(button_sel == 0) {
        if(AUTO_SAMPLE) {
            LCD_ShowString(0,67," NORM ");
        } else {
            LCD_ShowString(0,67," AUTO ");
        }
    } else if (button_sel == 1) {
        if(EDGE_SAMPLE) {
            LCD_ShowString(0,201," Rising ");
        } else {
            LCD_ShowString(0,201," Falling ");
        }
    } else if (button_sel == 2) {
        //POINT_COLOR = YELLOW;
        //LCD_DrawTriggerLine();
    } else if (button_sel ==3) {
        //POINT_COLOR = YELLOW;
        //LCD_DrawMeasLine();
    }
}