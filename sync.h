#ifndef SYNC_H
#define	SYNC_H

#include "config.h"
#include <stdint.h>
#include <stdbool.h>

// Estrutura de dados do semáforo
typedef struct sem {
    int16_t s_value;
    uint8_t s_queue[MAX_USER_TASKS];
    uint8_t s_size;
    uint8_t s_pos_out;
} sem_t;

// API para o semáforo
void sem_init(sem_t *sem, int16_t value);
void sem_wait(sem_t *sem);
void sem_post(sem_t *sem);

// Estrutura de dados do mutex
typedef struct mutex {
    bool is_free;                           // TRUE -> mutex livre; FALSE -> mutex ocupado
    uint8_t owner;                          //Processo que tem o mutex
    uint8_t queue[MAX_USER_TASKS];          // Fila de espera do mutex
    uint8_t queue_size;                     // Tamanho da fila de processos na fila de espera
    uint8_t pos_in;                         // Posição de entrada na fila de espera
    uint8_t pos_out;                        // Posição de saída da fila de espera do mutex
} mutex_t;

// API para o mutex
void mutex_init(mutex_t *mutex);
void mutex_lock(mutex_t *mutex);
void mutex_unlock(mutex_t *mutex);


#endif	/* SYNC_H */

