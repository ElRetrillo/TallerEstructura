#ifndef TABLERO_H
#define TABLERO_H

#include "ConfiguracionJuego.h"
#include <vector>
#include <iostream>
#include "Movimiento.h"

class Tablero {
public:
    std::vector<std::vector<char>> tablero;

    Tablero();
    Tablero(const Tablero& otro);

    void inicializar_tablero();
    void mostrar_tablero() const;
    std::vector<std::pair<int, int>> obtener_fichas_a_voltear(int fila, int columna, char jugador) const;
    bool es_movimiento_valido(int fila, int columna, char jugador) const;
    bool realizar_movimiento(int fila, int columna, char jugador);
    std::vector<Movimiento> obtener_movimientos_posibles(char jugador) const;
    int contar_fichas(char jugador) const;
    bool es_juego_terminado(char jugador_x, char jugador_o) const;
};

#endif