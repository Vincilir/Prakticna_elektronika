/* 
 * File:   main.c
 * Author: Vincilir
 *
 * Created on December 21, 2021, 11:22 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <p30fxxxx.h>
#include "adc.h"
#include "tajmeri.h"



_FOSC(CSW_FSCM_OFF & XT_PLL4);
_FWDT(WDT_OFF);

/*
 * 
 */
unsigned int adc_mq, adc_sb, adc_fo; //ADC promenljive
unsigned int brojac, vreme; //Tajmer promenljive

void pinInit() { //Inicijalizacija pinova 

    ADPCFGbits.PCFG6 = 1; //PIN B6 digitalni
    ADPCFGbits.PCFG7 = 0; //PIN B7 analogni
    ADPCFGbits.PCFG8 = 0; //PIN B8 analogni
    ADPCFGbits.PCFG9 = 0; //PIN B9 analogni
    
    TRISBbits.TRISB6 = 0; //PIN B6 izlaz(servo_pin)
    TRISBbits.TRISB7 = 1; //PIN B7 ulaz(senzor_blizine)
    TRISBbits.TRISB8 = 1; //PIN B8 ulaz(mq_senzor)
    TRISBbits.TRISB9 = 1; //PIN B9 ulaz(fotootpornik)
    TRISDbits.TRISD8 = 1; //PIN D8 ulaz(pir_senzor)

}


/////////////////____ADC____///////////////////////
void __attribute__((__interrupt__)) _ADCInterrupt(void){
    
    adc_sb = ADCBUF0;
    adc_mq = ADCBUF1;
    adc_fo = ADCBUF2;
    
    IFS0bits.ADIF = 0;
    
}


/////////////////____Tajmer____///////////////////////
void Delay_ms (int vreme)//funkcija za kasnjenje u milisekundama
	{
		brojac = 0;
		while(brojac < vreme);
	}



void __attribute__((__interrupt__)) _T1Interrupt(void)
{

   	TMR2 =0;
    brojac++;//brojac za funkciju Delay_ms
    IFS0bits.T1IF = 0;   

}  


/////////////////____Servo____///////////////////////
void servo0(){
    LATBbits.LATB4=~LATBbits.LATB4;
    Delay_ms(10);
    LATBbits.LATB4=~LATBbits.LATB4;
    Delay_ms(190);
}
void servo90(){
    LATBbits.LATB4=~LATBbits.LATB4;
    Delay_ms(20);
    LATBbits.LATB4=~LATBbits.LATB4;
    Delay_ms(180);
}



int main(int argc, char** argv) {

    
    
    return (EXIT_SUCCESS);
}

