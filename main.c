/* 
 * File:   main.c
 * Author: Vincilir
 *
 * Created on December 21, 2021, 11:22 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <p30fxxxx.h>
#include "outcompare.h"
#include "adc.h"
#include "timer2.h"
#include "timer1.h" 



_FOSC(CSW_FSCM_OFF & XT_PLL4);
_FWDT(WDT_OFF);

/*
 * 
 */
unsigned char tempRX;
unsigned int adc_mq, adc_sb, adc_fo, broj, broj1, broj2, r; //ADC promenljive
unsigned int brojac, vreme, vreme_paljenje, vreme_gasenje, vreme_paljenje_b, vreme_gasenje_b; //Tajmer promenljive

void pinInit() { //Inicijalizacija pinova 
    
    ADPCFGbits.PCFG10 = 1; //PIN B10 digitalni
    
    TRISDbits.TRISD0 = 0; //PIN D0 izlaz(buzzer)
    TRISDbits.TRISD1 = 0; //PIN D6 izlaz(servo_pin)
    TRISBbits.TRISB10 = 0; //PIN B6 izlaz(lcd_bckl_pin)
    TRISBbits.TRISB0 = 1; //PIN B7 ulaz(senzor_blizine)
    TRISBbits.TRISB1 = 1; //PIN B8 ulaz(mq_senzor)
    TRISBbits.TRISB2 = 1; //PIN B9 ulaz(fotootpornik)
    TRISDbits.TRISD8 = 1; //PIN D8 ulaz(pir_senzor)
    
}


/////////////////____ADC____///////////////////////
void __attribute__((__interrupt__)) _ADCInterrupt(void){
    
    adc_sb = ADCBUF0;
    adc_mq = ADCBUF1;
    adc_fo = ADCBUF2;
    
    IFS0bits.ADIF = 0;
    
}

void __attribute__((__interrupt__)) _U1RXInterrupt(void) 
{
    IFS0bits.U1RXIF = 0;
    tempRX=U1RXREG;

} 


/////////////////____Tajmer____///////////////////////
void Delay_ms (int vreme)//funkcija za kasnjenje u milisekundama
	{
		brojac = 0;
		while(brojac < vreme);
	}



void __attribute__((__interrupt__)) _T2Interrupt(void)
{

   	TMR2 =0;
    //TMR1 = 0;
    brojac++;//brojac za funkciju Delay_ms
    IFS0bits.T1IF = 0;   

}  


/////////////////____Servo____///////////////////////
void servo0(){
    LATDbits.LATD1=~LATDbits.LATD1;
    Delay_ms(10);
    LATDbits.LATD1=~LATDbits.LATD1;
    Delay_ms(190);
}
void servo90(){
    LATDbits.LATD1=~LATDbits.LATD1;
    Delay_ms(20);
    LATDbits.LATD1=~LATDbits.LATD1;
    Delay_ms(180);
}

void lcd_bck_light()
{
    vreme_paljenje = adc_fo;
    vreme_gasenje = 2800 - adc_fo;
        PORTBbits.RB10 = 1;
        Delay_ms(vreme_gasenje);
        PORTBbits.RB10 = 0;
        Delay_ms(vreme_paljenje);
        
}

int pirDetection()
{
    if(PORTDbits.RD8)
        return 1;
    else
        return 0;
    
}

 int alko_procenat(){
    
    int rezultat;
    rezultat = (adc_mq/4000) * 100;
    return rezultat;
    
} 


int main(int argc, char** argv) {

    initUART1();
    ADCinit();
    //pinInit();
    Init_T2();
    //initTIMER1(10);
    TRISDbits.TRISD0 = 0;
    PR2=2499;//odredjuje frekvenciju po formuli
        OC1RS=20;//postavimo pwm
        OC1R=1000;//inicijalni pwm pri paljenju samo
        OC1CON =OC_IDLE_CON & OC_TIMER2_SRC & OC_PWM_FAULT_PIN_DISABLE& T2_PS_1_256;//konfiguracija pwma
        T2CONbits.TON=1;//ukljucujemo timer koji koristi


        r=2500;
        OC1RS=r;//ovim postavljamo faktor ispune
    ADCON1bits.ADON = 1;
    
    
    while(1){
        /*int rez = alko_procenat();
        //lcd_bck_light();
		WriteUART1dec2string(adc_mq);
		//for(broj2=0;broj2<1000;broj2++);
		//enter
        WriteUART1(' ');
        WriteUART1dec2string(rez);
        WriteUART1(' ');
        WriteUART1(13);*/
        
        if(r<2499)
            {
                r++;
                OC1RS=r;//ovim postavljamo pwm
            }
        else
            {

                r=0;
                OC1RS=r;

            }


            for(broj1=0;broj1<20;broj1++)
                for(broj2=0;broj2<60;broj2++);
    }
    
    return (EXIT_SUCCESS);
}

