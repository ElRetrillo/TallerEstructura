#include "GestorJuego.h"
#include <iostream>
#include <limits>
#include <string>

GestorJuego::GestorJuego(int profundidad_ia) :
    jugador_actual(JUGADOR_X),
    agente_ia(profundidad_ia), 
    estado_juego(EN_CURSO),
    modo_actual(HUMANO_VS_IA), 
    jugador_x_puede_intercambiar_color(true),
    jugador_o_puede_intercambiar_color(true),
    puntos_esquina_jugador_x(0),
    puntos_esquina_jugador_o(0),
    energia_jugador_x(0),
    energia_jugador_o(0)
{
    historial.push(tablero); 
}

void GestorJuego::establecer_modo_jugador(ModoJugador modo) {
    modo_actual = modo; 
    reiniciar_juego();
}

bool GestorJuego::intentar_mover_jugador(int fila, int columna) {
    if (estado_juego != EN_CURSO) {
        return false;
    }

    if (jugador_actual == JUGADOR_X || (jugador_actual == JUGADOR_O && modo_actual == HUMANO_VS_HUMANO)) {
        if (fila >= 0 && fila < TAMANO_TABLERO && columna >= 0 && columna < TAMANO_TABLERO) {
            std::vector<std::pair<int, int>> fichas_volteadas_por_mov = tablero.obtener_fichas_a_voltear(fila, columna, jugador_actual);
            if (!fichas_volteadas_por_mov.empty()) {
                historial.push(tablero);
                tablero.realizar_movimiento(fila, columna, jugador_actual);
                ganar_energia_por_volteo(jugador_actual, fichas_volteadas_por_mov.size());
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

void GestorJuego::procesar_turno_ia() {
    if (estado_juego != EN_CURSO || jugador_actual != JUGADOR_O || modo_actual != HUMANO_VS_IA) {
        return;
    }

    std::cout << "La IA está pensando..." << std::endl;
    Movimiento mejor_movimiento = agente_ia.encontrar_mejor_movimiento(*this, jugador_actual);

    if (mejor_movimiento.fila != -1) {
        std::vector<std::pair<int, int>> fichas_volteadas_por_mov = tablero.obtener_fichas_a_voltear(mejor_movimiento.fila, mejor_movimiento.columna, jugador_actual);
        tablero.realizar_movimiento(mejor_movimiento.fila, mejor_movimiento.columna, jugador_actual);
        ganar_energia_por_volteo(jugador_actual, fichas_volteadas_por_mov.size());
        verificar_dominacion_esquinas(jugador_actual); 
        std::cout << "La IA juega en: " << mejor_movimiento.fila + 1 << " " << mejor_movimiento.columna + 1 << std::endl;
    } else {
        std::cout << "La IA no tiene movimientos validos." << std::endl;
    }
    actualizar_estado_juego();

    if (estado_juego == EN_CURSO) {
        cambiar_turno();
    }
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
    energia_jugador_x = 0;
    energia_jugador_o = 0;
}

char GestorJuego::obtener_jugador_actual() const {
    return jugador_actual;
}

EstadoJuego GestorJuego::obtener_estado_juego() const {
    return estado_juego;
}

ModoJugador GestorJuego::obtener_modo_jugador() const {
    return modo_actual;
}

std::string GestorJuego::obtener_nombre_jugador(char caracter_jugador) const {
    if (caracter_jugador == JUGADOR_X) {
        return "Jugador 1 (Negro)";
    } else {
        if (modo_actual == HUMANO_VS_IA) {
            return "IA (Blanco)";
        } else {
            return "Jugador 2 (Blanco)";
        }
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
        tablero.hash_actual = tablero.calcular_hash_zobrist();
        historial.push(tablero);
        return true;
    }
    return false;
}

void GestorJuego::verificar_dominacion_esquinas_publica(char jugador) {
    verificar_dominacion_esquinas(jugador);
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

int GestorJuego::obtener_energia(char jugador) const {
    if (jugador == JUGADOR_X) return energia_jugador_x;
    if (jugador == JUGADOR_O) return energia_jugador_o;
    return 0;
}

bool GestorJuego::puede_comprar_habilidad(char jugador, int costo) const {
    if (jugador == JUGADOR_X) return energia_jugador_x >= costo;
    if (jugador == JUGADOR_O) return energia_jugador_o >= costo;
    return false;
}

void GestorJuego::gastar_energia(char jugador, int costo) {
    if (jugador == JUGADOR_X) energia_jugador_x -= costo;
    if (jugador == JUGADOR_O) energia_jugador_o -= costo;
}

void GestorJuego::ganar_energia_por_volteo(char jugador, int fichas_volteadas) {
    int energia_ganada = fichas_volteadas * RECURSO_POR_FICHA_VOLTEADA;
    if (jugador == JUGADOR_X) energia_jugador_x += energia_ganada;
    if (jugador == JUGADOR_O) energia_jugador_o += energia_ganada;
}

bool GestorJuego::realizar_volteo_forzado(int fila, int columna, char jugador) {
    char oponente = (jugador == JUGADOR_X) ? JUGADOR_O : JUGADOR_X;
    if (fila >= 0 && fila < TAMANO_TABLERO && columna >= 0 && columna < TAMANO_TABLERO && tablero.tablero[fila][columna] == oponente) {
        tablero.tablero[fila][columna] = jugador;
        tablero.hash_actual = tablero.calcular_hash_zobrist();
        historial.push(tablero);
        return true;
    }
    return false;
}