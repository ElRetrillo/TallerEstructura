#ifndef IA_H
#define IA_H

class GestorJuego; 

#include "ConfiguracionJuego.h" 
#include <limits> 
#include <map>
#include "Tablero.h"
#include "Movimiento.h"
#include "TablaTransposicion.h" 

class IA {
public:
    int profundidad_maxima;
    std::map<unsigned long long, EntradaTransposicion> tabla_transposicion;

    IA(int profundidad);

    Movimiento encontrar_mejor_movimiento(GestorJuego& gestor_juego_actual, char jugador_ia);

private:
    int evaluar_tablero(const GestorJuego& gestor_juego_actual, char jugador_ia) const;

    int minimax(GestorJuego gestor_juego_estado, int profundidad, bool es_maximizador, int alfa, int beta, char jugador_actual);
};

#endif