#include <stdio.h>
#include "pvm3.h"

#define SLAVENAME "/home/juan/pvm/client"

int main() {
    int mytid, tids[20], n, nproc, numt, i, who, msgtype, loops;
    float data[10];
    int n_veces;

    if (pvm_parent() == PvmNoParent) {
        /* Retorna si es el proceso padre o hijo */
        loops = 1;
        printf("\n Cuántos hijos (120)? ");
        scanf("%d", &nproc);
        printf("\n Cuántos bucles de comunicación hijo-hijo (1 - 5000)? ");
        scanf("%d", &loops);
    }

    /* Redirecciona la entrada/salida de los hijos al padre */
    pvm_catchout(stdout);

    /* Crea los hijos */
    numt = pvm_spawn(SLAVENAME, (char**)0, 0, "", nproc, tids);
    printf("Resultado del Spawn: %d \n", numt);

    /* Ha podido? */
    if (numt < nproc) {
        printf("Error creando los hijos. Código de error:\n");
        for (i = numt; i < nproc; i++) {
            printf("Tid %d %d\n", i, tids[i]);
        }
        for (i = 0; i < numt; i++) {
            pvm_kill(tids[i]);
        }
        /* Mata los procesos con id en tids */
        pvm_exit();
        /* Termina */
    }

    /* Inicio del programa de padre, inicializa los datos */
    n = 10;
    for (i = 0; i < n; i++) {
        data[i] = 2.0;
    }

    /* Broadcast con datos iniciales a los slave */
    pvm_initsend(PvmDataDefault);
    /* Borra el buffer y especifica el message encoding */
    pvm_pkint(&loops, 1, 1);
    /* Empaqueta un dato en el buffer, 2.º N.º, 3*:stride */
    pvm_pkint(&nproc, 1, 1);
    pvm_pkint(tids, nproc, 1);
    pvm_pkint(&n, 1, 1);
    pvm_pkfloat(data, n, 1);
    pvm_mcast(tids, nproc, 0);
    /* Multicast en el buffer a los tids y espera el resultado de los hijos */
    msgtype = 5;
    for (i = 0; i < nproc; i++) {
        pvm_recv(-1, msgtype);
        /* Recibe un mensaje, -1 :de cualquiera, 2.ª :tag del msg */
        pvm_upkint(&who, 1, 1);
        /* Desempaqueta */
        printf("Terminó %d\n", who);
    }
    pvm_exit();

}
