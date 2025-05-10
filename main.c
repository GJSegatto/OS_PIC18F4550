#include "kernel.h"
#include "user_app.h"


int main()
{
    os_init();
    
    // Cria tarefas de usu�rio
    create_task(1, 5, acelerador);
    create_task(2, 3, controle_central);
    create_task(3, 2, injecao_eletronica);
    
    os_start();
    
    while (1);
    
    return 0;
}
