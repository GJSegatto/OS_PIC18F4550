#ifndef CONFIG_H
#define	CONFIG_H

#define ON                  1
#define OFF                 2

#define MAX_USER_TASKS      3
#define MAX_STACK_SIZE      32

#define DEFAULT_SCHEDULER   PRIORITY_SCHEDULER

#define IDLE_DEBUG          ON

#define DYNAMIC_MEM         OFF

#define PIPE_SIZE           3

#define MIN_PRIORITY        0           //MENOR PRIORIDADE = 0
#define MAX_PRIORITY        5           //MAIOR PRIORIDADE = 5

// Aplica��o exemplo

#define SISTEMA_CARRO       ON

// APP_2 exemplo das tarefas com sem�foro
#define TESTE               OFF

#endif	/* CONFIG_H */

