/* 
 * File:   lplc-adc.h
 * Author: mhibbett
 *
 * Created on 19 November 2014, 00:05
 */

#ifndef LPLC_ADC_H
#define	LPLC_ADC_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <plib\adc.h>

/* These are application specific values are variables */
#define NUM_SAMPLES 308 /* Number of ADC samples supported */
#define NUM_SAMPLE_BUFFS 2 /* Number of samples buffers: "previous" and "current" */
      
extern unsigned char samples[NUM_SAMPLE_BUFFS][NUM_SAMPLES];
extern unsigned char current_buff;
extern unsigned char samples_ready;

void adc_init(unsigned char param1, unsigned char param2, unsigned char param3, unsigned int param4);
void convert_adc(void);
char busy_adc(void);
void close_adc(void);
int getval_adc(void);
void handle_int_adc(void);
void int_enable_adc(void);
void LCD_DrawVNums(void);
void LCD_DrawTriggerLine(void);
void LCD_DrawMeasLine(void);

#ifdef	__cplusplus
}
#endif

#endif	/* LPLC_ADC_H */

