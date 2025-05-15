#include "user_app.h"
#include "kernel.h"
#include "sync.h"
#include "pipe.h"
#include "config.h"
#include "io.h"
#include <pic18.h>
#include <pic18f4550.h>
#include <stdint.h>

extern void idle(void);

#if SISTEMA_CARRO == ON

pipe_t *pipe;
mutex_t mutex;
uint16_t adc_value = 0;

//LEITURA DO ADC E ENVIO DA INFO
TASK acelerador()
{
    uint16_t pedal;
    while(1) {
        pedal = adc_read();
        write_pipe(pipe,pedal);
        delay(5);     
    }
}

//LEITURA DA INFO DO ACELERADOR E ALOCACAO NA VARIAVEL COMPARTILHADA
TASK controle_central()
{
    while (1) {
        mutex_lock(&mutex);
        read_pipe(pipe, &adc_value);
        mutex_unlock(&mutex);
        delay(5);
    }    
}

//CONTROLE DOS BICOS COM O PWM
TASK injecao_eletronica()
{
    uint16_t duty_cicle;
    while (1) {
        mutex_lock(&mutex);
        duty_cicle = (uint16_t)((uint32_t)adc_value * 195 / 1000);
        mutex_unlock(&mutex);

        if(duty_cicle > 200) {
            duty_cicle = 200;
        } 
        else if(duty_cicle < 0) {
            duty_cicle = 0;
        }
        activate_pwm(duty_cicle);
        delay(5);
   }
}

//CONFIGURACOES NECESSARIAS
void user_config()
{
    adc_config();
    pwm_config();

    create_pipe(pipe, PIPE_SIZE);
    
    mutex_init(&mutex);

    TRISEbits.RE0 = 0;
    
    // Tarefas como funcoes globais para que o compilador nao as retire na fase de otimizacao.
    asm("global _acelerador, _controle_central, _injecao_eletronica");
}

#elif TESTE == ON

pipe_t pipe;
mutex_t mutex;
uint16_t adc_value = 0;

TASK acelerador()
{
    while(1) {
 
    }
}

TASK controle_central()
{
    while (1) {
        LATDbits.LATD0 = 1;
        LATDbits.LATD1 = 1;
        LATDbits.LATD2 = 1;
    }    
}

TASK injecao_eletronica()
{
    while (1) {

   }
}

void user_config()
{
    TRISDbits.RD0 = 0;
    TRISDbits.RD1 = 0;
    TRISDbits.RD2 = 0;
    
    // Define as tarefas como fun��es globais para
    // evitar que o compilador as retire na fase
    // de gera��o de otimiza��o.
    asm("global _acelerador, _controle_central, _injecao_eletronica");
}
#endif
