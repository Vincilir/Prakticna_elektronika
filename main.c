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
#include "driverGLCD.h"

//#include "tajmeri.h"



//_FOSC(CSW_FSCM_OFF & XT_PLL4);
_FOSC(CSW_FSCM_OFF & HS3_PLL4); 
_FWDT(WDT_OFF);
_FGS(CODE_PROT_OFF);

//#define DRIVE_A PORTBbits.RB10
//#define DRIVE_B PORTCbits.RC13
#define DRIVE_A PORTCbits.RC13
#define DRIVE_B PORTCbits.RC14

const unsigned int AD_Xmin =220;
const unsigned int AD_Xmax =3642;
const unsigned int AD_Ymin =3790;
const unsigned int AD_Ymax =3930;
/*
 * 
 */
 const char  brightness [1024] = {
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,240,240,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,240,240, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,240,240,240,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0, 14, 30, 60,120,240,224,192,  0,  0, 
   0,128,128,128,159,159,128,128,  0,  0,128,  0,192,224,112, 56, 
  28, 14,  6,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0, 14, 30, 60,120,240,224,192,  0,128,128,192,192,192,207,207, 
 192,192,128,128,  0,  0,224,240,112, 56, 28, 14,  7,  3,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  6, 14, 28, 56,112, 
 224,192,  0,128,128,192,192,192,207,207,207,192,192,128,128,  0, 
 192,224,112, 56, 28, 14,  7,  3,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,192,192,192,192,192,192,192,192,  0,  0,224,248,252,254, 
 255,255,255,255,255,255,255,255,255,255,254,253,240,  0,  0,192, 
 192,192,192,192,192,192,192,192,  0,  0,  0,  0,192,192,192,192, 
 192,192,192,192,  0,224,248,254, 15,  3,  3,129,241,252, 60,252, 
 241,193,  3,  3, 15,254,252,240,  0,192,192,192,192,192,192,192, 
 192,192,  0,  0,  0,  0,192,192,192,192,192,192,192,192,192,  0, 
 248,254, 31,  7,  3,  1,  1,  0,  0,  0,  0,  1,  1,  3,  7, 31, 
 254,248,  0,192,192,192,192,192,192,192,192,192,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,192,193,199, 31, 63, 
  63,127,127,255,255,255,127,127,127, 63, 31, 15,195,192,128,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,128,193,207,159, 60,112,110,239,195,195,195,195, 
 195,239,110,112, 60, 31,207,211,128,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128, 
 199,207, 30, 56,112, 96,224,224,192,192,224,224, 96,112, 56, 30, 
 207,199,128,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0, 24, 28, 14,  7,  3,  1,  0,  0,  0, 
   0,  0,  0,  0,252,252,  0,  0,  0,  0,  0,  0,  1,  1,  7, 15, 
  30, 28, 24,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0, 28, 30, 15,  7,  3,  1,  0,  0,  0,  0,  0,  0,  0,252,252, 
   0,  0,  0,  0,  0,  0,  1,  3,  7, 15, 30, 28, 40, 48,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 28, 28, 14,  7,  3, 
   1,  0,  0,  0,  0,  0,  0,  0,254,254,  0,  0,  0,  0,  0,  0, 
   0,  1,  3,  7, 14, 28, 28,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  3,  3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  3,  3, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  3,  3,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 
};

const char  DAN [1024] = {
   0,  0,  0,  0,  0,  0,  0,  0,248,236,  4,  6,  6, 30, 56, 32, 
  32, 56, 30,  6,  6,  4,236,248,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,128,  0,  0, 
   0,  0,  0,  2,  4,  8, 16, 32,  0,  0,128,128, 64, 64, 64, 79, 
  64, 64,128,128,  0, 32, 16,  8,  4,  2,  1,  0,  0,  0,128,128, 
   0, 56, 62, 99, 97,195,  3,  3,  3,  1,128,224, 48, 24,  8, 12, 
  12,  8, 24, 48,224,128,  1,  3,  3,  3,193, 97,103, 62, 56,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1, 
   2,  2,  4,  0,  0,224, 24,  4,  6,  1,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  1,  2,  4, 24,224,  0,  2,  2,  1,  1,  0,  0, 
   0, 12,124,198,134,195,192,192,192,128,  1,  7, 12, 24, 16, 48, 
  48, 16, 24, 12,  7,  1,128,192,192,192,195,134,230,124, 28,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1, 
   1,  1,128, 64,  0,  7, 24, 32, 64,128,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,192, 64, 32, 24,  7,  0,  0,  0,129,129,  1,  1, 
   0,  0,  0,  0,  0,  0,  0,  0, 31, 51, 32, 96, 96, 56, 28,  4, 
   4, 28,120, 96, 32, 48, 23, 15,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  4,  2, 
   1,  1,  0,  0,  0,  0,192, 48,  8,  0,  1,  1,  2,  2,  2,242, 
   2,  2,  1,  1,  0,  0,  8, 16, 32, 64,128,  0,  0,  0,  1,  1, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  3, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 
};


unsigned int touchx, touchy, x_vrednost, y_vrednost, X, Y, adc_X, adc_Y;
unsigned int distanca, echo_brojac = 0;
unsigned int servo_pwm;
unsigned char tempRX;
unsigned int adc_mq, adc_sb, adc_fo, broj, broj1, broj2, r; //ADC promenljive
unsigned int brojac_t1, brojac_t2, brojac_t3, vreme, vreme_paljenje, vreme_gasenje, vreme_paljenje_b, vreme_gasenje_b; //Tajmer promenljive


void Delay(unsigned int N)
{
	unsigned int i;
	for(i=0;i<N;i++);
}
void pinInit() { //Inicijalizacija pinova 
    
    //ADPCFGbits.PCFG10 = 1; //PIN B10 digitalni
    TRISAbits.TRISA11 = 0; //PIN D0 izlaz(buzzer)
    //TRISDbits.TRISD9 = 0; //PIN D1 izlaz(servo_pin)
    TRISDbits.TRISD3 = 0; //PIN D3 izlaz(sb_trig_pin)
    //TRISBbits.TRISB10 = 0; //PIN B6 izlaz(lcd_bckl_pin)
    //TRISBbits.TRISB0 = 1; //PIN B7 ulaz(senzor_blizine)
    //TRISDbits.TRISD9 = 1; //PIN A11 ulaz(echo_pin_interupt)
    //TRISBbits.TRISB6 = 1; //PIN B8 ulaz(mq_senzor)
    //TRISBbits.TRISB7 = 1; //PIN B2 ulaz(fotootpornik)
    TRISDbits.TRISD8 = 1; //PIN D8 ulaz(pir_senzor)
    TRISFbits.TRISF5 = 0; //PIN F5 izlaz
    TRISFbits.TRISF6 = 0; //PIN F6 izlaz
    TRISFbits.TRISF4 = 0; //PIN F4 izlaz
    TRISFbits.TRISF3= 1; //PIN C15 ulaz(prekidac_zatvorenih_vrata)
    TRISCbits.TRISC13= 0;
    TRISCbits.TRISC14= 0;
    //Touch 
	ADPCFGbits.PCFG8=0;
	ADPCFGbits.PCFG9=0;
	TRISBbits.TRISB8=1;
	TRISBbits.TRISB9=1;
    
}




/////////////////____ADC____///////////////////////
void __attribute__((__interrupt__)) _ADCInterrupt(void){
    
    //adc_mq = ADCBUF0;
    //adc_fo = ADCBUF1;
    touchx = ADCBUF0;
    touchy = ADCBUF1;
    
    adc_X = touchx;
    adc_Y = touchy;
    
    IFS0bits.ADIF = 0;
    
}

void __attribute__((__interrupt__)) _U1RXInterrupt(void) 
{
    IFS0bits.U1RXIF = 0;
    tempRX=U1RXREG;

} 


/////////////////____Tajmer____///////////////////////
void Delay_us (int vreme)//funkcija za kasnjenje u milisekundama
	{
		brojac_t1 = 0;
		while(brojac_t1 < vreme);
	}

void Delay_ms (int vreme)//funkcija za kasnjenje u milisekundama
	{
		brojac_t2 = 0;
		while(brojac_t1 < vreme);
	}

void __attribute__((__interrupt__)) _T1Interrupt(void)
{

   	TMR1 =0;
    brojac_t1++;//brojac za funkciju Delay_ms
    if(brojac_t1 == 250000) brojac_t2++;
    IFS0bits.T1IF = 0;   

}  

void __attribute__((__interrupt__)) _T2Interrupt(void)
{

   	//TMR2 =0;
    //TMR1 = 0;
    //brojac_t2++;//brojac za funkciju Delay_ms
    IFS0bits.T2IF = 0;   

} 






/////////////////____Servo____///////////////////////
void servo0(){
    LATDbits.LATD1=~LATDbits.LATD1;
    Delay_ms(1);
    LATDbits.LATD1=~LATDbits.LATD1;
    Delay_ms(19);
}
void servo90(){
    LATDbits.LATD1=~LATDbits.LATD1;
    Delay_ms(2);
    LATDbits.LATD1=~LATDbits.LATD1;
    Delay_ms(18);
}

void lcd_bck_light()
{
    vreme_paljenje = adc_fo;
    vreme_gasenje = 2800 - adc_fo;
        PORTBbits.RB10 = 1;
        Delay_us(vreme_gasenje);
        PORTBbits.RB10 = 0;
        Delay_us(vreme_paljenje);
        
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
 

unsigned int buzzer0(){
     
     return r = 0;
        
 }
 
 unsigned int buzzer100(){
     
     return r = 2000;
     
 }
 
 void buzzer_sb(){ //signal buzzer prilikom prilaska mq senzoru 
     
     OC1RS=buzzer0();
     Delay_us(10000);
     OC1RS=buzzer100();
     Delay_us(10000);
     
 }
 
 void buzzer_stop(){ //signal buzzer prilikom prisustva alkohola 
     unsigned int i;
     for(i = 0; i < 10; i++){
        //OC1RS=buzzer0();
        PORTDbits.RD0 = 0;
        Delay_us(350);
        PORTDbits.RD0 = 1;
        Delay_us(350);
        PORTDbits.RD0 = 0;
    } 
 }
 
  void buzzer_open(){ //signal buzzer prilikom prolaska alko_testa
     
      unsigned int i;
     for(i = 0; i < 15; i++){
        PORTDbits.RD0 = 0;
        Delay_us(100);
        PORTDbits.RD0 = 1;
        Delay_us(500);
        PORTDbits.RD0 = 0;
     }
 }
 
 /*void PWM_buzzer(int ispuna){
     
    PR2=5000;//odredjuje frekvenciju po formuli
    OC1RS=ispuna;//postavimo pwm
    //OC1R=0;//inicijalni pwm pri paljenju samo
    OC1CON =OC_IDLE_CON & OC_TIMER2_SRC & OC_PWM_FAULT_PIN_DISABLE& T2_PS_1_256;//konfiguracija pwma
    T2CONbits.TON=1;//ukljucujemo timer koji koristi

    //OC1RS=r;//ovim postavljamo faktor ispune
     
 }*/
 
 void readEcho(){
     
     while(!PORTAbits.RA11);
     T1CONbits.TON=1;
     while(PORTAbits.RA11);
     T1CONbits.TON = 0;
     echo_brojac = TMR1;
 }
void initPWM_servo(){
    
    PR2=800;//odredjuje frekvenciju po formuli
    OC2RS=adc_sb/10;//postavimo pwm
    //OC2R=1000;//inicijalni pwm pri paljenju samo
    OC2CON =OC_IDLE_CON & OC_TIMER2_SRC & OC_PWM_FAULT_PIN_DISABLE & T2_PS_1_1;//konfiguracija pwma
    T2CONbits.TON=1;//ukljucujemo timer koji koristi

    //OC2RS=servo_pwm;//ovim postavljamo faktor ispune
    
}

void initTrig(){
    
    TRISDbits.TRISD3 = 1;
    Delay_us(4);
    TRISDbits.TRISD3 = 0;
    
}

void Touch_Panel (void)
{
// vode horizontalni tranzistori
	DRIVE_A = 1;  
	DRIVE_B = 0;
    
     LATCbits.LATC13=1;
     LATCbits.LATC14=0;

	Delay(500); //cekamo jedno vreme da se odradi AD konverzija
				
	// ocitavamo x	
	x_vrednost = adc_X;//temp0 je vrednost koji nam daje AD konvertor na BOTTOM pinu		

	// vode vertikalni tranzistori
     LATCbits.LATC13=0;
     LATCbits.LATC14=1;
	DRIVE_A = 0;  
	DRIVE_B = 1;

	Delay(500); //cekamo jedno vreme da se odradi AD konverzija
	
	// ocitavamo y	
	y_vrednost = adc_Y;// temp1 je vrednost koji nam daje AD konvertor na LEFT pinu	
	
//Ako ?elimo da nam X i Y koordinate budu kao rezolucija ekrana 128x64 treba skalirati vrednosti x_vrednost i y_vrednost tako da budu u opsegu od 0-128 odnosno 0-64
//skaliranje x-koordinate

    X=(x_vrednost-161)*0.03629;



//X= ((x_vrednost-AD_Xmin)/(AD_Xmax-AD_Xmin))*128;	
//vrednosti AD_Xmin i AD_Xmax su minimalne i maksimalne vrednosti koje daje AD konvertor za touch panel.


//Skaliranje Y-koordinate
	Y= ((y_vrednost-500)*0.020725);

//	Y= ((y_vrednost-AD_Ymin)/(AD_Ymax-AD_Ymin))*64;
}






int main(int argc, char** argv) {

    unsigned int uslov = 1;
    unsigned int promena = 0;
    unsigned int temp = 0; 
    unsigned int flag1 = 1;
    unsigned int flag2 = 0;
    int set_alko_proc = 10;
    initUART1();
    ADCinit();
    pinInit();
    ConfigureLCDPins();
    GLCD_LcdInit();
	GLCD_ClrScr();
    //initTIMER2(30);
    initTIMER1(10000);
    //Init_T1();
    //Init_T2();

    //OC1RS = 0;
    //initPWM_servo();
    ADCON1bits.ADON = 1;
    //initTrig();
  //Interrupt_init();
   OpenTimer3(25,0);
   //GLCD_ClrScr();

    while(1){
        //initPWM_servo();
        //meny();
   
   GLCD_DisplayPicture(DAN);
   Touch_Panel();
   Delay_ms(500);
    //WriteUART1dec2string(Y);
   // WriteUART1(10);
   
   
   //***********PROVERA STANJA************ 
   if(Y > 30 && Y <= 64 && X > 0 && X < 31){
       temp = 1;  
       uslov = 1;
       promena = 1;
       Delay_ms(200);
   }
   GLCD_ClrScr();
   do{    
       Touch_Panel();
       Delay_ms(500);
       //GLCD_ClrScr();
   if (temp == 1){
       if(X > 0 && X <= 60 && Y > 38 && Y < 60){
           temp = 2;  //brightness
       }
       if(X > 0 && X <= 60 && Y > 9 && Y < 36){
           temp = 3;  //distance
       } 
       if(X > 64 && X <= 127 && Y > 38 && Y < 63){
           temp = 4;  //pir sensor
       }
       if(X > 64 && X <= 127 && Y > 9 && Y < 36){
           temp = 5;  //tolerance
       }
       if(X > 0 && X <= 7 && Y > 0 && Y < 49){
           temp = 0;
           uslov = 0;//NAZAD
       }
   }
       
       
   if (temp == 2){
       if(X > 6 && X <= 31 && Y > 19 && Y < 43){
            //tamnije
       }
       if(X > 53 && X <= 74 && Y > 19 && Y < 43){
            //AUTO
       } 
       if(X > 93 && X <= 117 && Y > 19 && Y < 43){
            //svetlije
       }
       
       if(X > 0 && X <= 7 && Y > 0 && Y < 49){
           temp = 1; 
           promena = 1;//NAZAD
       }   
   }
       
   if (temp == 3){    //DISTANCA
       if(X > 0 && X <= 61 && Y > 9 && Y < 36){
             //+  POVECAJ DISTANCU
       }
       if(X > 65 && X <= 127 && Y > 9 && Y < 36){
             //-  SMANJI DISTANCU
       } 

       if(X > 0 && X <= 7 && Y > 0 && Y < 49){
           temp = 1;
           uslov = 0;//NAZAD
       }
   }
       
       
   if (temp == 4){    //PIR SENZOR
       if(X > 0 && X <= 61 && Y > 9 && Y < 36){
             // ON
       }
       if(X > 65 && X <= 127 && Y > 9 && Y < 36){
             // OFF
       } 

       if(X > 0 && X <= 7 && Y > 0 && Y < 49){
           temp = 1;
           uslov = 0;//NAZAD
       }
   }
       
    if (temp == 5){    //TOLERANCIJA
       if(X > 0 && X <= 61 && Y > 9 && Y < 36){
             //+  POVECAJ TOLERANCIJU
       }
       if(X > 65 && X <= 127 && Y > 9 && Y < 36){
             //-  SMANJI TOLERANCIJU
       } 

       if(X > 0 && X <= 7 && Y > 0 && Y < 49){
           temp = 1;
           uslov = 0;//NAZAD
       }
   }
   
  
   
   //************ISPIS SLIKE*******************
   if (temp == 1 && promena == 1){ //meni
       meny();
       promena =0;
      }
   
    if (temp == 2 && promena == 1){     //brightness
       GLCD_DisplayPicture(brightness);
       GoToXY(0,0);
       GLCD_Rectangle (0,56,49,63);
       GoToXY(10,7);
       GLCD_Printf ("NAZAD");
       promena = 0;
      }
   
   
     if (temp == 3 && promena == 1){   //distamce
        GLCD_ClrScr();
        GoToXY(0,0);
        GLCD_Rectangle (0,0,127,25);
        GoToXY(20,1);
        GLCD_Printf ("DISTANCA");
        GLCD_Rectangle (0,27,61,54);
        GoToXY(25,5);
        GLCD_Printf ("+");
        GLCD_Rectangle (65,27,127,54);
        GoToXY(90,5);
        GLCD_Printf ("-");
        GoToXY(0,0);
        GLCD_Rectangle (0,56,49,63);
        GoToXY(10,7);
        GLCD_Printf ("NAZAD");
        promena = 0;
      }   
       
    if (temp == 4 && promena == 1){   //pir senzor
        GLCD_ClrScr();
        GoToXY(0,0);
        GLCD_Rectangle (0,0,127,25);
        GoToXY(20,1);
        GLCD_Printf ("PIR SENZOR");
        GLCD_Rectangle (0,27,61,54);
        GoToXY(25,5);
        GLCD_Printf ("ON");
        GLCD_Rectangle (65,27,127,54);
        GoToXY(90,5);
        GLCD_Printf ("OFF");
        GoToXY(0,0);
        GLCD_Rectangle (0,56,49,63);
        GoToXY(10,7);
        GLCD_Printf ("NAZAD");
        promena =0;
      }
      
    if (temp == 5 && promena == 1){   //TOLERANCIJA
        GLCD_ClrScr();
        GoToXY(0,0);
        GLCD_Rectangle (0,0,127,25);
        GoToXY(20,1);
        GLCD_Printf ("TOLERANCIJA");
        GLCD_Rectangle (0,27,61,54);
        GoToXY(25,5);
        GLCD_Printf ("+");
        GLCD_Rectangle (65,27,127,54);
        GoToXY(90,5);
        GLCD_Printf ("-");
        GoToXY(0,0);
        GLCD_Rectangle (0,56,49,63);
        GoToXY(10,7);
        GLCD_Printf ("NAZAD");
        promena =0;
      }
       
    if (temp == 5 && promena == 1){ 
       GLCD_DisplayPicture(DAN);
       promena =0;
       
    }   
       
   } while (uslov == 1);
    
        /*int dis = 0;
        LATDbits.LATD3 = 1;
        Delay_ms(10);
        LATDbits.LATD3 = 0;
        
        while(PORTDbits.RD9);
            T3CONbits.TON = 1;
        
        if(!PORTDbits.RD9 || IFS0bits.T3IF == 1)  
            T3CONbits.TON = 0;
            
            
            if(PORTDbits.RD9 == 0){
                dis = TMR3;
                dist = (float)dis*12.8/58;
                
            }
            
            if(IFS0bits.T3IF == 1){
                IFS0bits.T3IF = 0; 
            }*/
 
        /*if(PORTDbits.RD8 && flag1 == 1) {
            
            RS232_putst("Korisnik je ispred uredjaja");
            flag1 = 0;
            flag2 = 0;
            while(adc_mq < 50);
        }else
             PORTFbits.RF4 = 0;
        
        
        
            
        //echo_brojac = TMR1 * 4.5333e-5;
        
        //int test = alko_procenat();
        //WriteUART1dec2string(dist);
        //WriteUART1(dis);
        //WriteUART1(echo_brojac);
        
        while(flag1 == 0){
            
            
            if(adc_mq < 1000 && flag2 == 0){
               
                PORTFbits.RF6 = 1;
                buzzer_open();
                servo90();
                flag2 = 1; 
                RS232_putst("Korisnik prosao alko test");
                
            }
            while(adc_mq > 1000 && flag2 == 0){
             
                
                PORTFbits.RF5 = 1;
                buzzer_stop();
                PORTFbits.RF5 = 0;
                flag2 = 1;
                RS232_putst("Korisnik nije prosao alko test");
                flag1 = 1;
                
            }
            
            if(PORTFbits.RF3 == 1){
        
                flag1 = 1;
                flag2 = 0;
                PORTFbits.RF6 = 0;
                RS232_putst("Vrata uspesno zatvorena");
               
            }
           
            
        }*/
      //T2CONbits.TON=1;
      
    }
    
    return (EXIT_SUCCESS);
}

