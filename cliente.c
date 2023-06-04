#include <stdio.h>
#include "pvm3.h"

float work(int me, float *data, int *tids, int nproc, int loops);

int main() {
    int mytid; /* Mi task id */
    int tids[20]; /* Task ids */
    int n, me, i, nproc, master, msgtype, loops;
    float data[10];
    long result[4];

    mytid = pvm_mytid();
    msgtype = 0;
    pvm_recv(-1, msgtype);
    pvm_upkint(&loops, 1, 1);
    pvm_upkint(&nproc, 1, 1);
    pvm_upkint(tids, nproc, 1);
    pvm_upkint(&n, 1, 1);
    pvm_upkfloat(data, n, 1);

    /* Determina qué hijo es (0 – nproc-1) */
    for (i = 0; i < nproc; i++) {
        if (mytid == tids[i]) {
            me = i;
            break;
        }
    }

    /* Procesa y pasa los datos entre vecinos */
    work(me, data, tids, nproc, loops);

    /* Envía los datos al máster */
    pvm_initsend(PvmDataDefault);
    pvm_pkint(&me, 1, 1);
    msgtype = 5;
    master = pvm_parent(); /* Averigua quién lo creó */
    pvm_send(master, msgtype);
    pvm_exit();

    return 0;
}

float work(int me, float *data, int *tids, int nproc, int loops) {
    int i, j, dest;
    float psum = 0.0, sum = 0.1;

    for (j = 1; j <= loops; j++) {
        pvm_initsend(PvmDataDefault);
        pvm_pkfloat(&sum, 1, 1);
        dest = me + 1;
        if (dest == nproc) dest = 0;
        pvm_send(tids[dest], 22);
        i = me - 1;
        if (me == 0) i = nproc - 1;
        pvm_recv(tids[i], 22);
        pvm_upkfloat(&psum, 1, 1);
    }
}
