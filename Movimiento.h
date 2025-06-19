#ifndef MOVIMIENTO_H
#define MOVIMIENTO_H

struct Movimiento {
    int fila;
    int columna;

    bool operator==(const Movimiento& otro) const {
        return fila == otro.fila && columna == otro.columna;
    }
};

#endif