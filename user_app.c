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

#if APP_1 == ON

TASK tarefa_1()
{
    while(1) {
        activate_pwm();
    }
}

TASK tarefa_2()
{
    while (1) {
        Nop();
    }    
}

TASK tarefa_3()
{
    while (1) {
        LATDbits.LD2 = ~PORTDbits.RD2;
   }
}

void user_config()
{
    pwm_config();

    TRISDbits.RD0 = 0;
    TRISDbits.RD1 = 0;
    TRISDbits.RD2 = 0;
    
    // Define as tarefas como fun��es globais para
    // evitar que o compilador as retire na fase
    // de gera��o de otimiza��o.
    asm("global _tarefa_1, _tarefa_2, _tarefa_3");
}

#elif APP_2 == ON

sem_t s;

TASK tarefa_1()
{
    while (1) {
        LATDbits.LATD7 = ~PORTDbits.RD7;
        //sem_wait(&s);
        LATDbits.LD0 = ~PORTDbits.RD0;
        //change_state(WAITING);
        //delay(50);
        
    }    
}

TASK tarefa_2()
{
    while (1) {
        //sem_wait(&s);
        LATDbits.LD1 = ~PORTDbits.RD1;
        //change_state(WAITING);
    }    
}

TASK tarefa_3()
{
    while (1) {
        //sem_wait(&s);
        LATDbits.LD2 = ~PORTDbits.RD2;
        //change_state(WAITING);
    }
}

void user_config()
{
    TRISDbits.RD0 = 0;
    TRISDbits.RD1 = 0;
    TRISDbits.RD2 = 0;
   
    // Inicializar o sem�foro
    sem_init(&s, 0);
    
    // Define as tarefas como fun��es globais para
    // evitar que o compilador as retire na fase
    // de gera��o de otimiza��o.
    asm("global _tarefa_1, _tarefa_2, _tarefa_3");
}

#elif APP_3 == ON

pipe_t pipe;

TASK tarefa_1()
{
    while (1) {
        LATDbits.LD0 = 1;
        //LATDbits.LD0 = ^=1;
    }    
}

TASK tarefa_2()
{
    uint8_t dados[3] = {'L', 'D', 'D'};
    int i = 0;
    
    while (1) {
        write_pipe(&pipe, dados[i]);
        i = (i+1) % 3;
        LATDbits.LD1 = ~PORTDbits.RD1;        
    }    
}

TASK tarefa_3()
{
    uint8_t dado_pipe = 0;
    while (1) {
        read_pipe(&pipe, &dado_pipe);
        if (dado_pipe == 'L') {
            LATDbits.LD2 = 1;
        }
        else if (dado_pipe == 'D') {
            LATDbits.LD2 = 0;
        }
        destroy_pipe(&pipe);
        delay(10);
    }
}

void user_config()
{
    TRISDbits.RD0 = 0;
    TRISDbits.RD1 = 0;
    TRISDbits.RD2 = 0;
   
    create_pipe(&pipe);

    asm("global _tarefa_1, _tarefa_2, _tarefa_3");
}

#elif APP_4 == ON

mutex_t m;

TASK tarefa_1()
{
    while (1) {
        di();
        mutex_lock(&m);
        LATDbits.LD0 = 1;
        
        delay(10);

        LATDbits.LD0 = 0;
        mutex_unlock(&m);

        delay(10);
        ei();
    }
    
}

TASK tarefa_2()
{
    while (1) {
        di();
        mutex_lock(&m);
        LATDbits.LD1 = 1;
        
        delay(10);

        LATDbits.LD1 = 0;
        mutex_unlock(&m);

        delay(10);
        ei();
    }    
}

TASK tarefa_3()
{
    while (1) {
        di();
        mutex_lock(&m);
        LATDbits.LD2 = 1;
        
        delay(10);

        LATDbits.LD2 = 0;
        mutex_unlock(&m);

        delay(10);
        ei();
    } 
}

void user_config()
{
    TRISDbits.RD0 = 0;
    TRISDbits.RD1 = 0;
    TRISDbits.RD2 = 0;

    PORTDbits.RD0 = 0;
    PORTDbits.RD1 = 0;
    PORTDbits.RD2 = 0;


    mutex_init(&m);

    asm("global _tarefa_1, _tarefa_2, _tarefa_3");
}

#elif APP_5 == ON

TASK tarefa_1() //PRIORIDADE 5
{
    while (1) {
        adc_config();
        unsigned int adc_value = 0;
        
        while(1) {
            adc_value = adc_read();

            if(adc_value >= 0 && adc_value <= 500) {
            PORTDbits.RD0 = 1; // LED ON
            PORTDbits.RD1 = 0; // LED OFF
            }
            else if(adc_value > 500 && adc_value <= 1023) {
                PORTDbits.RD0 = 0; // LED OFF
                PORTDbits.RD1 = 1; // LED ON
            }
        }
    }
    
}

TASK tarefa_2() //PRIORIDADE 3
{
    while(1) {
        Nop();
    }  
}

TASK tarefa_3() //PRIORIDADE 2
{
    while(1) {
        Nop();
    }
}

void user_config()
{
    TRISDbits.RD0 = 0;
    TRISDbits.RD1 = 0;
    TRISAbits.RA0 = 1;

    PORTDbits.RD0 = 0;
    PORTDbits.RD1 = 0;

    asm("global _tarefa_1, _tarefa_2, _tarefa_3");
}

#endif
