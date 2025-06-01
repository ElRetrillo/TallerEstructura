#include "GestorJuego.h"
#include <iostream>
#include <limits>
#include <string>

GestorJuego::GestorJuego() :
    jugador_actual(JUGADOR_X),
    estado_juego(EN_CURSO),
    modo_actual(HUMANO_VS_HUMANO), 
    jugador_x_puede_intercambiar_color(true),
    jugador_o_puede_intercambiar_color(true),
    puntos_esquina_jugador_x(0),
    puntos_esquina_jugador_o(0)
{
    historial.push(tablero); 
}

void GestorJuego::establecer_modo_jugador(ModoJugador modo) {
    modo_actual = HUMANO_VS_HUMANO; 
    reiniciar_juego();
}

bool GestorJuego::intentar_mover_jugador(int fila, int columna) {
    if (estado_juego != EN_CURSO) {
        return false;
    }

    if (jugador_actual == JUGADOR_X || jugador_actual == JUGADOR_O) {
        if (fila >= 0 && fila < TAMANO_TABLERO && columna >= 0 && columna < TAMANO_TABLERO) {
            if (tablero.es_movimiento_valido(fila, columna, jugador_actual)) {
                historial.push(tablero);
                tablero.realizar_movimiento(fila, columna, jugador_actual);
                verificar_dominacion_esquinas(jugador_actual);
                actualizar_estado_juego();

                if (estado_juego == EN_CURSO) {
                    cambiar_turno();
                }
                return true;
            }
        }
    }
    return false;
}

void GestorJuego::cambiar_turno() {
    char siguiente_jugador_candidato = (jugador_actual == JUGADOR_X) ? JUGADOR_O : JUGADOR_X;

    bool jugador_actual_tiene_movimientos = !tablero.obtener_movimientos_posibles(jugador_actual).empty();
    bool siguiente_jugador_tiene_movimientos = !tablero.obtener_movimientos_posibles(siguiente_jugador_candidato).empty();

    if (siguiente_jugador_tiene_movimientos) {
        jugador_actual = siguiente_jugador_candidato;
    } else if (jugador_actual_tiene_movimientos) {
        std::cout << "¡" << siguiente_jugador_candidato << " no tiene movimientos! " << jugador_actual << " vuelve a jugar." << std::endl;
    } else {
        std::cout << "¡Ambos jugadores no tienen movimientos! Juego terminado." << std::endl;
    }
}

void GestorJuego::actualizar_estado_juego() {
    if (tablero.es_juego_terminado(JUGADOR_X, JUGADOR_O)) {
        int puntuacion_X = tablero.contar_fichas(JUGADOR_X) + puntos_esquina_jugador_x;
        int puntuacion_O = tablero.contar_fichas(JUGADOR_O) + puntos_esquina_jugador_o;

        if (puntuacion_X > puntuacion_O) {
            estado_juego = JUGADOR_X_GANO;
        } else if (puntuacion_O > puntuacion_X) {
            estado_juego = JUGADOR_O_GANO;
        } else {
            estado_juego = EMPATE;
        }
    } else {
        estado_juego = EN_CURSO;
    }
}

void GestorJuego::reiniciar_juego() {
    tablero = Tablero();
    jugador_actual = JUGADOR_X;
    estado_juego = EN_CURSO;
    while (!historial.empty()) {
        historial.pop();
    }
    historial.push(tablero);

    jugador_x_puede_intercambiar_color = true;
    jugador_o_puede_intercambiar_color = true;
    puntos_esquina_jugador_x = 0;
    puntos_esquina_jugador_o = 0;
}

char GestorJuego::obtener_jugador_actual() const {
    return jugador_actual;
}

EstadoJuego GestorJuego::obtener_estado_juego() const {
    return estado_juego;
}

ModoJugador GestorJuego::obtener_modo_jugador() const {
    return HUMANO_VS_HUMANO; 
}

std::string GestorJuego::obtener_nombre_jugador(char caracter_jugador) const {
    if (caracter_jugador == JUGADOR_X) {
        return "Jugador 1 (Negro)";
    } else {
        return "Jugador 2 (Blanco)";
    }
}

bool GestorJuego::puede_intercambiar_color(char jugador) const {
    if (jugador == JUGADOR_X) return jugador_x_puede_intercambiar_color;
    if (jugador == JUGADOR_O) return jugador_o_puede_intercambiar_color;
    return false;
}

void GestorJuego::usar_intercambio_color(char jugador) {
    if (jugador == JUGADOR_X) jugador_x_puede_intercambiar_color = false;
    if (jugador == JUGADOR_O) jugador_o_puede_intercambiar_color = false;
}

bool GestorJuego::realizar_intercambio_color(int fila, int columna, char jugador) {
    char oponente = (jugador == JUGADOR_X) ? JUGADOR_O : JUGADOR_X;
    if (tablero.tablero[fila][columna] == oponente) {
        tablero.tablero[fila][columna] = jugador;
        historial.push(tablero);
        return true;
    }
    return false;
}

void GestorJuego::verificar_dominacion_esquinas(char jugador) {
    int esquinas[4][2] = {{0, 0}, {0, TAMANO_TABLERO - 1}, {TAMANO_TABLERO - 1, 0}, {TAMANO_TABLERO - 1, TAMANO_TABLERO - 1}};
    int valor_esquina = 10; 

    int actuales_esquinas_x = 0;
    int actuales_esquinas_o = 0;

    for (int i = 0; i < 4; ++i) {
        int fila = esquinas[i][0];
        int columna = esquinas[i][1];
        if (tablero.tablero[fila][columna] == JUGADOR_X) {
            actuales_esquinas_x += valor_esquina;
        } else if (tablero.tablero[fila][columna] == JUGADOR_O) {
            actuales_esquinas_o += valor_esquina;
        }
    }
    puntos_esquina_jugador_x = actuales_esquinas_x;
    puntos_esquina_jugador_o = actuales_esquinas_o;
}