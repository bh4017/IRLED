/*
 * File:   main.c
 * Author: bhoskins
 *
 * Created on 20 September 2017, 16:50
 */
#include <xc.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>

#pragma config FOSC=INTOSC, PLLEN=OFF, WDTE=OFF, MCLRE=ON,
#pragma config CLKOUTEN=OFF, IESO=OFF, FCMEN=OFF,CP=OFF, CPD=OFF,BOREN=OFF
#pragma config WRT=OFF,STVREN=ON,BORV=LO,LVP=OFF

/* DEFINITIONS */
#define _XTAL_FREQ  16000000        // this is used by the __delay_ms(xx) and __delay_us(xx) functions
unsigned int DutyCycle;

void set_pwm_duty_cycle(unsigned int duty_cycle_value)
{
    CCP1CONbits.DC1B = duty_cycle_value & 0x03; //first set the 2 lsb bits
    CCPR1L =  (duty_cycle_value >> 2);           //now set upper 8 msb bits
}

void setup_hardware(void)
{
    /* SETUP OSCCON */
    OSCCONbits.SPLLEN=0;        // PLL is disabled
    OSCCONbits.IRCF=0x0F;       // Set OSCCON IRCF bits to select OSC frequency=16Mhz
    OSCCONbits.SCS=0x02;        // Set the SCS bits to select internal oscillator block
    /* SETUP IO PORTS */
    // ANSEL SETUP
    ANSELAbits.ANSELA=0;        // Set all analog pins to digital I/O
    ADCON0bits.ADON=0;          // Turn ADC off
    DACCON0bits.DACEN=0;        // Turn DAC off
    // PORTA DIRECTION
    TRISAbits.TRISA0 = 0;       // INDICATION OUTPUT
    TRISAbits.TRISA2 = 1;       // MODE INPUT
    TRISAbits.TRISA3 = 1;       // RESET
    TRISAbits.TRISA4 = 0;       // LED ENABLE
    TRISAbits.TRISA5 = 1;       // PWM OUTPUT - DISABLE TO START WITH
    // PORTA INITIAL OUTPUT
    LATAbits.LATA0 = 0;
    LATAbits.LATA4 = 0;
    LATAbits.LATA5 = 0;
    /* SETUP INTERRUPTS */
    /* SETUP PWM */
    // PWM Period = (1/Fosc) * 4 * (TMR2 Prescaler) * (PR2+1)
    // = 16E6^-1 * 4 * 64 * 124 = 500Hz
    TRISAbits.TRISA5 = 1;       // PWM OUTPUT - DISABLE TO START WITH
    APFCONbits.CCP1SEL = 1;     // Set PWM output to RA5
    PR2 = 0x7B;                 // Set PWM Period
    CCP1CONbits.CCP1M=0x0C;     // Select PWM mode for CCP module
    CCP1CONbits.P1M=0x00;       // Select single output on CCP1 pin (RA5)
    PIR1bits.TMR2IF=0;          // Clear TMR2 interrupt flag
    T2CONbits.T2CKPS= 3;        // Select TMR2 prescalar as divide by 64 as per our example above
    T2CONbits.TMR2ON=1;         // Turn TMR2 on
    TRISAbits.TRISA5 = 0;       // Turn PWM output back on   
}

void main(void) 
{
    DutyCycle = 0;
    setup_hardware();
    while(DutyCycle <= 1022)
    {
        set_pwm_duty_cycle(DutyCycle);
        DutyCycle = DutyCycle + 2;
        __delay_ms(10);
    }
    
    while(1);
//    for(int i = 0; i <= 1023; i+= 32)
//    {
//        set_pwm_duty_cycle(i);
//        __delay_ms(100);
//    }
    return;
}
