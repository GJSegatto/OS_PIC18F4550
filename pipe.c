#include "pipe.h"
#include "mem.h"
#include <stdint.h>

void create_pipe(pipe_t *p)
{
    p->pipe_msg = (uint16_t*)SRAMalloc(sizeof(uint16_t)); // Alocação dinâmica
    p->pipe_pos_read    = 0;
    p->pipe_pos_write   = 0;
    sem_init(&p->pipe_sem_read, 0);
    sem_init(&p->pipe_sem_write, PIPE_SIZE);
}

void write_pipe(pipe_t *p, uint16_t data)
{
    di();
    
    sem_wait(&p->pipe_sem_write);
    p->pipe_msg[p->pipe_pos_write] = data;
    p->pipe_pos_write =  (p->pipe_pos_write+1) % PIPE_SIZE;
    sem_post(&p->pipe_sem_read);   
    
    ei();    
}

void read_pipe(pipe_t *p, uint16_t *data)
{
    di();
    
    sem_wait(&p->pipe_sem_read);
    *data = p->pipe_msg[p->pipe_pos_read];
    p->pipe_pos_read = (p->pipe_pos_read+1) % PIPE_SIZE;
    sem_post(&p->pipe_sem_write);
    
    ei();
}

// Função opcional para liberar o buffer do pipe
void destroy_pipe(pipe_t *p)
{
    if (p->pipe_msg) {
        SRAMfree(p->pipe_msg);
        p->pipe_msg = 0;
    }
}

