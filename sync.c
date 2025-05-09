#include "sync.h"
#include "config.h"
#include "types.h"
#include "kernel.h"
#include "scheduler.h"
#include <stdbool.h>
#include <xc.h>

// Declara fila de aptos
extern ready_queue_t r_queue;

// API para o semáfiro
void sem_init(sem_t *sem, int16_t value)
{
    sem->s_value            = value;
    sem->s_size             = 0;
    sem->s_pos_out          = 0;
}

void sem_wait(sem_t *sem)
{
    di();
    
    sem->s_value--;
    if (sem->s_value < 0) {
        // Bloqueia a tarefa
        sem->s_queue[sem->s_size] = r_queue.task_running;
        sem->s_size = (sem->s_size+1) % MAX_USER_TASKS;
        // For�a a preemp��o
        SAVE_CONTEXT(SEM_WAITING);
        scheduler();
        RESTORE_CONTEXT();
    }
    
    ei();    
}

void sem_post(sem_t *sem)
{
    di();
    
    sem->s_value++;
    
    if (sem->s_value <= 0) {
        // Desbloqueia tarefa mais antiga
        r_queue.ready_queue[sem->s_queue[sem->s_pos_out]].task_state = READY;
        sem->s_pos_out = (sem->s_pos_out + 1) % MAX_USER_TASKS;        
    }
    
    ei();
}

// API para o mutex
void mutex_init(mutex_t *mutex)
{
    mutex->is_free = true;  // Seção crítica liberada
    mutex->owner = 0;
    mutex->queue_size = 0;
    mutex->pos_in = 0;
    mutex->pos_out = 0;
}

void mutex_lock(mutex_t *mutex)
{
    di();
    
    if (mutex->is_free) {
        mutex->is_free = false;
        mutex->owner = r_queue.task_running;
    }
    else {
        mutex->queue[mutex->pos_in] = r_queue.task_running;
        mutex->pos_in = (mutex->pos_in + 1) % MAX_USER_TASKS;
        mutex->queue_size++;
        SAVE_CONTEXT(MUTEX_WAITING);
        scheduler();
        RESTORE_CONTEXT();
    }
    
    ei();
}

void mutex_unlock(mutex_t *mutex)
{
    di();
    
    if(mutex->owner == r_queue.task_running) {
        if(mutex->queue_size > 0) {
            mutex->is_free = false;
            mutex->owner = mutex->queue[mutex->pos_out];
            r_queue.ready_queue[mutex->queue[mutex->pos_out]].task_state = READY;
            mutex->queue_size--;
            mutex->pos_out = (mutex->pos_out + 1) % MAX_USER_TASKS;
        }
        else {
            mutex->is_free = true;
        }
    }
    
    ei();
}

