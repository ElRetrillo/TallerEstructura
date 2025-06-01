#include "Tablero.h"
#include "ConfiguracionJuego.h" 

Tablero::Tablero() : tablero(TAMANO_TABLERO, std::vector<char>(TAMANO_TABLERO, VACIO)) {
    inicializar_tablero();
}

Tablero::Tablero(const Tablero& otro) : tablero(otro.tablero) {
}

void Tablero::inicializar_tablero() {
    tablero[3][3] = JUGADOR_O;
    tablero[3][4] = JUGADOR_X;
    tablero[4][3] = JUGADOR_X;
    tablero[4][4] = JUGADOR_O;
}

void Tablero::mostrar_tablero() const {
    std::cout << "  ";
    for (int i = 0; i < TAMANO_TABLERO; ++i) {
        std::cout << i + 1 << " ";
    }
    std::cout << std::endl;
    for (int i = 0; i < TAMANO_TABLERO; ++i) {
        std::cout << i + 1 << " ";
        for (int j = 0; j < TAMANO_TABLERO; ++j) {
            std::cout << tablero[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

std::vector<std::pair<int, int>> Tablero::obtener_fichas_a_voltear(int fila, int columna, char jugador) const {
    std::vector<std::pair<int, int>> a_voltear;
    if (tablero[fila][columna] != VACIO) {
        return a_voltear;
    }

    char oponente = (jugador == JUGADOR_X) ? JUGADOR_O : JUGADOR_X;

    int df[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dc[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    for (int i = 0; i < 8; ++i) {
        std::vector<std::pair<int, int>> fichas_direccion_actual;
        int fila_actual = fila + df[i];
        int col_actual = columna + dc[i];

        while (fila_actual >= 0 && fila_actual < TAMANO_TABLERO && col_actual >= 0 && col_actual < TAMANO_TABLERO &&
               tablero[fila_actual][col_actual] == oponente) {
            fichas_direccion_actual.push_back({fila_actual, col_actual});
            fila_actual += df[i];
            col_actual += dc[i];
        }

        if (fila_actual >= 0 && fila_actual < TAMANO_TABLERO && col_actual >= 0 && col_actual < TAMANO_TABLERO &&
            tablero[fila_actual][col_actual] == jugador) {
            a_voltear.insert(a_voltear.end(), fichas_direccion_actual.begin(), fichas_direccion_actual.end());
        }
    }
    return a_voltear;
}

bool Tablero::es_movimiento_valido(int fila, int columna, char jugador) const {
    return !obtener_fichas_a_voltear(fila, columna, jugador).empty();
}

bool Tablero::realizar_movimiento(int fila, int columna, char jugador) {
    std::vector<std::pair<int, int>> a_voltear = obtener_fichas_a_voltear(fila, columna, jugador);

    if (a_voltear.empty()) {
        return false;
    }

    tablero[fila][columna] = jugador;
    for (const auto& p : a_voltear) {
        tablero[p.first][p.second] = jugador;
    }
    return true;
}

std::vector<Movimiento> Tablero::obtener_movimientos_posibles(char jugador) const {
    std::vector<Movimiento> movimientos_posibles;
    for (int fila = 0; fila < TAMANO_TABLERO; ++fila) {
        for (int columna = 0; columna < TAMANO_TABLERO; ++columna) {
            if (es_movimiento_valido(fila, columna, jugador)) {
                movimientos_posibles.push_back({fila, columna});
            }
        }
    }
    return movimientos_posibles;
}

int Tablero::contar_fichas(char jugador) const {
    int contador = 0;
    for (int fila = 0; fila < TAMANO_TABLERO; ++fila) {
        for (int columna = 0; columna < TAMANO_TABLERO; ++columna) {
            if (tablero[fila][columna] == jugador) {
                contador++;
            }
        }
    }
    return contador;
}

bool Tablero::es_juego_terminado(char jugador_x, char jugador_o) const {
    return obtener_movimientos_posibles(jugador_x).empty() && obtener_movimientos_posibles(jugador_o).empty();
}