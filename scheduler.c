#include "scheduler.h"
#include "config.h"
#include "types.h"
#include <stdint.h>

// Refer�ncia para a fila de aptos
extern ready_queue_t r_queue;
uint8_t last_active_task = 0;       //ÚLTIMA TASK ESCOLHIDA PELO ESCALONADOR

void __reentrant scheduler()
{
    #if DEFAULT_SCHEDULER == RR_SCHEDULER
    // chama rr scheduler
    rr_scheduler();
    #elif DEFAULT_SCHEDULER == PRIORITY_SCHEDULER
    // chama priority_scheduler
    priority_scheduler();
    #endif
}

void __reentrant rr_scheduler()
{
    uint8_t idle_task = 0;
    do {    
        r_queue.task_running = (r_queue.task_running+1) % r_queue.ready_queue_size;
        if (r_queue.task_running == 0) {
            idle_task++;
            if (idle_task == 2) break;
        }
    } while (r_queue.ready_queue[r_queue.task_running].task_state != READY ||
             r_queue.task_running == 0);
}

void __reentrant priority_scheduler()
{
    // Ordenar a fila de aptos por prioridade
    uint8_t highest_priority = 0;
    uint8_t next_task = 0;

    for(uint8_t i = 0; i < r_queue.ready_queue_size; i++) {
        if(r_queue.ready_queue[i].task_state == READY) {
            //AGING PARA NÃO TER STARVATION
            if(r_queue.ready_queue[i].task_priority < MAX_PRIORITY) {
                r_queue.ready_queue[i].task_priority++;
            }
            //SELEÇÃO DE MAIOR PRIORIDADE NA FILA
            if(r_queue.ready_queue[i].task_priority > highest_priority) {
                highest_priority = r_queue.ready_queue[i].task_priority;
            }
        }
    }

    next_task = last_active_task;
    for(uint8_t i = 1; i <= r_queue.ready_queue_size; i++) {
        uint8_t idx = (last_active_task + i) % r_queue.ready_queue_size;
        if(r_queue.ready_queue[idx].task_state == READY && r_queue.ready_queue[idx].task_priority == highest_priority) {
            next_task = r_queue.ready_queue[idx].task_id;
            break;
        }
    }

    r_queue.task_running = next_task;
    last_active_task = next_task;
    r_queue.ready_queue[next_task].task_priority = r_queue.ready_queue[next_task].original_priority;
}