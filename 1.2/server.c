#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pvm3.h"

#define MSG_TAG 1
#define MSG_SIZE 100

int main()
{
    int tid;
    char message[MSG_SIZE];

    // Inicializar PVM
    pvm_recv(-1, MSG_TAG);
    pvm_upkstr(message);

    printf("Mensaje recibido del cliente: %s\n", message);

    // Finalizar PVM
    pvm_exit();

    return 0;
}
