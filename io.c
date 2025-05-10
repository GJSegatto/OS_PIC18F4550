#include "io.h"
#include "kernel.h"
#include <stdint.h>

uint16_t adc_read(void) {
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
    OSCCON = 0x36;
    TRISCbits.RC2 = 0;
    PR2 = 199;                  //PARA DUTY CICLE DE 50%
    CCPR1L = 100;               //PARA DUTY CICLE DE 50%
    T2CONbits.T2CKPS = 0b00;    //PREESCALER 1:16
    CCP1CONbits.DC1B = 0b00;    //LSB DO PWM
    CCP1CONbits.CCP1M = 0b1100; //LIGA PWM
}

void activate_pwm(uint16_t dc) {
    if(!T2CONbits.TMR2ON) {
        TMR2 = 0;
        T2CONbits.TMR2ON = 1;
    }
    uint16_t pwm_value = ((uint32_t)(PR2 + 1) * dc) / 50;
    CCPR1L = (uint8_t)(pwm_value >> 2); // Os 8 bits mais significativos
    CCP1CONbits.DC1B = pwm_value & 0x03; // Os 2 bits menos significativos
}

void stop_pwm() {
    if(T2CONbits.TMR2ON) {
        TMR2 = 0;
        T2CONbits.TMR2ON = 0;
    }
}

void config_interruption(void) {
    TRISBbits.TRISB0 = 1;           //INT0 COMO ENTRADA
    INTCON2bits.INTEDG0 = 1;        //BORDA DE SUBIDA
    INTCONbits.INT0F = 0;           //ZERAR FLAG
    INTCONbits.INT0IE = 1;          //HABILITA A INTERRUPÇÃO EM INT0
    INTCONbits.GIE = 1;             //HABILITA INTERRUPÇÃO GLOBAL
    INTCONbits.PEIE = 1;            //HABILITA INTERRUPÇÃO DE PERIFÉRICOS
}
