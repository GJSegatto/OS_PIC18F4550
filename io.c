#include "io.h"
#include <stdint.h>

// Função para ler o valor do ADC
uint16_t adc_read(void) {
    ADCON0bits.GO = 1;                 // Inicia a conversão ADC
    while(ADCON0bits.GODONE);          // Aguarda a conversão terminar
    return ADRES;                      // Retorna o valor convertido
}

// Configurações iniciais do ADC
void adc_config(void) {
    ADCON0bits.CHS = 0b0000;           // Seleciona o canal 0 (AN0)
    ADCON1bits.VCFG = 0b00;            // Vref+ = Vdd, Vref- = Vss
    ADCON1bits.PCFG = 0b1110;          // AN0 como analógico, os demais como digitais
    ADCON2bits.ADFM = 1;               // Resultado justificado à direita
    ADCON2bits.ACQT = 0b101;           // Tempo de aquisição: 12 Tad
    ADCON2bits.ADCS = 0b101;           // Clock de conversão: Fosc/16
    ADCON0bits.ADON = 1;               // Ativa o módulo ADC
}

// Configurações iniciais do PWM
void pwm_config(void) {
    //OSCCON = 0x36;                   // (Comentado) Configuração do clock interno
    TRISCbits.RC2 = 0;                // Define RC2 como saída (PWM)
    PR2 = 199;                        // Período do PWM (para duty cycle de 50%)
    CCPR1L = 100;                     // Duty cycle inicial de 50%
    T2CONbits.T2CKPS = 0b00;          // Prescaler do Timer2 em 1:16
    CCP1CONbits.DC1B = 0b00;          // Bits menos significativos do duty cycle
    CCP1CONbits.CCP1M = 0b1100;       // Ativa o modo PWM no CCP1
}

// Ativa o PWM e ajusta o duty cycle
void activate_pwm(uint16_t dc) {
    if(!T2CONbits.TMR2ON) {
        TMR2 = 0;                     // Zera o Timer2
        T2CONbits.TMR2ON = 1;         // Liga o Timer2
    }
    uint16_t pwm_value = ((uint32_t)(PR2 + 1) * dc) / 50; // Calcula o valor do duty cycle
    CCPR1L = (uint8_t)(pwm_value >> 2);                   // 8 bits mais significativos
    CCP1CONbits.DC1B = pwm_value & 0x03;                  // 2 bits menos significativos
}

// Desativa o PWM
void stop_pwm() {
    CCP1CONbits.CCP1M = 0b0000;       // Desativa o modo PWM
    T2CONbits.TMR2ON = 0;             // Desliga o Timer2
    TMR2 = 0;                         // Zera o Timer2
    TRISCbits.TRISC2 = 0;             // Garante que RC2 está como saída
}

// Configura interrupção externa INT0
void config_interruption(void) {
    TRISBbits.TRISB0 = 1;             // INT0 como entrada
    INTCON2bits.INTEDG0 = 1;          // Interrupção na borda de subida
    INTCONbits.INT0F = 0;             // Zera a flag de interrupção
    INTCONbits.INT0IE = 1;            // Habilita interrupção INT0
    INTCONbits.GIE = 1;               // Habilita interrupções globais
    INTCONbits.PEIE = 1;              // Habilita interrupções de periféricos
}
