#include "kernel.h"
#include "user_app.h"


int main()
{
    os_init();
    
    // Cria tarefas de usuario
    create_task(1, 3, acelerador);
    create_task(2, 2, controle_central);
    create_task(3, 4, injecao_eletronica);
    
    os_start();
    
    while (1);
    
    return 0;
}
