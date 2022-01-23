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



_FOSC(CSW_FSCM_OFF & XT_PLL4);
_FWDT(WDT_OFF);


//#define DRIVE_A PORTBbits.RB10
//#define DRIVE_B PORTCbits.RC13
#define DRIVE_A PORTCbits.RC13
#define DRIVE_B PORTCbits.RC14


/*
 * 
 */
unsigned int touchx, touchy, x_vrednost, y_vrednost, X, Y;
unsigned int distanca, echo_brojac = 0;
unsigned int servo_pwm;
unsigned char tempRX;
unsigned int adc_mq, adc_sb, adc_fo, broj, broj1, broj2, r; //ADC promenljive
unsigned int brojac_t1, brojac_t2, brojac_t3, vreme, vreme_paljenje, vreme_gasenje, vreme_paljenje_b, vreme_gasenje_b; //Tajmer promenljive

void pinInit() { //Inicijalizacija pinova 
    
    ADPCFGbits.PCFG10 = 1; //PIN B10 digitalni
    
    TRISAbits.TRISA11 = 0; //PIN D0 izlaz(buzzer)
    TRISDbits.TRISD9 = 0; //PIN D1 izlaz(servo_pin)
    TRISDbits.TRISD3 = 0; //PIN D3 izlaz(sb_trig_pin)
    TRISBbits.TRISB10 = 0; //PIN B6 izlaz(lcd_bckl_pin)
    TRISBbits.TRISB0 = 1; //PIN B7 ulaz(senzor_blizine)
    TRISDbits.TRISD9 = 1; //PIN A11 ulaz(echo_pin_interupt)
    TRISBbits.TRISB6 = 1; //PIN B8 ulaz(mq_senzor)
    TRISBbits.TRISB7 = 1; //PIN B2 ulaz(fotootpornik)
    TRISDbits.TRISD8 = 1; //PIN D8 ulaz(pir_senzor)
    TRISFbits.TRISF5 = 0; //PIN F5 izlaz
    TRISFbits.TRISF6 = 0; //PIN F6 izlaz
    TRISFbits.TRISF4 = 0; //PIN F4 izlaz
    TRISFbits.TRISF3= 1; //PIN C15 ulaz(prekidac_zatvorenih_vrata)
    
    //Touch 
	ADPCFGbits.PCFG8=0;
	ADPCFGbits.PCFG9=0;
	TRISBbits.TRISB8=1;
	TRISBbits.TRISB9=1;
    
}




/////////////////____ADC____///////////////////////
void __attribute__((__interrupt__)) _ADCInterrupt(void){
    
    adc_mq = ADCBUF0;
    adc_fo = ADCBUF1;
    touchx = ADCBUF2;
    touchy = ADCBUF3;
    
    
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

	Delay_ms(500); //cekamo jedno vreme da se odradi AD konverzija
				
	// ocitavamo x	
	x_vrednost = touchx;//temp0 je vrednost koji nam daje AD konvertor na BOTTOM pinu		

	// vode vertikalni tranzistori
     LATCbits.LATC13=0;
     LATCbits.LATC14=1;
	DRIVE_A = 0;  
	DRIVE_B = 1;

	Delay_ms(500); //cekamo jedno vreme da se odradi AD konverzija
	
	// ocitavamo y	
	y_vrednost = touchy;// temp1 je vrednost koji nam daje AD konvertor na LEFT pinu	
	
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
    
    
    unsigned int flag1 = 1;
    unsigned int flag2 = 0;
    int set_alko_proc = 10;
    initUART1();
    ADCinit();
    pinInit();
    GLCD_LcdInit();
	GLCD_ClrScr();
    //initTIMER2(30);
    initTIMER1(25);
    //Init_T1();
    //Init_T2();

    //OC1RS = 0;
    //initPWM_servo();
    ADCON1bits.ADON = 1;
    //initTrig();
  //Interrupt_init();
   OpenTimer3(25,0);
   
   CORCONbits.PSV = 1;
	//deo mkoji erovatno nije potreban ali pisan je jer je oc pravio problem zbog pullupa
	IEC1bits.OC4IE = 0;// Disable the Interrupt bit in IEC Register 
	OC4CONbits.OCM = 0;//Turn off Output Compare 4 
	IFS1bits.OC4IF = 0;//Disable the Interrupt Flag bit in IFS Register 
    GLCD_LcdInit();
	GLCD_ClrScr();
    while(1){
        //initPWM_servo();
        meny();
        Delay_ms(1000);
        GLCD_ClrScr();
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

