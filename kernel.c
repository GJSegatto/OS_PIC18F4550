#include "kernel.h"
#include "scheduler.h"
#include "user_app.h"
#include "timer.h"
#include "config.h"
#include <language_support.h>
#include <pic18f4550.h>

// Declara fila de aptos
ready_queue_t r_queue;

void create_task(uint8_t id, uint8_t priority, f_ptr task)
{
    tcb_t new_task;
    
    new_task.task_id            = id;
    new_task.original_priority  = priority;
    new_task.task_priority      = priority;
    new_task.task_f             = task;
    
    new_task.task_sp            = 0;
    new_task.time_sleeping      = 0;
    new_task.task_state         = READY;
    
    new_task.BSR_REG            = 0x000;
    new_task.W_REG              = 0x000;
    new_task.STATUS_REG         = 0x000;
    
    r_queue.ready_queue[r_queue.ready_queue_size] = new_task;
    r_queue.ready_queue_size += 1;
}

void delay(uint16_t time)
{
    di();
    
    SAVE_CONTEXT(WAITING);
    r_queue.ready_queue[r_queue.task_running].time_sleeping = time;
    scheduler();
    RESTORE_CONTEXT();
    
    ei();
}

void yield()
{
    di();
    
    SAVE_CONTEXT(READY);
    scheduler();
    RESTORE_CONTEXT();
    
    ei();
}

void os_init()
{
    r_queue.ready_queue_size    = 0;
    r_queue.task_running        = 0;
    
    // Cria a tarefa idle
    create_task(0, 0, idle);
    asm("global _idle");
    
    config_timer0();
    
    #if DYNAMIC_MEM == ON
    SRAMInitHeap();
    #endif
    
}

void os_start()
{
    #if DEFAULT_SCHEDULER == PRIORITY_SCHEDULER
    // Ordenada a fila de aptos
    #endif
    user_config();
    
    // Habilita as interrup��es    
    ei();
    
    // Liga o timer
    start_timer0();
}

void change_state(state_t new_state)
{
    di();
    
    SAVE_CONTEXT(new_state);
    scheduler();
    RESTORE_CONTEXT();
    
    ei();    
}

// Tarefa idle
TASK idle()
{
    #if IDLE_DEBUG == ON
    TRISDbits.RD7 = 0;
    #endif    
    
    while (1) {
        #if IDLE_DEBUG == ON
        LATDbits.LATD7 = ~PORTDbits.RD7;
        #endif
        Nop();
    }
}

void decrease_time(void)
{
    for (uint8_t i = 1; i < r_queue.ready_queue_size; i++) {
        if (r_queue.ready_queue[i].time_sleeping > 0) {
            r_queue.ready_queue[i].time_sleeping--;
            if (r_queue.ready_queue[i].time_sleeping == 0) {
                r_queue.ready_queue[i].task_state = READY;
            }
        }
    }
}

void __interrupt(high_priority) high_int(void) {
    if(INTCONbits.INT0F) {
        while(PORTBbits.RB0) {
            LATDbits.LATD0 = 1;
        }
        LATDbits.LATD0 = 0;
        INTCONbits.INT0F = 0;
    }

    if(INTCONbits.TMR0IF) {
        di();
    
        // Seta o flag do timer em zero
        INTCONbits.TMR0IF   = 0;
        // Valor inicial do timer
        TMR0 = 0;
        
        // Decrementa o delay das tarefas que est�o em estado 
        // de waiting
        decrease_time();
        
        // Salva o contexto da tarefa que est� em execu��o
        SAVE_CONTEXT(READY);
        // Chama o escalonador para definir qual a pr�xima tarefa ser� executada
        scheduler();
        // Restaura o contexto da tarefa que entrar� em execu��o
        RESTORE_CONTEXT();
        
        ei();
    }
}
