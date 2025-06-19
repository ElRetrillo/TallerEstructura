#include "TablaTransposicion.h"
#include "ConfiguracionJuego.h"

unsigned long long TABLA_ZOBRIST[TAMANO_TABLERO][TAMANO_TABLERO][2];
std::mt19937_64 generador_aleatorio(std::chrono::system_clock::now().time_since_epoch().count());

void inicializar_tabla_zobrist() {
    for (int i = 0; i < TAMANO_TABLERO; ++i) {
        for (int j = 0; j < TAMANO_TABLERO; ++j) {
            TABLA_ZOBRIST[i][j][0] = generador_aleatorio(); 
            TABLA_ZOBRIST[i][j][1] = generador_aleatorio(); 
        }
    }
}