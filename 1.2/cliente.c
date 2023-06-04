#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pvm3.h"

#define MSG_TAG 1
#define MSG_SIZE 100

int main()
{
    int ptid;
    char message[MSG_SIZE];

    // Inicializar PVM
    ptid = pvm_parent();

    // Enviar mensaje al servidor
    strcpy(message, "Hola Mundo");
    pvm_initsend(PvmDataDefault);
    pvm_pkstr(message);
    pvm_send(ptid, MSG_TAG);

    printf("Mensaje enviado al servidor.\n");

    // Finalizar PVM
    pvm_exit();

    return 0;
}
