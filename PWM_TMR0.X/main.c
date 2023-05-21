
// PIC16F877A Configuration Bit Settings

// 'C' source line config statements

// CONFIG
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = ON         // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3/PGM pin has PGM function; low-voltage programming enabled)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.
#include <xc.h>
#include "pic16f877a.h"
#define _XTAL_FREQ 800000
#define RS RD2
#define EN RD3
#define D4 RD4
#define D5 RD5
#define D6 RD6
#define D7 RD7
#include<LCD.h>
#include<stdio.h>

int counter=0;
int pwm_duty_cycle=50; // First value of pwm duty cycle
int dizi[20];

void __interrupt () TMR_Intt(){
    GIE=0;
    if (T0IF){
       counter++;
       if (counter==101) counter=0; //Pwm Period = 100* Tmr0 period
       if (counter < pwm_duty_cycle ){ // adjusting duty cycle through counter
        RC3=1;    
        }
       if (counter>pwm_duty_cycle){
        RC3=0;    
 }  
    }
    GIE=1;
    T0IF=0;
    TMR0=0;
}

 void TMR0_Init(){ // Timer 0 initilaze.
 GIE = 1; PEIE= 1; T0IE = 1;
 T0CS=0; PSA=0;
 PS2=0; PS1=1; PS0=1;
 // Set 62Khz Timer 0 
 }
 
void main(){
 TRISBbits.TRISB4=1; // Set PortB as a input
 TRISBbits.TRISB5=1;
 TRISC=0x00; TRISD=0;
 PORTC=0; PORTD=0;
 TMR0_Init();
 TMR0=0; 
 Lcd_Init(); // LCD Initilaze

 while(1){ 
     RC0=1;
     RC1=0;
     while(RBIF){
        if(RB4 && pwm_duty_cycle <100 ) pwm_duty_cycle++; // to increase duty cycle pushing RB4 button
        if(RB5 && pwm_duty_cycle > 0 && pwm_duty_cycle<101) pwm_duty_cycle--; 
        RBIF=0;
     }
     Lcd_Set_Cursor(1,1);
     sprintf (dizi,"Pwm Duty Cycle= %d");
     Lcd_Write_String(dizi);
     Lcd_Set_Cursor(2,1);
     sprintf (dizi,"      %.3u",pwm_duty_cycle);
     Lcd_Write_String(dizi);
 }
}

