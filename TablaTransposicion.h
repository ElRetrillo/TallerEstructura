#ifndef TABLA_TRANSPOSICION_H
#define TABLA_TRANSPOSICION_H

#include "ConfiguracionJuego.h" // Primero

#include <random>
#include <chrono>

extern unsigned long long TABLA_ZOBRIST[TAMANO_TABLERO][TAMANO_TABLERO][2];
extern std::mt19937_64 generador_aleatorio;

void inicializar_tabla_zobrist();

enum BanderaTransposicion {
    EXACTO,
    LIMITE_INFERIOR,
    LIMITE_SUPERIOR
};

struct EntradaTransposicion {
    int valor;
    int profundidad;
    BanderaTransposicion bandera;
};

#endif