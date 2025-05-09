# Sistema Operacional Embarcado - PIC18F4550

Este projeto implementa um sistema operacional embarcado simples para o microcontrolador PIC18F4550, desenvolvido para fins didáticos na disciplina de Sistemas Operacionais Embarcados.

## Funcionalidades
- Escalonamento de tarefas (Round Robin e por Prioridade com Aging)
- Suporte a semáforos e mutex para sincronização
- Pipes para comunicação entre tarefas
- Gerenciamento dinâmico de memória (heap)
- Suporte a tarefas de usuário e tarefa idle

## Estrutura do Projeto
- `kernel.c/h` - Núcleo do SO, gerenciamento de tarefas e inicialização
- `scheduler.c/h` - Algoritmos de escalonamento
- `sync.c/h` - Implementação de semáforos e mutex
- `pipe.c/h` - Comunicação entre tarefas via pipe
- `mem.c/h` - Gerenciamento dinâmico de memória
- `user_app.c/h` - Exemplos de aplicações e tarefas de usuário
- `timer.c/h` - Configuração e controle do timer
- `io.c/h` - Funções de entrada/saída

## Como usar
1. Configure as tarefas desejadas em `user_app.c`.
2. Compile o projeto utilizando MPLAB X ou CMake.
3. Grave o firmware no PIC18F4550 e execute.

## Observações
- O projeto é modular e pode ser expandido para incluir novos recursos.
- O código é voltado para fins educacionais.

---
Desenvolvido para a disciplina de Sistemas Operacionais Embarcados - UFSC 2025

# T1

## Structure

| Path                  | Purpose                                                                                      |
| --------------------- | -------------------------------------------------------------------------------------------- |
| _build                | Stores build artifacts, can be deleted                                                       |
| cmake                 | Generated [CMake](https://cmake.org/) files. May be deleted if user.cmake has not been added |
| .vscode               | See [VSCode Settings](https://code.visualstudio.com/docs/getstarted/settings)                |
| .vscode\T1.mplab.json | The MPLAB project file, should not be deleted                                                |
