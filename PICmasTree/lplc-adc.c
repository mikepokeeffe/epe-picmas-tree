/*
 * File:    lplc-adc.c
 *
 *          Support for analogue to digital conversion library.
 *          This makes use of the Microchip Library functions.
 *
 * Author: mike
 *
 * Created on 27th September 2014, 21:49
 */

#include <stdio.h>
#include <stdlib.h>
#include "lplc-adc.h"
//#include "LCD-Graphic-ST7735S.h"
#include "LCD-Graphic-ILI9341.h"
#include "timers.h"

unsigned char samples[NUM_SAMPLE_BUFFS][NUM_SAMPLES];
unsigned char current_buff = 0; /* which buffer we are writing to */
unsigned char saved_buff = 0;   /* Saved Buffer to overwrite during normal capture mode */
unsigned char samples_ready = 0;  /* Set when buffer is full */
unsigned int sample_index = 0; /* Position to write to next in samples buffer */
int current_adc = 0; /* The most recent FULL reading from ADC */

void adc_init(unsigned char param1, unsigned char param2, unsigned char param3, unsigned int param4)
{
    // For now, we simply 'wrap' the peripheral library's function
    OpenADC(param1, param2, param3, param4);
}

void convert_adc(void)
{
    // start a conversion
    ConvertADC();
}

char busy_adc(void)
{
    // Return true if the ADC is currently converting an input
    return BusyADC();
}

void close_adc(void)
{
    PIR1bits.ADIF = 0;
    PIE1bits.ADIE = 0;
    CloseADC();
}

int getval_adc(void)
{
    return ReadADC();
}

void int_enable_adc(void)
{
    PIR1bits.ADIF = 0;
    PIE1bits.ADIE = 1;
}

void handle_int_adc(void)
{
    // read the ADC value from ADRESH/L, store it somewhere
    // Set a variable to tell the main program that a value has been received,
    // or handle here in the interrupt
    // Then restart the conversion.

    unsigned char next_sample = 0;
    unsigned char prev_sample = 0;
    
    current_adc = ReadADC();

    if (!samples_ready) {
        // Take the last sample value and store it in prev_sample
        prev_sample = samples[current_buff][sample_index];
        // Increment the sample_index to store new adc value
        samples[current_buff][sample_index++] = 11 + ((current_adc << 1)/13);
        /* scale the sample to our range of 11-238, or 11 + 0-227*/
        // Take latest sample and store in next_sample
        next_sample = samples[current_buff][sample_index];

        // Compare previous sample and current sample versus our TRIGGER_LINE global variable
        if((prev_sample <= (TRIGGER_LINE)) && (next_sample >= TRIGGER_LINE) && (!RISING_EDGE)) {
            FALLING_EDGE = 1;
        } 

        if((prev_sample >= (TRIGGER_LINE)) && (next_sample <= TRIGGER_LINE) && (!FALLING_EDGE)) {
            RISING_EDGE = 1;
        }
        
        if (sample_index == NUM_SAMPLES) {
            /* Swap to the next buffer */
            current_buff = (current_buff == 0) ? 1 : 0;
            sample_index = 0;
            samples_ready = 1;
        }
    }   
}
