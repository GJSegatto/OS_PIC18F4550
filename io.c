#include "io.h"
#include <pic18f4550.h>

int adc_read(void) {
    ADCON0bits.GO = 1;
    while(ADCON0bits.GODONE);
    return ADRES;
}

void adc_config(void) {
    ADCON0bits.CHS = 0b0000;    //CANAL 0
    ADCON1bits.VCFG = 0b00;     // Vref+ = Vdd, Vref- = Vss
    ADCON1bits.PCFG = 0b1110;   //A0 como analógico
    ADCON2bits.ADFM = 1;        //Justificado à direita
    ADCON2bits.ACQT = 0b101;   // 12 Tad
    ADCON2bits.ADCS = 0b101;   // Fosc/16
    ADCON0bits.ADON = 1;        //ATIVA O MÓDULO ADC  
}

void pwm_config(void) {
    TRISCbits.RC0 = 0;

    //LIGANDO TIMER2 POIS É NECESSÁRIO
    PR2 = 49;                               // Ex.: Para Fosc= 4MHz, PR2=49 gera PWM de 5 kHz

    uint16_t dc = 100;                      // 50 % de 200 passos
    CCPR1L            = (dc >> 2);          // = 25
    CCP1CONbits.DC1B0 = (dc >> 0) & 1;      // = 0
    CCP1CONbits.DC1B1 = (dc >> 1) & 1;      // = 0

    TRISCbits.TRISC2 = 0;                   //RC2 COMO SAÍDA

    T2CONbits.T2CKPS = 0b01;                // Prescaler 4
    T2CONbits.TMR2ON = 1;

    T2CONbits.TMR2ON = 1;                   // Liga Timer2
    CCP1CONbits.CCP1M = 0b1100;             //ATIVANDO MODO PWM
}

void config_interruption(void) {
    TRISBbits.TRISB0 = 1;           //INT0 COMO ENTRADA
    INTCON2bits.INTEDG0 = 1;        //BORDA DE SUBIDA
    INTCONbits.INT0F = 0;           //ZERAR FLAG
    INTCONbits.INT0IE = 1;          //HABILITA A INTERRUPÇÃO EM INT0
    INTCONbits.GIE = 1;             //HABILITA INTERRUPÇÃO GLOBAL
    INTCONbits.PEIE = 1;            //HABILITA INTERRUPÇÃO DE PERIFÉRICOS
}
